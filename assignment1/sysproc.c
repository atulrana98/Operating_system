#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
extern int trace;
extern int system_call_count[30];
extern void list_process(void);
extern int MSGSIZE;

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

int
sys_print_count(void)
{
 /* if(system_call_count[24]>0){
cprintf("sys_add %d\n",system_call_count[24]);
}*/
if(system_call_count[9]>0){
cprintf("sys_chdir %d\n",system_call_count[9]);
}
if(system_call_count[21]>0){
cprintf("sys_close %d\n",system_call_count[21]);
}

if(system_call_count[10]>0){
cprintf("sys_dup %d\n",system_call_count[10]);
}
if(system_call_count[7]>0){
cprintf("sys_exec %d\n",system_call_count[7]);
}
if(system_call_count[2]>0){
cprintf("sys_exit %d\n",system_call_count[2]);
}
if(system_call_count[1]>0){
cprintf("sys_fork %d\n",system_call_count[1]);
}

if(system_call_count[8]>0){
cprintf("sys_fstat %d\n",system_call_count[8]);
}
if(system_call_count[11]>0){
cprintf("sys_getpid %d\n",system_call_count[11]);
}
if(system_call_count[6]>0){
cprintf("sys_kill %d\n",system_call_count[6]);
}
if(system_call_count[19]>0){
cprintf("sys_link %d\n",system_call_count[19]);
}
if(system_call_count[20]>0){
cprintf("sys_mkdir %d\n",system_call_count[20]);
}
if(system_call_count[17]>0){
  cprintf("sys_mknod %d\n",system_call_count[17]);
}
if(system_call_count[15]>0){
cprintf("sys_open %d\n",system_call_count[15]);
}
// if(system_call_count[22]>0){
// cprintf("sys_print_count %d\n",system_call_count[22]);
// }
if(system_call_count[4]>0){
cprintf("sys_pipe %d\n",system_call_count[4]);
}
/*if(system_call_count[25]>0){
cprintf("sys_ps %d\n",system_call_count[25]);
}*/
if(system_call_count[5]>0){
cprintf("sys_read %d\n",system_call_count[5]);
}
/*if(system_call_count[27]>0){
cprintf("sys_recv %d\n",system_call_count[27]);
}*/
if(system_call_count[12]>0){
cprintf("sys_sbrk %d\n",system_call_count[12]);
}
/*if(system_call_count[26]>0){
cprintf("sys_send %d\n",system_call_count[26]);
}
if(system_call_count[28]>0){
cprintf("sys_send_multi %d\n",system_call_count[28]);
}*/
if(system_call_count[13]>0){
cprintf("sys_sleep %d\n",system_call_count[13]);
}
// if(system_call_count[23]>0){
// cprintf("sys_toggle %d\n",system_call_count[23]);
// }
if(system_call_count[18]>0){
cprintf("sys_unlink %d\n",system_call_count[18]);
}
if(system_call_count[14]>0){
cprintf("sys_uptime %d\n",system_call_count[14]);
}
if(system_call_count[3]>0){
cprintf("sys_wait %d\n",system_call_count[3]);
}
if(system_call_count[16]>0){
cprintf("sys_write %d\n",system_call_count[16]);
}
return 0;

}

int
sys_toggle(void)
{
  if(trace==0){
trace=1;
}
else{
trace=0;
for(int i=0;i<30;i++){
system_call_count[i]=0;
}
}
return 0;

}

int 
sys_add(void)
{
int a,b;
if(argint(0,&a)<0)
{
return -1;
}
if(argint(1,&b)<0)
{
return -1;
}
return a+b;
}

int 
sys_ps(void)
{
list_process();
return 0;
}

int sys_send(void) {
    int sender_id, receiver_id;
    char *message;

    if (argint(0, &sender_id) < 0 || argint(1, &receiver_id) < 0 || argptr(2, (void *)&message, sizeof(char *)) < 0)
        return -1;

    return send(sender_id, receiver_id, message);
}

int sys_recv(void) {
    char *message;

    if (argptr(0, (void *)&message, sizeof(char *)) < 0)
        return -1;

    return recv(message);
}

int sys_send_multi(void) {
    int sender_id;
    int *receiver_ids;
    char *message;

    if (argint(0, &sender_id) < 0 || argptr(1, (void *)&receiver_ids, sizeof(int *)) < 0 || argstr(2, (char **)&message) < 0)
        return -1;

    int length = 8; // Assuming you always have 8 child processes
    receiver_ids = (int *)receiver_ids;

    for (int i = 0; i < length; i++) {
        int result = send(sender_id, receiver_ids[i], message);
        if (result != 0) {
            // Handle the error, you might want to print an error message
            return result;
        }
    }

    return 0; // All messages sent successfully
}

