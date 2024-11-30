#include "types.h"
#include "stat.h"
#include "user.h"
#include "syscall.h"

#define MSGSIZE 8

int main(void) {
    toggle();
    printf(1, "%s\n", "IPC Test case");

    int childs[3];
    int parentid = getpid();
    int cid = fork();

    int cid1 = 0, cid2 = 0;
    int i = 0;

    if (cid != 0) {
        cid1 = fork();
        childs[i++] = cid;
    }
    if (cid != 0 && cid1 != 0) {
        cid2 = fork();
        childs[i++] = cid1;
    }

    if (cid2 != 0 && cid1 != 0 && cid != 0) {
        childs[i++] = cid2;

        for (int i = 0; i < 3; i++)
            printf(1, "%d ", childs[i]);

        char *msg_child = "Passmsg1";
        printf(1, "\nPARENT: msg sent to all childs is: %s\n", msg_child);
        int result = send_multi(parentid, childs, msg_child);

        if (result == 0) {
            printf(1, "PARENT: send_multi succeeded\n");
        } else {
            printf(1, "PARENT: send_multi failed with error code %d\n", result);
        }
		int i;
		for(i=0;i<3;i++){
			wait();
		}

       
        exit();
    } else {
        sleep(3);
        char msg[MSGSIZE];
        int result = receive(msg);

        if (result == 0) {
            printf(1, "CHILD(%d): msg received is: %s\n", getpid(), msg);
        } else {
            printf(1, "CHILD(%d): receive failed with error code %d\n", getpid(), result);
        }
        sleep(1);
        exit();
    }

    wait();
    exit();
}
