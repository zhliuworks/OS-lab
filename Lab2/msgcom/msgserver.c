/*
 *  msgserver.c  --  message communication server end source file
 */

#include "msgcom.h"

void main() {
    msgbuf buf;
    int qid;  // msgqueue id
    char outfile[] = "../test/test.msg.out";  // output message
    FILE *out = fopen(outfile, "w");
    // create message queue
    if ((qid = msgget(MSGCOM_KEY, IPC_CREAT|0666)) == -1) {
        printf("Message queue with specified key has already existed\n");
        exit(0);
    }

    while (1) {
        // receive the message from the client process
        msgrcv(qid, &buf, BUFFER_SIZE, MSGTYPE, MSG_NOERROR);
        printf("Received message from client process:\n%s\n", buf.msgtext);
        // write to the file
        fwrite(buf.msgtext, sizeof(char), buf.msglen, out);
        buf.msgtype = buf.sendpid;
        // send the message to the client process
        msgsnd(qid, &buf, sizeof(buf.msgtext), 0);
        if (buf.msglen < BUFFER_SIZE) break;
    }
    
    fclose(out);
    exit(0);
}