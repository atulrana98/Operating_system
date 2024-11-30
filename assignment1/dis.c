// // // #include "types.h"
// // // #include "stat.h"
// // // #include "user.h"

// // // #define MSGSIZE 4  // Assuming each partial sum is an integer

// // // struct buffer_struct {
// // //     int buffer;
// // //     int bufferstatus;
// // //     struct spinlock bufferlock;
// // // };

// // // struct buffer_struct buffers[8];
// // // struct spinlock sleeplock;

// // // void
// // // unicast_sum(int myid, int *partial_sum) {
// // //     // Sum the elements in the array for each process
// // //     int sum = 0;
// // //     for (int i = myid * 125; i < (myid + 1) * 125; i++) {
// // //         sum += arr[i];
// // //     }

// // //     // Send the partial sum to the coordinator
// // //     acquire(&buffers[0].bufferlock);
// // //     buffers[0].buffer = sum;
// // //     buffers[0].bufferstatus = 1;
// // //     wakeup((void *)&buffers[0]);
// // //     release(&buffers[0].bufferlock);

// // //     // Wait for the final sum from the coordinator
// // //     int recv_sum;
// // //     acquire(&sleeplock);
// // //     sleep((void *)&buffers[myid], &sleeplock);
// // //     release(&sleeplock);

// // //     // Print the partial sum and the final sum
// // //     printf(1, "Processor %d: Partial Sum = %d, Final Sum = %d\n", myid, sum, recv_sum);

// // //     // Update the pointer to the partial sum
// // //     *partial_sum = sum;
// // // }

// // // void
// // // multicast_variance(int myid, int partial_sum) {
// // //     int mean;

// // //     // Coordinator computes the mean
// // //     if (myid == 0) {
// // //         int total_sum = 0;
// // //         for (int i = 0; i < 8; i++) {
// // //             acquire(&buffers[i].bufferlock);
// // //             while (buffers[i].bufferstatus == 0) {
// // //                 sleep((void *)&buffers[i], &buffers[i].bufferlock);
// // //             }
// // //             total_sum += buffers[i].buffer;
// // //             release(&buffers[i].bufferlock);
// // //         }
// // //         mean = total_sum / 8;

// // //         // Multicast the mean to all worker processes
// // //         for (int i = 1; i < 8; i++) {
// // //             acquire(&buffers[i].bufferlock);
// // //             buffers[i].buffer = mean;
// // //             buffers[i].bufferstatus = 1;
// // //             wakeup((void *)&buffers[i]);
// // //             release(&buffers[i].bufferlock);
// // //         }
// // //     } else {
// // //         // Worker processes wait for the mean
// // //         acquire(&buffers[myid].bufferlock);
// // //         while (buffers[myid].bufferstatus == 0) {
// // //             sleep((void *)&buffers[myid], &buffers[myid].bufferlock);
// // //         }
// // //         mean = buffers[myid].buffer;
// // //         release(&buffers[myid].bufferlock);
// // //     }

// // //     // Worker processes compute the sum of squares of differences about the mean
// // //     int diff_squared_sum = 0;
// // //     for (int i = myid * 125; i < (myid + 1) * 125; i++) {
// // //         int diff = arr[i] - mean;
// // //         diff_squared_sum += diff * diff;
// // //     }

// // //     // Send the partial sum of squares to the coordinator
// // //     acquire(&buffers[0].bufferlock);
// // //     buffers[0].buffer = diff_squared_sum;
// // //     buffers[0].bufferstatus = 1;
// // //     wakeup((void *)&buffers[0]);
// // //     release(&buffers[0].bufferlock);

// // //     // Coordinator waits for the partial sums of squares
// // //     if (myid == 0) {
// // //         int total_diff_squared_sum = 0;
// // //         for (int i = 0; i < 8; i++) {
// // //             acquire(&buffers[i].bufferlock);
// // //             while (buffers[i].bufferstatus == 0) {
// // //                 sleep((void *)&buffers[i], &buffers[i].bufferlock);
// // //             }
// // //             total_diff_squared_sum += buffers[i].buffer;
// // //             release(&buffers[i].bufferlock);
// // //         }

// // //         // Compute the variance
// // //         float variance = total_diff_squared_sum / 1000.0;

// // //         // Print the variance
// // //         printf(1, "Processor %d: Variance = %d\n", myid, (int)variance);
// // //     }
// // // }

// // // int
// // // main(int argc, char *argv[]) {
// // //     if (argc < 3) {
// // //         printf(1, "Need type and input filename\n");
// // //         exit();
// // //     }

// // //     char *filename;
// // //     filename = argv[2];
// // //     int type = atoi(argv[1]);
// // //     printf(1, "Type is %d and filename is %s\n", type, filename);

// // //     int tot_sum = 0;

// // //     int size = 1000;
// // //     short arr[size];
// // //     char c;
// // //     int fd = open(filename, 0);
// // //     for (int i = 0; i < size; i++) {
// // //         read(fd, &c, 1);
// // //         arr[i] = c - '0';
// // //         read(fd, &c, 1);
// // //     }
// // //     close(fd);

// // //     int partial_sum;
// // //     int myid = getpid() % 8;

// // //     if (type == 0) {
// // //         // Unicast version
// // //         unicast_sum(myid, &partial_sum);
// // //     } else {
// // //         // Multicast version
// // //         unicast_sum(myid, &partial_sum);
// // //         multicast_variance(myid, partial_sum);
// // //     }

// // //     exit();
// // // }

// // #include "types.h"
// // #include "stat.h"
// // #include "user.h"

// // #define MSGSIZE 8
// // float variance;

// // void print_variance(float xx) {
// //     int beg = (int)(xx);
// //     int fin = (int)(xx * 100) - beg * 100;
// //     printf(1, "Variance of array for the file arr is %d.%d", beg, fin);
// // }

// // int main(int argc, char *argv[]) {
// //     if (argc < 2) {
// //         printf(1, "Need type and input filename\n");
// //         exit();
// //     }
// //     char *filename;
// //     filename = argv[2];
// //     int type = atoi(argv[1]);
// //     printf(1, "Type is %d and filename is %s\n", type, filename);

// //     int tot_sum = 0;

// //     int size = 1000;
// //     short arr[size];
// //     char c;
// //     int fd = open(filename, 0);
// //     for (int i = 0; i < size; i++) {
// //         read(fd, &c, 1);
// //         arr[i] = c - '0';
// //         read(fd, &c, 1);
// //     }
// //     close(fd);

// //     int noOfProcesses = 8;
// //     int pidParent = getpid();
// //     int i, j, cid;
// //     int elmForEachProc = size / noOfProcesses;
// //     void *partialSumP = (void *)malloc(MSGSIZE);
// //     void *pmsg = (void *)malloc(MSGSIZE);
// //     int partialSum = 0;

// //     if (type == 0) {
// //         for (i = 0; i < noOfProcesses; i++) {
// //             cid = fork();
// //             if (cid == 0) {
// //                 int curStart = i * elmForEachProc;
// //                 int curEnd = (i + 1) * elmForEachProc;
// //                 partialSum = 0;
// //                 for (j = curStart; j < curEnd; j++) {
// //                     partialSum += (short)arr[j];
// //                 }
// //                 pmsg = (void *)(&partialSum);
// //                 send(getpid(), pidParent, (void *)pmsg);
// //                 exit();
// //             } else {
// //                 wait();
// //                 receive((void *)partialSumP);
// //                 tot_sum += *(short *)partialSumP;
// //             }
// //         }
// //     } else {
// //         int pidChildren[noOfProcesses];
// //         int indexArr[noOfProcesses + 1];
// //         indexArr[0] = 0;
// //         indexArr[noOfProcesses] = size;

// //         for (int i = 1; i < noOfProcesses; i++)
// //             indexArr[i] = indexArr[i - 1] + (size / noOfProcesses);

// //         for (i = 1; i < noOfProcesses; i++) {
// //             cid = fork();
// //             if (cid == 0) {
// //                 int curStart = indexArr[i - 1];
// //                 int curEnd = indexArr[i];
// //                 partialSum = 0;
// //                 for (j = curStart; j < curEnd; j++) {
// //                     partialSum += (short)arr[j];
// //                 }
// //                 pmsg = (void *)(&partialSum);
// //                 send(getpid(), pidParent, (void *)pmsg);
// //                 sleep(100); // Give time for the coordinator to receive
// //                 void *mean = (void *)malloc(MSGSIZE);
// //                 receive((void *)mean);
// //                 float var = 0;
// //                 for (int k = curStart; k < curEnd; k++) {
// //                     var += (arr[k] - *(float *)mean) * (arr[k] - *(float *)mean);
// //                 }
// //                 sleep(100); // Give time for the coordinator to finish receiving
// //                 void *ret_var = (void *)malloc(MSGSIZE);
// //                 ret_var = (void *)(&var);
// //                 send(getpid(), pidParent, (void *)ret_var);
// //                 exit();
// //             } else {
// //                 pidChildren[i - 1] = cid;
// //             }
// //         }

// //         for (i = 0; i < noOfProcesses - 1; i++) {
// //             receive((void *)partialSumP);
// //             tot_sum += *(short *)partialSumP;
// //         }

// //         float p_mean = (float)tot_sum / (float)size;
// //         void *multi_msg = (void *)malloc(MSGSIZE);
// //         multi_msg = (void *)(&p_mean);
// //         send_multi(getpid(), pidChildren, (void *)multi_msg);

// //         float v_v = 0;
// //         for (i = 0; i < noOfProcesses - 1; i++) {
// //             void *partial_var = (void *)malloc(MSGSIZE);
// //             receive((void *)partial_var);
// //             v_v += *(float *)partial_var;
// //         }
// //         variance = v_v / (float)size;
// //     }

// //     if (type == 0) {
// //         printf(1, "Sum of array for file %s is %d\n", filename, tot_sum);
// //     } else
// //         print_variance(variance);
// //     exit();
// // }


// // #include "types.h"
// // #include "stat.h"
// // #include "user.h"

// // #define MSGSIZE 8

// // float variance;

// // void print_variance(float xx)
// // {
// //     int beg = (int)(xx);
// //     int fin = (int)(xx * 100) - beg * 100;
// //     printf(1, "Variance of array for the file arr is %d.%d\n", beg, fin);
// // }

// // int main(int argc, char *argv[])
// // {
// //     if (argc < 3)
// //     {
// //         printf(1, "Usage: %s <type> <filename>\n", argv[0]);
// //         exit();
// //     }

// //     char *filename = argv[2];
// //     int type = atoi(argv[1]);
// //     printf(1, "Type is %d and filename is %s\n", type, filename);

// //     int tot_sum = 0;
// //     int size = 1000;
// //     short arr[size];
// //     char c;

// //     int fd = open(filename, 0);
// //     for (int i = 0; i < size; i++)
// //     {
// //         read(fd, &c, 1);
// //         arr[i] = c - '0';
// //         read(fd, &c, 1);
// //     }
// //     close(fd);

// //     // ---- Unicast and Multicast Code ----
// //     int noOfProcesses = 8;
// //     int pidParent = getpid();
// //     int i;
// //     void *mean_msg = 0; // Initialize to 0

// //     int indexArr[noOfProcesses + 1];
// //     indexArr[0] = 0;
// //     indexArr[noOfProcesses] = size;
// //     int children[noOfProcesses];

// //     for (int i = 1; i < noOfProcesses; i++)
// //         indexArr[i] = indexArr[i - 1] + (size / noOfProcesses);

// //     for (int i = 0; i < noOfProcesses; i++)
// //     {
// //         int cid = fork();

// //         if (cid == 0)
// //         {
// //             int curStart = indexArr[i];
// //             int curEnd = indexArr[i + 1];
// //             short partialSum = 0;

// //             for (int j = curStart; j < curEnd; j++)
// //             {
// //                 partialSum += (short)arr[j];
// //             }

// //             send(getpid(), pidParent, &partialSum);

// //             if (i == 0)
// //             {
// //                 sleep(100); // Sleep to ensure the coordinator receives all partial sums
// //                 receive(mean_msg);
// //                 float mean = *(float *)mean_msg;

// //                 for (int k = curStart; k < curEnd; k++)
// //                 {
// //                     variance += (arr[k] - mean) * (arr[k] - mean);
// //                 }

// //                 sleep(100); // Sleep to ensure all workers have completed
// //                 void *var_msg = (void *)&variance;
// //                 send(getpid(), pidParent, var_msg);
// //             }

// //             exit();
// //         }
// //         else
// //         {
// //             children[i] = cid;
// //         }
// //     }

// //     // Wait for all children to complete
// //     for (int i = 0; i < noOfProcesses; i++)
// //     {
// //         wait();
// //     }

// //     if (type == 0)
// //     {
// //         // Unicast sum
// //         for (int i = 0; i < noOfProcesses; i++)
// //         {
// //             void *partialSumP = (void *)malloc(MSGSIZE);
// //             receive(partialSumP);
// //             tot_sum += *(short *)partialSumP;
// //             free(partialSumP);
// //         }

// //         printf(1, "Sum of array for file %s is %d\n", filename, tot_sum);
// //     }
// //     else
// //     {
// //         // Multicast variance
// //         float mean = variance / (float)size;
// //         void *multi_msg = (void *)&mean;
// //         send_multi(getpid(), children, multi_msg);

// //         for (int i = 0; i < noOfProcesses; i++)
// //         {
// //             void *get_var = (void *)malloc(MSGSIZE);
// //             receive(get_var);
// //             variance += *(float *)get_var;
// //             free(get_var);
// //         }

// //         print_variance(variance);
// //     }

// //     exit();
// // }


// // #include "types.h"
// // #include "stat.h"
// // #include "user.h"

// // #define MSGSIZE 8

// // void print_variance(float xx) {
// //     int beg = (int)(xx);
// //     int fin = (int)(xx * 100) - beg * 100;
// //     printf(1, "Variance of array is %d.%d\n", beg, fin);
// // }

// // int main(int argc, char *argv[]) {
// //     if (argc < 3) {
// //         printf(1, "Usage: %s <type> <filename>\n", argv[0]);
// //         exit();
// //     }

// //     char *filename = argv[2];
// //     int type = atoi(argv[1]);
// //     printf(1, "Type is %d and filename is %s\n", type, filename);

// //     int size = 1000;
// //     short arr[size];
// //     char c;
// //     int fd = open(filename, 0);

// //     for (int i = 0; i < size; i++) {
// //         read(fd, &c, 1);
// //         arr[i] = c - '0';
// //         read(fd, &c, 1);
// //     }

// //     close(fd);

// //     int tot_sum = 0;
// //     float variance = 0.0;
// //     int noOfProcesses = 8;
// //     int pidParent = getpid();

// //     if (type == 0) { // Unicast sum
// //         int elmForEachProc = size / noOfProcesses;

// //         for (int i = 0; i < noOfProcesses; i++) {
// //             int cid = fork();
// //             if (cid == 0) {
// //                 int curStart = i * elmForEachProc;
// //                 int curEnd = (i + 1) * elmForEachProc;
// //                 short partialSum = 0;

// //                 for (int j = curStart; j < curEnd; j++) {
// //                     partialSum += (short)arr[j];
// //                 }

// //                 send(getpid(), pidParent, &partialSum);
// //                 exit();
// //             } else {
// //                 wait();
// //                 void *partialSumP = 0; // Initialize to 0
// //                 receive(partialSumP);
// //                 tot_sum += *(short *)partialSumP;
// //             }
// //         }
// //     } else { // Multicast variance
// //         int indexArr[noOfProcesses + 1];
// //         indexArr[0] = 0;
// //         indexArr[noOfProcesses] = size;

// //         for (int i = 1; i < noOfProcesses; i++)
// //             indexArr[i] = indexArr[i - 1] + (size / noOfProcesses);

// //         int children[noOfProcesses];

// //         for (int i = 0; i < noOfProcesses; i++) {
// //             int cid = fork();

// //             if (cid == 0) {
// //                 int curStart = indexArr[i];
// //                 int curEnd = indexArr[i + 1];
// //                 short partialSum = 0;

// //                 for (int j = curStart; j < curEnd; j++) {
// //                     partialSum += (short)arr[j];
// //                 }

// //                 send(getpid(), pidParent, &partialSum);

// //                 if (i == 0) {
// //                     sleep(100); // Sleep to ensure the coordinator receives all partial sums
// //                     void *mean_msg = 0; // Initialize to 0
// //                     receive(mean_msg);
// //                     float mean = *(float *)mean_msg;

// //                     for (int k = curStart; k < curEnd; k++) {
// //                         variance += (arr[k] - mean) * (arr[k] - mean);
// //                     }

// //                     sleep(100); // Sleep to ensure all workers have completed
// //                     void *var_msg = 0; // Initialize to 0
// //                     var_msg = (void *)&variance;
// //                     send(getpid(), pidParent, var_msg);
// //                 }

// //                 exit();
// //             } else {
// //                 children[i] = cid;
// //             }
// //         }

// //         for (int i = 0; i < noOfProcesses; i++) {
// //             wait();
// //             void *partialSumP = 0; // Initialize to 0
// //             receive(partialSumP);
// //             tot_sum += *(short *)partialSumP;
// //         }

// //         for (int i = 1; i < noOfProcesses; i++) {
// //             wait();
// //             void *mean_msg = 0; // Initialize to 0
// //             receive(mean_msg);
// //             float mean = *(float *)mean_msg;

// //             void *var_msg = 0; // Initialize to 0
// //             receive(var_msg);
// //             float var = *(float *)var_msg;

// //             variance += var;
// //         }
// //         variance /= size;
// //     }

// //     if (type == 0) {
// //         printf(1, "Sum of array for file %s is %d\n", filename, tot_sum);
// //     } else {
// //         print_variance(variance);
// //     }

// //     exit();
// // }


// // #include "types.h"
// // #include "stat.h"
// // #include "user.h"

// // #define MSGSIZE 8
// // float variance;

// // void print_variance(float xx) {
// //     int beg = (int)(xx);
// //     int fin = (int)(xx * 100) - beg * 100;
// //     printf(1, "Variance of array for the file arr is %d.%d", beg, fin);
// // }

// // int main(int argc, char *argv[]) {
// //     if (argc < 2) {
// //         printf(1, "Need type and input filename\n");
// //         exit();
// //     }
// //     char *filename;
// //     filename = argv[2];
// //     int type = atoi(argv[1]);
// //     printf(1, "Type is %d and filename is %s\n", type, filename);

// //     int tot_sum = 0;

// //     int size = 1000;
// //     short arr[size];
// //     char c;
// //     int fd = open(filename, 0);
// //     for (int i = 0; i < size; i++) {
// //         read(fd, &c, 1);
// //         arr[i] = c - '0';
// //         read(fd, &c, 1);
// //     }
// //     close(fd);

// //     int noOfProcesses = 8;
// //     int pidParent = getpid();
// //     int i, j, cid;
// //     int elmForEachProc = size / noOfProcesses;
// //     void *partialSumP = (void *)malloc(MSGSIZE);
// //     void *pmsg = (void *)malloc(MSGSIZE);
// //     int partialSum = 0;

// //     if (type == 0) {
// //         for (i = 0; i < noOfProcesses; i++) {
// //             cid = fork();
// //             if (cid == 0) {
// //                 int curStart = i * elmForEachProc;
// //                 int curEnd = (i + 1) * elmForEachProc;
// //                 partialSum = 0;
// //                 for (j = curStart; j < curEnd; j++) {
// //                     partialSum += (short)arr[j];
// //                 }
// //                 pmsg = (void *)(&partialSum);
// //                 send(getpid(), pidParent, (void *)pmsg);
// //                 exit();
// //             } else {
// //                 wait();
// //                 receive((void *)partialSumP);
// //                 tot_sum += *(short *)partialSumP;
// //             }
// //         }
// //     } else {
// //         int pidChildren[noOfProcesses];
// //         int indexArr[noOfProcesses + 1];
// //         indexArr[0] = 0;
// //         indexArr[noOfProcesses] = size;

// //         for (int i = 1; i < noOfProcesses; i++)
// //             indexArr[i] = indexArr[i - 1] + (size / noOfProcesses);

// //         for (i = 1; i < noOfProcesses; i++) {
// //             cid = fork();
// //             if (cid == 0) {
// //                 int curStart = indexArr[i - 1];
// //                 int curEnd = indexArr[i];
// //                 partialSum = 0;
// //                 for (j = curStart; j < curEnd; j++) {
// //                     partialSum += (short)arr[j];
// //                 }
// //                 pmsg = (void *)(&partialSum);
// //                 send(getpid(), pidParent, (void *)pmsg);
// //                 sleep(100); // Give time for the coordinator to receive
// //                 void *mean = (void *)malloc(MSGSIZE);
// //                 receive((void *)mean);
// //                 float var = 0;
// //                 for (int k = curStart; k < curEnd; k++) {
// //                     var += (arr[k] - *(float *)mean) * (arr[k] - *(float *)mean);
// //                 }
// //                 sleep(100); // Give time for the coordinator to finish receiving
// //                 void *ret_var = (void *)malloc(MSGSIZE);
// //                 ret_var = (void *)(&var);
// //                 send(getpid(), pidParent, (void *)ret_var);
// //                 exit();
// //             } else {
// //                 pidChildren[i - 1] = cid;
// //             }
// //         }

// //         for (i = 0; i < noOfProcesses - 1; i++) {
// //             receive((void *)partialSumP);
// //             tot_sum += *(short *)partialSumP;
// //         }

// //         float p_mean = (float)tot_sum / (float)size;
// //         void *multi_msg = (void *)malloc(MSGSIZE);
// //         multi_msg = (void *)(&p_mean);
// //         send_multi(getpid(), pidChildren, (void *)multi_msg);

// //         float v_v = 0;
// //         for (i = 0; i < noOfProcesses - 1; i++) {
// //             void *partial_var = (void *)malloc(MSGSIZE);
// //             receive((void *)partial_var);
// //             v_v += *(float *)partial_var;
// //         }
// //         variance = v_v / (float)size;
// //     }

// //     if (type == 0) {
// //         printf(1, "Sum of array for file %s is %d\n", filename, tot_sum);
// //     } else
// //         print_variance(variance);
// //     exit();
// // }


// // #include "types.h"
// // #include "stat.h"
// // #include "user.h"

// // #define MSGSIZE 8

// // float variance;

// // void print_variance(float xx)
// // {
// //     int beg = (int)(xx);
// //     int fin = (int)(xx * 100) - beg * 100;
// //     printf(1, "Variance of array for the file arr is %d.%d\n", beg, fin);
// // }

// // int main(int argc, char *argv[])
// // {
// //     if (argc < 3)
// //     {
// //         printf(1, "Usage: %s <type> <filename>\n", argv[0]);
// //         exit();
// //     }

// //     char *filename = argv[2];
// //     int type = atoi(argv[1]);
// //     printf(1, "Type is %d and filename is %s\n", type, filename);

// //     int tot_sum = 0;
// //     int size = 1000;
// //     short arr[size];
// //     char c;

// //     int fd = open(filename, 0);
// //     for (int i = 0; i < size; i++)
// //     {
// //         read(fd, &c, 1);
// //         arr[i] = c - '0';
// //         read(fd, &c, 1);
// //     }
// //     close(fd);

// //     // ---- Unicast and Multicast Code ----
// //     int noOfProcesses = 8;
// //     int pidParent = getpid();
// //     int i;
// //     void *mean_msg = 0; // Initialize to 0

// //     int indexArr[noOfProcesses + 1];
// //     indexArr[0] = 0;
// //     indexArr[noOfProcesses] = size;
// //     int children[noOfProcesses];

// //     for (int i = 1; i < noOfProcesses; i++)
// //         indexArr[i] = indexArr[i - 1] + (size / noOfProcesses);

// //     for (int i = 0; i < noOfProcesses; i++)
// //     {
// //         int cid = fork();

// //         if (cid == 0)
// //         {
// //             int curStart = indexArr[i];
// //             int curEnd = indexArr[i + 1];
// //             short partialSum = 0;

// //             for (int j = curStart; j < curEnd; j++)
// //             {
// //                 partialSum += (short)arr[j];
// //             }

// //             send(getpid(), pidParent, &partialSum);

// //             if (i == 0)
// //             {
// //                 sleep(100); // Sleep to ensure the coordinator receives all partial sums
// //                 receive(mean_msg);
// //                 float mean = *(float *)mean_msg;

// //                 for (int k = curStart; k < curEnd; k++)
// //                 {
// //                     variance += (arr[k] - mean) * (arr[k] - mean);
// //                 }

// //                 sleep(100); // Sleep to ensure all workers have completed
// //                 void *var_msg = (void *)&variance;
// //                 send(getpid(), pidParent, var_msg);
// //             }

// //             exit();
// //         }
// //         else
// //         {
// //             children[i] = cid;
// //         }
// //     }

// //     // Wait for all children to complete
// //     for (int i = 0; i < noOfProcesses; i++)
// //     {
// //         wait();
// //     }

// //     if (type == 0)
// //     {
// //         // Unicast sum
// //         for (int i = 0; i < noOfProcesses; i++)
// //         {
// //             void *partialSumP = (void *)malloc(MSGSIZE);
// //             receive(partialSumP);
// //             tot_sum += *(short *)partialSumP;
// //             free(partialSumP);
// //         }

// //         printf(1, "Sum of array for file %s is %d\n", filename, tot_sum);
// //     }
// //     else
// //     {
// //         // Multicast variance
// //         float mean = variance / (float)size;
// //         void *multi_msg = (void *)&mean;
// //         send_multi(getpid(), children, multi_msg);

// //         for (int i = 0; i < noOfProcesses; i++)
// //         {
// //             void *get_var = (void *)malloc(MSGSIZE);
// //             receive(get_var);
// //             variance += *(float *)get_var;
// //             free(get_var);
// //         }

// //         print_variance(variance);
// //     }

// //     exit();
// // }


// // #include "types.h"
// // #include "stat.h"
// // #include "user.h"

// // #define MSGSIZE 4

// // float variance;

// // void print_variance(float xx)
// // {
// //     int beg = (int)(xx);
// //     int fin = (int)(xx * 100) - beg * 100;
// //     printf(1, "Variance of array for the file arr is %d.%d\n", beg, fin);
// // }

// // int main(int argc, char *argv[])
// // {
// //     if (argc < 3)
// //     {
// //         printf(1, "Usage: %s <type> <filename>\n", argv[0]);
// //         exit();
// //     }

// //     char *filename = argv[2];
// //     int type = atoi(argv[1]);
// //     printf(1, "Type is %d and filename is %s\n", type, filename);

// //     int tot_sum = 0;
// //     int size = 1000;
// //     short arr[size];
// //     char c;

// //     int fd = open(filename, 0);
// //     for (int i = 0; i < size; i++)
// //     {
// //         read(fd, &c, 1);
// //         arr[i] = c - '0';
// //         read(fd, &c, 1);
// //     }
// //     close(fd);

// //     int noOfProcesses = 8;
// //     int pidParent = getpid();
// //     int indexArr[noOfProcesses + 1];
// //     indexArr[0] = 0;
// //     indexArr[noOfProcesses] = size;
// //     int children[noOfProcesses];

// //     for (int i = 1; i < noOfProcesses; i++)
// //         indexArr[i] = indexArr[i - 1] + (size / noOfProcesses);

// //     for (int i = 0; i < noOfProcesses; i++)
// //     {
// //         int cid = fork();

// //         if (cid == 0)
// //         {
// //             int curStart = indexArr[i];
// //             int curEnd = indexArr[i + 1];
// //             short partialSum = 0;

// //             for (int j = curStart; j < curEnd; j++)
// //             {
// //                 partialSum += (short)arr[j];
// //             }

// //             send(getpid(), pidParent, &partialSum);

// //             if (i == 0)
// //             {
// //                 sleep(100); // Sleep to ensure the coordinator receives all partial sums
// //                 float mean = (float)partialSum / (float)(curEnd - curStart);
// //                 void *mean_msg = (void *)&mean;
// //                 send(getpid(), pidParent, mean_msg);
// //                 sleep(100); // Sleep to ensure all workers have completed

// //                 float local_variance = 0;

// //                 for (int k = curStart; k < curEnd; k++)
// //                 {
// //                     local_variance += (arr[k] - mean) * (arr[k] - mean);
// //                 }

// //                 void *var_msg = (void *)&local_variance;
// //                 send(getpid(), pidParent, var_msg);
// //             }

// //             exit();
// //         }
// //         else
// //         {
// //             children[i] = cid;
// //         }
// //     }

// //     // Wait for all children to complete
// //     for (int i = 0; i < noOfProcesses; i++)
// //     {
// //         wait();
// //     }

// //     if (type == 0)
// //     {
// //         // Unicast sum
// //         for (int i = 0; i < noOfProcesses; i++)
// //         {
// //             void *partialSumP = (void *)malloc(MSGSIZE);
// //             receive(partialSumP);
// //             tot_sum += *(short *)partialSumP;
// //             free(partialSumP);
// //         }

// //         printf(1, "Sum of array for file %s is %d\n", filename, tot_sum);
// //     }
// //     else
// //     {
// //         // Multicast variance
// //         float mean;
// //         void *mean_msg = (void *)malloc(MSGSIZE);
// //         receive(mean_msg);
// //         mean = *(float *)mean_msg;
// //         free(mean_msg);

// //         for (int i = 0; i < noOfProcesses; i++)
// //         {
// //             void *get_var = (void *)malloc(MSGSIZE);
// //             receive(get_var);
// //             variance += *(float *)get_var;
// //             free(get_var);
// //         }

// //         print_variance(variance / (float)size);
// //     }

// //     exit();
// // }


// #include "types.h"
// #include "stat.h"
// #include "user.h"

// #define MSGSIZE 8

// float variance;

// void print_variance(float xx)
// {
//     int beg = (int)(xx);
//     int fin = (int)(xx * 100) - beg * 100;
//     printf(1, "Variance of array for the file arr is %d.%d\n", beg, fin);
// }

// int main(int argc, char *argv[])
// {
//     if (argc < 3)
//     {
//         printf(1, "Usage: %s <type> <filename>\n", argv[0]);
//         exit();
//     }

//     char *filename = argv[2];
//     int type = atoi(argv[1]);
//     printf(1, "Type is %d and filename is %s\n", type, filename);

//     int tot_sum = 0;
//     int size = 1000;
//     short arr[size];
//     char c;

//     int fd = open(filename, 0);
//     for (int i = 0; i < size; i++)
//     {
//         read(fd, &c, 1);
//         arr[i] = c - '0';
//         read(fd, &c, 1);
//     }
//     close(fd);

//     // ---- Unicast and Multicast Code ----
//     int noOfProcesses = 8;
//     int pidParent = getpid();
//     int i;
//     void *mean_msg = 0; // Initialize to 0

//     int indexArr[noOfProcesses + 1];
//     indexArr[0] = 0;
//     indexArr[noOfProcesses] = size;
//     int children[noOfProcesses];

//     for (int i = 1; i < noOfProcesses; i++)
//         indexArr[i] = indexArr[i - 1] + (size / noOfProcesses);

//     for (int i = 0; i < noOfProcesses; i++)
//     {
//         int cid = fork();

//         if (cid == 0)
//         {
//             int curStart = indexArr[i];
//             int curEnd = indexArr[i + 1];
//             short partialSum = 0;

//             for (int j = curStart; j < curEnd; j++)
//             {
//                 partialSum += (short)arr[j];
//             }

//             send(getpid(), pidParent, &partialSum);

//             if (i == 0)
//             {
//                 sleep(100); // Sleep to ensure the coordinator receives all partial sums
//                 receive(mean_msg);
//                 float mean = *(float *)mean_msg;

//                 for (int k = curStart; k < curEnd; k++)
//                 {
//                     variance += (arr[k] - mean) * (arr[k] - mean);
//                 }

//                 sleep(100); // Sleep to ensure all workers have completed
//                 void *var_msg = (void *)&variance;
//                 send(getpid(), pidParent, var_msg);
//             }

//             exit();
//         }
//         else
//         {
//             children[i] = cid;
//         }
//     }

//     // Wait for all children to complete
//     for (int i = 0; i < noOfProcesses; i++)
//     {
//         wait();
//     }

//     if (type == 0)
//     {
//         // Unicast sum
//         for (int i = 0; i < noOfProcesses; i++)
//         {
//             void *partialSumP = (void *)malloc(MSGSIZE);
//             receive(partialSumP);
//             tot_sum += *(short *)partialSumP;
//             free(partialSumP);
//         }

//         printf(1, "Sum of array for file %s is %d\n", filename, tot_sum);
//     }
//     else
//     {
//         // Multicast variance
//         float mean = variance / (float)size;
//         void *multi_msg = (void *)&mean;
//         send_multi(getpid(), children, multi_msg);

//         for (int i = 0; i < noOfProcesses; i++)
//         {
//             void *get_var = (void *)malloc(MSGSIZE);
//             receive(get_var);
//             variance += *(float *)get_var;
//             free(get_var);
//         }

//         print_variance(variance);
//     }

//     exit();
// }
