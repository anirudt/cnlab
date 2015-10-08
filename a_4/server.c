/*
** echos.c -- the echo server for echoc.c; demonstrates unix sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "echo_socket"
#define WINDOW_SIZE 5

int random_gen()
{
  double n;
	srand(time(NULL));
	n = (random()%RAND_MAX+1)/(1.0*RAND_MAX);
  if(n<=0.7)
    return 1;
  else 
    return 0;
}

int main(void)
{
	int s, s2, t, len, pkt_no, i;
	struct sockaddr_un local, remote;
	char str_ack[100];
	char rcv_str[100];
	int msg_pkt;

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
		pkt_no = 0;
		do {
			n = recv(s2, rcv_str, 100, 0);
			
			msg_pkt = atoi(rcv_str);
      
      printf("msg_pkt: %d\n", msg_pkt);
			if(msg_pkt == pkt_no && n>=0)
			{
          status = random_gen();
			    if(status == 1 || msg_pkt==0)
          {
            snprintf(str_ack, 100, "%d", msg_pkt);
            if(send(s2, str_ack, n, 0)<0)
			      {
			          perror("send");
			          done = 1;
			      }
            pkt_no++;
            
          }
          else
          {
            msg_pkt;
            printf("Failing msg_pkt[%d]:\n", msg_pkt);
            snprintf(str_ack, 100, "%d", msg_pkt);
            if(send(s2, str_ack, n, 0)<0)
			      {
			          perror("send");
			          done = 1;
            }
          }
			    continue;
			} 
			if (n <= 0) {
				if (n < 0) perror("recv");
				done = 1;
			}
        memset(rcv_str, 100, 0);   
		} while (!done && pkt_no < 100);
        
		close(s2);
	}

	return 0;
}
