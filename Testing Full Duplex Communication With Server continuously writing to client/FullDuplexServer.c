#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define _POSIX_SOURCE 1
#include <signal.h>
#include <sys/wait.h>
int main(){
	int sockid;
	int bindid;
	struct sockaddr_in myaddr;
	struct sockaddr_in client;
	int newsockid;
	int clientlen;
        int n;
        char msg[1000];
	int recvid, sendid;
	int port_id = 6000;


	sockid = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char*)&myaddr, sizeof(struct sockaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(port_id);
	bindid = bind(sockid, (struct sockaddr*)&myaddr, sizeof(struct sockaddr_in));

	if(bindid < 0)
		printf("error \n");

	listen(sockid, 5);

        clientlen = sizeof(struct sockaddr_in);
        newsockid = accept(sockid, (struct sockaddr*)&client, &clientlen);
		if(newsockid < 0)
			printf("error 2\n");
	printf("sleeping for 2");
	sleep(2);
	int fl = 0;
	int c = 0;
	pid_t pid;
	//while(fl==0){
		if((pid = fork())==0 && fl==0){
			while(1){
				recvid = recvfrom(newsockid, msg, sizeof(msg), 0, (struct sockaddr*)&client, &clientlen);
				if(recvid < 0)
					printf("error 2\n");

				printf("\n%s \n", msg);

				if(strncmp("bye",msg,3)==0)
				 {
				   printf("Exit session...\n");
				   fl=1;
				   kill(pid, SIGKILL);
				   break;
				 }
				 bzero(msg,1000);
				 //break;
			}
			
		}
		//if(fl==1){break;}
		if(pid!=0 && fl==0){
			while(1){
				//printf("\n write to the client:");
				n=0;
				sleep(0.5);
				sendto(newsockid,"hello client",sizeof("hello client"),0,(struct sockaddr*)&client, sizeof(struct sockaddr_in));
				c++;
				//else{while((msg[n++]=getchar())!='\n');}
				sendto(newsockid,msg,sizeof(msg),0,(struct sockaddr*)&client, sizeof(struct sockaddr_in));
				if(strncmp("bye",msg,3)==0)
				 {
				   printf("Exit session...\n");
				   fl=1;
				   break;
				   return 0;
				 }
				 bzero(msg,sizeof(msg));
				 //break;
			}
			
		}

	return 0;
}
