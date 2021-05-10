/*
 *  msgclient.c  --  message communication client end source file
 */

#include "msgcom.h"

void main() {
    msgbuf buf;
    int qid;  // msgqueue id
    pid_t pid;
    char buffer[BUFFER_SIZE];
    char infile[] = "../test/test.in";  // input message
    FILE *in = fopen(infile, "r");
    pid = getpid();
    // get existed msgqueue id after the server process has created the message queue
    qid = msgget(MSGCOM_KEY, IPC_CREAT|0666);
    int length;

    while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, in)) > 0) {
        // fill in the fields of message to be sent
        buf.msgtype = MSGTYPE;
        buf.msglen = length;
        buf.sendpid = pid;
        strcpy(buf.msgtext, buffer);
        // send the message to the server process
        msgsnd(qid, &buf, sizeof(buf.msgtext), 0);
        // receive the message from the server process
        msgrcv(qid, &buf, BUFFER_SIZE, pid, MSG_NOERROR);
        printf("Received message from server process: client pid %ld\n", buf.msgtype);
    }
    
    fclose(in);
    exit(0);
}