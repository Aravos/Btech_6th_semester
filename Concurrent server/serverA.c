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
   
 /* Name the socket */

   server_address.sun_family = AF_UNIX;
   strcpy(server_address.sun_path, "server_socket");
   server_len = sizeof(server_address);
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
        	read(client_sockfd, &ch, sizeof(ch));
        	if (strcmp("hello",ch)==0){
        		printf("%s","hello\n");
        		strcpy(ch , "ACK");
        		write(client_sockfd, ch, 3);
        		printf("ACK sent\n");
        	}
		else{
			printf("%s","NOT");
	      		write(client_sockfd, "NO", 3);
	      	}
	      	close(client_sockfd);
	      	exit(0);
      	}
  	else {
  		close(client_sockfd);
  	}
  }
}

