/*
** echoc.c -- the echo client for echos.c; demonstrates unix sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <time.h>

#define SOCK_PATH "echo_socket"
#define WINDOW_SIZE 5

int main(void)
{
    int i, s, t, len;
    int window_complete = 0;
    struct sockaddr_un remote;
    int msg_pkt[100];
    char msg_send[100];
    int received_ack = 0;
    char str[100];
    int failures = 0, success = 0;
    int flag = 0;
    for(i = 0; i < 100; i++)
    {
        msg_pkt[i] = i;
    }

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    printf("Trying to connect...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(s, (struct sockaddr *)&remote, len) == -1) {
        perror("connect");
        exit(1);
    }

    printf("Connected.\n");
    size_t bk= sizeof(int);
    i = 0;
    printf("Before loop");
    while(i<100) {
            snprintf(msg_send, 100, "%d", msg_pkt[i]);
            printf("msg_send = %s", msg_send);
             
            if (send(s, msg_send,bk, 0) == -1) {
             perror("send");
             exit(1);
            }
            window_complete++;
            printf("window_complete=%d\n", window_complete);
            i++;
            sleep(1);
        /* Receiving the ACK signal can be initiated 
         * only when we have completed sending our
         * entire window of packets */ 
        if(window_complete%WINDOW_SIZE==0)
        {
          t = recv(s, str, 100, 0);
          received_ack = atoi(str);
          printf("received_ack=%d\n", received_ack);
          if (t > 0) {
              failures++;
              i = received_ack + 1;
              printf("echo> msg_pkt[%d] ACK received:\n", received_ack);
              window_complete = 0;
          } 
        }
        memset(msg_send, 100, 0);
        memset(str, 100, 0);
    }

    printf("RESULTS:\n");
    printf("Success rate = %d", success);
    printf("Failure rate = %d", failures);
    close(s);
    return 0;
}



