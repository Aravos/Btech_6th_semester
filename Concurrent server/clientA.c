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
     printf("\n Client: Enter hello: ");
     scanf("%s",ch);

    /* You can now read and write via sockfd */

   /*  pid=fork();

     if(pid<0)
       {
          printf("\nfork failed");
          exit(1);
       }
     else if(pid==0)
       {
          printf("\n This is child process, enter a character");
          scanf("%c",&ch);
          //ch = 'A';
       }
     else
       {
          printf("\n I am parent process, enter a character");
          scanf("%c", &ch);
          //ch = 'D';         
       } */
   
      /* create a socket for the client*/

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

   
     write(sockfd, ch, strlen(ch));
     printf("Sent");
     char r[50];
      
     read(sockfd, &r, 3);
     printf("\nchar from server = %s\n", r);
     close(sockfd);
     exit(0);
}




     

    

