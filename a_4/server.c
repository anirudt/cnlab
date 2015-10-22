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
	struct sockaddr_un local, remote;
	char str_ack[100];
	char rcv_str[100];
	int msg_pkt, msg_ack[100], msg_pkt_id;

  for (i = 0; i < 100; i++) {
    msg_ack[i] = 0; /* Nack inited */
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
    last_correct = 10;
		done = 0;
		pkt_no = 10;
    int temp, j;
    temp = random_gen(last_correct)+1;
		do {
			n = recv(s2, rcv_str, 100, 0);
			
			msg_pkt = atoi(rcv_str);
      msg_pkt_id = msg_pkt;
      window_flag++;
      
      printf("msg_pkt: %d\n", msg_pkt_id);
      printf("window_flag= %d\n", window_flag);
      if(msg_pkt_id<=temp)
      {
        printf("Reached successfully\n");
        msg_ack[msg_pkt_id] = 1;
        snprintf(str_ack, 100, "%d", 1+msg_pkt_id*10);
        if(send(s2, str_ack, n, 0)<0) {
			    perror("send");
			    done = 0;
			  }
      }
      else if(msg_pkt_id==temp+1)
      {
        printf("Unsuccessful\n");
        snprintf(str_ack, 100, "%d", 0+msg_pkt_id*10);
        if(send(s2, str_ack, n, 0)<0) {
			    perror("send");
			    done = 0;
			  }
      }
      else if(msg_pkt_id >= last_correct + 5) {
        /* Tackling Acks from earlier ones*/
        printf("Passover ones?\n");
        msg_ack[msg_pkt_id] = 1;
        snprintf(str_ack, 100, "%d", 1+msg_pkt_id*10);
        if(send(s2, str_ack, n, 0)<0) {
			    perror("send");
			    done = 0;
			  }
      }
      else
      {
        int jadu = random();
        if(jadu%2==0) {
          printf("Reached Successfully\n");
          snprintf(str_ack, 100, "%d", 1+msg_pkt_id*10);
          if(send(s2, str_ack, n, 0)<0) {
			      perror("send");
			      done = 0;
			    }
          msg_ack[msg_pkt_id] = 1;
        }
        else {
          printf("Reached unsuccessfully\n");
          snprintf(str_ack, 100, "%d", 0+msg_pkt_id*10);
          if(send(s2, str_ack, n, 0)<0) {
			      perror("send");
			      done = 0;
			    }
          msg_ack[msg_pkt_id] = 0;
        }
      }

			if(msg_pkt_id == pkt_no && n>=0)
			{
        if(window_flag==WINDOW_SIZE) 
        {
          last_correct = temp;
          printf("Sending last correct as packet[%d]\n", last_correct);
          snprintf(str_ack, 100, "%d", last_correct);
          if(send(s2, str_ack, n, 0)<0)
			    {
			      perror("send");
			      done = 0;
			    }
          window_flag = 0;
          pkt_no = last_correct+1;
          temp = random_gen(last_correct)+1;
        }
        else
        {
          pkt_no++;
          //continue;
        }
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
