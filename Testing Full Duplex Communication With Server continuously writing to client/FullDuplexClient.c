#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define _POSIX_SOURCE 1
#include <signal.h>
#include <sys/wait.h>
int main()
{
	int sockid;
	int connectid;
	struct sockaddr_in servaddr;
	struct sockaddr_in client;
	int newsockid;
	int clientlen;
        int n;

	char msg[1000];
	int sendid, recvid;
	int port_id = 6000;
        


	sockid = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*)&servaddr, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(port_id);
	connectid = connect(sockid, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in));

	if(connectid < 0)
		printf("error \n");
		
        int fl=0;
        int c = 0;
        pid_t pid;
        //while(fl==0){
		if((pid = fork())==0 && fl==0){
			while(1){
				printf("\n write to the server:");
				n=0;
				while((msg[n++]=getchar())!='\n');
				sendto(sockid,msg,sizeof(msg),0,(struct sockaddr*)&servaddr, sizeof(struct sockaddr_in));
				

				if(strncmp("bye",msg,3)==0)
				 {
				   printf("Exit session...\n");
				   fl=1;
				   kill(pid, SIGKILL);
				   return 0;
				 }
				 bzero(msg,sizeof(msg));
				 //break;
			}
			
		}
		//if(fl==1){break;}
		if(pid!=0 && fl==0){
			while(1){
				recvid = recvfrom(sockid, msg, sizeof(msg), 0, (struct sockaddr*)&servaddr, &clientlen);
				printf("\n%s", msg);
				printf("\n write to the server:");
				if(strncmp("bye",msg,3)==0)
				 {
				   printf("Exit session...\n");
				   fl=1;
				   break;
				 }
				 bzero(msg,sizeof(msg));
				 //break;
			}
		}
		//if(fl==1){break;}
	//}
	return 0;
}
