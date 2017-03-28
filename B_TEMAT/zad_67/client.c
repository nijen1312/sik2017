#include "unix.h"
#define LISTENING_PORT 10000
#define BUFFER_SIZE 1024
int main ()
{

	int connSock = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in connAddr;
	memset(&connAddr,0,sizeof(connAddr));
	connAddr.sin_family=AF_INET;
	connAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	connAddr.sin_port=htons(LISTENING_PORT);
	int connStatus=connect(connSock,(struct sockaddr *)&connAddr,sizeof(connAddr));
	printf("status polaczenia: %d\n",connStatus);
	char buff[BUFFER_SIZE];
	int inpSize, messageSize;

	fd_set Read, Read0;
	FD_ZERO(&Read0);
	FD_SET(connSock, &Read0);
	FD_SET(0, &Read0);

	while(1) {
		Read=Read0;
		select(FD_SETSIZE, &Read, NULL, NULL, 0);
		if( FD_ISSET(0,&Read) ) { 
			inpSize=read(0,buff,BUFFER_SIZE);write(connSock,buff,inpSize);
		}
		if( FD_ISSET(connSock,&Read) ) { 
			messageSize=read(connSock,buff,BUFFER_SIZE);write(1,&buff,messageSize);
		}
	}
	close(connSock);
	return 0;
}

