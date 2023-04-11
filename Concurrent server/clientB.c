  /* Set up variables and make necessary includes  */

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
     int pid;
     char ch[5];
     int sockfd;
     int len;
     struct sockaddr_un address;
     int result;
    /* You can now read and write via sockfd */

     sockfd = socket(AF_UNIX,SOCK_STREAM,0);
  
     /* Name the socket as agreed with the server */

     address.sun_family=AF_UNIX;
     strcpy(address.sun_path,"server_socket");
     len = sizeof(address);

     /*Connect your socket to the server’s socket*/

     result = connect(sockfd, (struct sockaddr *)&address, len);
     if(result == -1) {
        perror("oops: client1");
        exit(1);
     }
     while(1){
	     int count = 0;
	     while(count!=5){
		     write(sockfd, "hello", strlen("hello"));
		     printf("Sent\n");
		     count++;
	     }
	     printf("BLOCKED\n");
	     int c[5] = {0,0,0,0,0};
	      char* str_1 = "hello";
	      for(int i=0;i<5;i++){
	      	if (c[i]==0){
	      		read(sockfd, &ch, 3);
	      		if (strcmp("ACK",ch)==0){
	      			printf("\n ACK Recived \n");
	      			c[i]=1;
	      		}else{
	      			i--;
	      		}
	      	}
	      }
	     
	     write(sockfd, "ACK",3);
     }
     close(sockfd);
     exit(0);
}




     

    

