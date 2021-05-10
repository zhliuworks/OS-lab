/*
 *  msgcom.h  --  message communication header file
 */

#ifndef __MESSAGE_COMM__
#define __MESSAGE_COMM__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGCOM_KEY 123456
#define BUFFER_SIZE 1024
#define MSGTYPE 1

/* message buffer */
typedef struct {
    long msgtype;
    int msglen;
    pid_t sendpid;
    char msgtext[BUFFER_SIZE];
} msgbuf;

#endif