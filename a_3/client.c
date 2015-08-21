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

int main(void)
{
    int i, s, t, len;
    struct sockaddr_un remote;
    int msg_pkt[100];
    char msg_send[100];
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
        sleep(1);
        /* Receiving the ACK signal */ 
        if ((t=recv(s, str, 100, 0)) > 0) {
          if(str[0] == 'Y')
          {
            success++;
            printf("echo> msg_pkt[%d] ACK received:", i);
          }
          if(str[0] == 'N')
          {
            failures++;
            printf("echo> msg_pkt[%d] NACK received:", i);
          }
          i++;
        } else {
            if (t < 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
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



