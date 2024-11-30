#include "types.h"
#include "stat.h"
#include "user.h"

// TC#1: EDF Schedulable test case
int main(int argc, char *argv[])
{   
    int num_procs = 3;
    int deadline_value[3] = {29, 34, 31};
    int exectime[3] = {5, 6, 4};

    int parent_pid = getpid();

    // This is for parent process which is this current program
    // Set the scheduling policy to EDF
    deadline(parent_pid, 25);
    exec_time(parent_pid, 10);
    sched_policy(parent_pid, 0);

    for(int i = 0; i < num_procs; i++)
    {
        int cid = fork();   // PID of child process
        if (cid != 0)
        {
            // This block is executed by the parent processs
            // Set the scheduling policy to EDF
            deadline(cid, deadline_value[i]);
            exec_time(cid, exectime[i]);
            sched_policy(cid, 0);
        }
        else
        {
            // This block is executed by the child process
            /*The XV6 kills the process if th exec time is completed*/
            while(1) {
                
            }
        }
    }

    // After creating all child processes, the parent process enters an infinite loop
    while(1) {

    }
}
