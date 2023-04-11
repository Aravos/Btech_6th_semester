/*Include necessary headers*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include  <sys/un.h>
#include  <unistd.h>

int main()
{
   int server_sockfd, client_sockfd;
   int server_len, client_len;
   struct sockaddr_un server_address;
   struct sockaddr_un client_address;

/* Remove any old sockets and create an unnamed socket for the server*/

   unlink("server_socket");
   server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
   

   server_address.sun_family = AF_UNIX;
   strcpy(server_address.sun_path, "server_socket");
   server_len = sizeof(server_address);
   /* Name the socket */
   bind(server_sockfd, (struct sockaddr *)&server_address, server_len);


  /*creating a connection queue and waiting for clients*/
   listen(server_sockfd, 5);
   
   signal(SIGCHLD, SIG_IGN);
   while(1) {
        char ch[5];
        printf("server waiting\n");
     
  /*Accepting connections*/

   client_len = sizeof(client_address);
   client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);

  if(fork() == 0) {
  	while(1){
	      int c[5] = {0,0,0,0,0};
	      char* str_1 = "hello";
	      for(int i=0;i<5;i++){
	      	if (c[i]==0){
	      		read(client_sockfd, &ch, 5);
	      		if (strcmp(str_1,ch)==0){
	      			printf("\n%s","hello");
	      			write(client_sockfd, "ACK", 3);
	      			printf("\nsent ACK");
	      			c[i]=1;
	      		}else{
	      			i--;
	      		}
	      	}
	      }
	      char a[3];
	      read(client_sockfd, &a, 3);
	      printf("\n%s Recived\n\n",a);
      }
      close(client_sockfd);
      exit(0);
    }
  else {
      close(client_sockfd);
    }
  }
}

