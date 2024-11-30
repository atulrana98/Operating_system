#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

// Externally defined variables and functions
extern struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;
extern int edf_count;
extern int rms_count;
extern uint ticks;

// Function to set the arrival time of a process
void set_arrival_time(unsigned int *arrival_time){
	*arrival_time = ticks;
}

// Array to store Luiland bound values
float threshold_array[64] = {1.0000, 0.8284, 0.7798, 0.7568, 0.7435, 0.7348, 0.7286, 0.7241, 0.7205, 0.7177, 0.7155, 0.7136, 0.7120, 0.7106, 0.7094, 0.7084, 0.7075, 0.7067, 0.7059, 0.7053, 0.7047, 0.7042, 0.7037, 0.7033, 0.7028, 0.7025, 0.7021, 0.7018, 0.7015, 0.7012, 0.7010, 0.7007, 0.7005, 0.7003, 0.7001, 0.6999, 0.6997, 0.6995, 0.6993, 0.6992, 0.6990, 0.6989, 0.6988, 0.6986, 0.6985, 0.6984, 0.6983, 0.6982, 0.6981, 0.6980, 0.6979, 0.6978, 0.6977, 0.6976, 0.6975, 0.6975, 0.6974, 0.6973, 0.6972, 0.6972, 0.6971, 0.6970, 0.6970, 0.6969};

// Functions for necessary operations
int 
max_value(int num1,int num2){
	return num1>num2 ? num1 : num2;
}

int 
ceil_value(float num){
	return (int)num+1;
}


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Assignment system calls from here onwards.

// sys_sched_policy()
// Function to handle Early Deadline First (EDF) policy
int 
handle_edf_policy(int pid) {
    float cpu_utilization = 0.0;
    struct proc *p;
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if ((p->sched_policy == 0 && p->state != UNUSED) || p->pid==pid) {
            cpu_utilization += (float)(p->execution_time) / (float)(p->deadline);
        }
    }
    if (cpu_utilization > 1.0) {
        return -22; // Return error code indicating Not Schedulable
    }
    return 0;
}

// Function to handle Rate-Monotonic Scheduling (RMS) policy
int 
handle_rms_policy(int pid, int count) {
    float total_resource = 0.0;
    struct proc *p;
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if (p->sched_policy == 1 || p->pid==pid) {
            total_resource += ((float)(p->execution_time) * (float)(p->rate));
        }
    }
    if (total_resource > (threshold_array[count - 1] * 100.0)) {
        return -22; // Return error code indicating resource limit exceeded
    }
    return 0;
}

int 
sys_sched_policy(void) {
    int pid, policy;
    if (argint(0, &pid) < 0 || argint(1, &policy) < 0) {
        return -1;
    }
	else{
    	struct proc *p;
    	acquire(&ptable.lock);

    	// Handle policy-specific checks
    	if (policy == 0) { // EDF
    	    int edf_check = handle_edf_policy(pid);
    	    if (edf_check != 0) {
				// killing the task from the kernel space by setting it's state to Zombie.
				for(p=ptable.proc;p<&ptable.proc[NPROC];p++){
					if(p->pid==pid){
						p->state=ZOMBIE;
						break;
					}
				}
    	    release(&ptable.lock);
    	    return edf_check;	// return -22
    	    }
    	} 
		else if (policy == 1) { // RMS
    	    int count = 0;
    	    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    	        if (p->sched_policy == 1 || p->pid == pid) {
    	            count++;
    	        }
    	    }
    	    int rms_check = handle_rms_policy(pid, count);
    	    if (rms_check != 0) {
				for(p=ptable.proc;p<&ptable.proc[NPROC];p++){
					if(p->pid==pid){
						p->state=ZOMBIE;
						break;
					}
				}
    	    release(&ptable.lock);
    	    return rms_check;
    	    }
    	}

    	for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
    	    if (p->pid == pid) {
    	        set_arrival_time(&(p->arrival_time));
    	        p->sched_policy = policy;
    	        if (policy == 0) {
    	            edf_count++;
    	        } 
				else if (policy == 1) {
    	            rms_count++;
    	        }
    	        break;
    	    }
    	}
    	release(&ptable.lock);
    	return 0;
	}
}

// sys_exec_time()
int 
sys_exec_time(void) {
    int pid, exec_time;
    if(argint(0, &pid) < 0 || argint(1, &exec_time) < 0){
        return -1;
	}
	else{
		struct proc *p;
    	acquire(&ptable.lock);
    	for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        	if (p->pid == pid) {
            	p->execution_time = exec_time;
            	release(&ptable.lock);
            	return 0;
        	}
    	}
    release(&ptable.lock);
 	return -1;
	}  
}

// sys_deadline()
int 
sys_deadline(void){
    int pid, deadline;
    if(argint(0, &pid) < 0 || argint(1, &deadline) < 0){
        return -1;
	}
	else{
    	struct proc *p;
    	acquire(&ptable.lock);
    	for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    	    if(p->pid == pid){
    	        p->deadline = deadline;
    	        release(&ptable.lock);
    	        return 0;
    	    }
    	}
    release(&ptable.lock);
    return -1;	
	}
}

// sys_rate()
int
sys_rate(void){
	int pid,rate;
	if(argint(0, &pid) < 0 || argint(1, &rate) < 0){
        return -1;
	}
    else{
		struct proc *p;
		acquire(&ptable.lock);
		for(p=ptable.proc;p<&ptable.proc[NPROC];p++){
			if(p->pid==pid){
				p->rate=rate;
				p->priority=max_value(1,ceil_value(((30.0-rate)/29.0)*3.0));
				release(&ptable.lock);
            	return 0;
			}
		}
	release(&ptable.lock);
	return -1;
	}
}
