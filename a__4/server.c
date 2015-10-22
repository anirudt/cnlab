/*
 * Go-BackN server implementation
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define SOCK_PATH "echo_socket"
#define WINDOW_SIZE 5

int random_gen(int i)
{
	srand(time(NULL));
  int n;
	n = (i+random()%5);
  return n;
}

int main(void)
{
	int s, s2, t, len, pkt_no, i, last_correct, window_flag=0;
  int pkts[WINDOW_SIZE];
  int last_nack = 10;
	struct sockaddr_un local, remote;
	char str_ack[100];
	char rcv_str[100];
	int msg_pkt, msg_ack[100], msg_pkt_id;
  int count_nacks = 0;

  for (i = 0; i < WINDOW_SIZE; i++) {
    pkts[i] = 0; /* Nack inited */
  }
  /* Creation of socket */
	if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
  printf("Go Back-N Protocol");
	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);

  /* Binding the socket to this */
	if (bind(s, (struct sockaddr *)&local, len) == -1) {
		perror("bind");
		exit(1);
	}

  /* And, listening to the socket */
	if (listen(s, 5) == -1) {
		perror("listen");
		exit(1);
	}
    
	for(;;) {
		int done, n, status;
		printf("Waiting for a connection...\n");
		t = sizeof(remote);
		if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
			perror("accept");
			exit(1);
		}

		printf("Connected.\n");
		done = 0;
		pkt_no = 10;
    int temp, j;
    temp = random_gen(last_nack)+1;
		do {
      count_nacks = 0;
      for (i = last_nack; i < last_nack+5; i++) {
        printf("%d\n",i);
        n = recv(s2, rcv_str, 100, 0);
        msg_pkt_id = atoi(rcv_str);
        printf("pkt = %d\n",msg_pkt_id);
        pkts[i-last_nack] = msg_pkt_id;
        if (msg_pkt_id != temp+1) {
          printf("Server> Packet received for %d, sending Ack\n", msg_pkt_id);
        }
        else {
          printf("Server> Packet not received for %d, sending Nack after timeout\n", msg_pkt_id);
          count_nacks++;
        }
      }
      for (i = last_nack; i < last_nack+5; i++) {
        if (pkts[i-last_nack] == temp+1) 
          snprintf(str_ack, 100, "%d", 0+pkts[i-last_nack]*10);
        else 
          snprintf(str_ack, 100, "%d", 1+pkts[i-last_nack]*10);
        if (send(s2, str_ack, n, 0)<0) {
          perror("send");
          done = 0;
        }
        sleep(1);
      }
      for (j = 0; j < WINDOW_SIZE-1; j++) {
			  n = recv(s2, rcv_str, 100, 0);
        msg_pkt_id = atoi(rcv_str);
        printf("Server> Packet received for %d\n", msg_pkt_id);
      }
			
      memset(rcv_str, 100, 0);   
      memset(str_ack, 100, 0);
		} while (!done && pkt_no < 100);
        
		close(s2);
	}

	return 0;
}
