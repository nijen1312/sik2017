#include "unix.h"
#define LISTENING_PORT 10000
#define MAX_HOST_QUEUE 5
int main ()
{
	int listSock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in listAddr;
	memset(&listAddr,0,sizeof(listAddr));
	listAddr.sin_family=AF_INET;
	listAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	listAddr.sin_port=htons(LISTENING_PORT);
	bind(listSock,(struct sockaddr *)&listAddr,sizeof(listAddr));
	listen(listSock,MAX_HOST_QUEUE);
	while(1){
		int incSock = accept(listSock,NULL,NULL);
		int pid = fork();
		if(pid==0){
			dup2(incSock,0);
			dup2(incSock,1);
			close(incSock);
			execlp("./kropkaBin","./kropkaBin",NULL);
		}
		if(pid>0){
			close(incSock);
		}
	}
	return 0;
}

