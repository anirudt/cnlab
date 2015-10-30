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
#include <unistd.h>

#define SOCK_PATH "echo_socket"
#define WINDOW_SIZE 5


int main(void)
{
  int i, s, t, len, n, j;
  int fl = 0;
  int window_complete = 0;
  struct sockaddr_un remote;
  int msg_pkt[100];
  char msg_send[100];
  int received_ack = 0;
  char str[100];
  int failures = 0, success = 0;
  int flag = 0;
  int msg_pkt_id, msg_pkt_ack;
  int last_nack = 10;
  int nack_id=0, acks=0;
  int pkts[WINDOW_SIZE];
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
    acks = 0;
    fl = 0;
    for (i = last_nack; i < last_nack+5; i++) {
      snprintf(msg_send, 100, "%d", msg_pkt[i]);
      printf("msg_send = %s", msg_send);
      sleep(1);

      if (send(s, msg_send,bk, 0) == -1) {
        perror("send");
      }
      printf("message has been sent\n");
    }
    for (j = 0; j < 5; j++) {
      t = recv(s, str, 100, 0);
      if (t>0) {
        received_ack = atoi(str);
        msg_pkt_id = received_ack/10;
        msg_pkt_ack = received_ack%10;
        if (msg_pkt_ack == 1 && msg_pkt_id < 95 && fl==0) {
          printf("client> Received Ack from %d\n",msg_pkt_id);
          pkts[acks] = msg_pkt_id;
          acks++;
        }
        if (msg_pkt_ack == 0) {
          if (fl == 0) {
            nack_id = msg_pkt_id;
          }
          fl++;
          printf("client> Received Nack from %d\n",msg_pkt_id);
        }
      }
    }
    for (j = 0; j < acks; j++) {
      snprintf(msg_send, 100, "%d", pkts[j]+5);
      printf("Sending ACKed packets %d \n", pkts[j]+5);
      if (send(s, msg_send, bk, 0)<0) {
        perror("send");
      }
      pkts[j] = 0;
      sleep(1);
    }
      
    if (acks==5) {
      last_nack = last_nack + 5;
      nack_id = last_nack;
    }
    else {
      last_nack = nack_id;
    }
    printf("window_complete=%d\n", window_complete);
    //sleep(1);
    memset(msg_send, 100, 0);
    memset(str, 100, 0);
  }
  close(s);
  return 0;
}
