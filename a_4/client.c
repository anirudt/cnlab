/*
 * Go-backN client implementation"
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
    int i, s, t, len, n;
    int window_complete = 0;
    struct sockaddr_un remote;
    int msg_pkt[100];
    char msg_send[100];
    int received_ack = 0;
    char str[100];
    int failures = 0, success = 0;
    int flag = 0;
    int msg_pkt_id, msg_pkt_ack;
    int last_nack = 0;
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
    i = 10;
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

            t = recv(s, str, 100, 0);
            if (t > 0) {
              received_ack = atoi(str);
              msg_pkt_id = received_ack/10;
              msg_pkt_ack = received_ack%10;
              printf("received_ack=%d\n", received_ack);
              if (msg_pkt_id >= last_nack + 5) {
                msg_pkt_ack = -1;
              }
              else {}
              if (msg_pkt_ack == 1) {
                /* Ack sent by the server */
                printf("echo> msg_pkt[%d] ACK received:\n",msg_pkt_id);
                snprintf(msg_send, 100, "%d", msg_pkt_id+5);
                if (send(s, msg_send, bk, 0) == -1) {
                  perror("send");
                  exit(1);
                }
              }
              else if(msg_pkt_ack == 0) {
                /* Nack sent by the server */
                printf("echo> msg_pkt[%d] NACK received:\n",msg_pkt_id);
                i = msg_pkt_id + 1;
                window_complete = 0;
                last_nack = msg_pkt_id;
              }
            } 
            window_complete = window_complete%5;
            memset(msg_send, 100, 0);
            memset(str, 100, 0);
    }

    close(s);
    return 0;
}



