#include "types.h"
#include "stat.h"
#include "user.h"

#define MSGSIZE 8

float variance;

void print_variance(float xx) {
    int beg = (int)(xx);
    int fin = (int)(xx * 100) - beg * 100;
    printf(1, "Variance of array for the file arr is %d.%d\n", beg, fin);
}


struct sumData {
    int partialSum;
    int childId;
};

struct MeanData {
    float mean;
    int childId;
};

struct VarData {
    float var;
    int childId;
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf(1, "Need type and input filename\n");
        exit();
    }

    char *filename = argv[2];
    int type = atoi(argv[1]);
    printf(1, "Type is %d and filename is %s\n", type, filename);

    int tot_sum = 0;

    int size = 1000;
    short arr[size];

    char c;
    int fd = open(filename, 0);
	for(int i=0; i<size; i++){
		read(fd, &c, 1);
		arr[i]=c-'0';
		read(fd, &c, 1);
	}	
  	close(fd);

    printf(1, "First element: %d\n", arr[0]);

    int noOfProcesses = 8;
    int pidParent = getpid();
    int elmForEachProc = size / noOfProcesses;

    struct sumData procData[noOfProcesses];

    if (type == 0) {
        for (int i = 0; i < noOfProcesses; i++) {
            int cid = fork();
            if (cid == 0) {
                int curStart = i * elmForEachProc;
                int curEnd = (i + 1) * elmForEachProc;
                procData[i].partialSum = 0;
                for (int j = curStart; j < curEnd; j++) {
                    procData[i].partialSum += (int)arr[j];
                }

                procData[i].childId = getpid();
                send(getpid(), pidParent, (void *)&procData[i]);
                exit();
            } else {
                wait();
                struct sumData partialSumP;
                recv((void *)&partialSumP);
                tot_sum += partialSumP.partialSum;
            }
        }
    } else {
        struct sumData procData[noOfProcesses];

        int indexArr[noOfProcesses + 1];
        indexArr[0] = 0;
        indexArr[noOfProcesses] = size;
        int children[noOfProcesses];

        for (int i = 1; i < noOfProcesses; i++)
            indexArr[i] = indexArr[i - 1] + (size / noOfProcesses);

        for (int i = 0; i < noOfProcesses; i++) {
            int cid = fork();
            if (cid == 0) {
                int curStart = indexArr[i];
                int curEnd = indexArr[i + 1];
                procData[i].partialSum = 0;
                for (int j = curStart; j < curEnd; j++) {
                    procData[i].partialSum += (short)arr[j];
                }

                procData[i].childId = getpid();
                send(getpid(), pidParent, (void *)&procData[i]);
                sleep(100 * (i + 1));

                struct MeanData mean;
                recv((void *)&mean);

                float var = 0;
                for (int k = curStart; k < curEnd; k++) {
                    var += (arr[k] - mean.mean) * (arr[k] - mean.mean);
                }

                sleep(100 * (i + 1));

                struct VarData ret_var;
                ret_var.var = var;
                ret_var.childId = getpid();
                send(getpid(), pidParent, (void *)&ret_var);
                exit();
            } else {
                children[i] = cid;
            }
        }

        sleep(10);

        for (int i = 0; i < noOfProcesses; i++) {
            struct sumData partialSumP1;
            recv((void *)&partialSumP1);
            tot_sum += partialSumP1.partialSum;
        }

        float p_mean = (float)tot_sum / size;
        struct MeanData multi_msg;
        multi_msg.mean = p_mean;
        multi_msg.childId = children[0];

        send_multi(getpid(), children, (void *)&multi_msg);

        struct VarData get_var;
        int v_v = 0;

        for (int i = 0; i < noOfProcesses; i++) {
            wait();
            recv((void *)&get_var);
            v_v += get_var.var;
        }

        variance = v_v / (float)size;
    }

    if (type == 0) {
        printf(1, "Sum of array for file %s is %d\n", filename, tot_sum);
    } else {
        print_variance(variance);
    }

    exit();
}



