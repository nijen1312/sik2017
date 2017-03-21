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
	connect(connSock,(struct sockaddr *)&connAddr,sizeof(connAddr));
	char buff[BUFFER_SIZE];
	int inpSize, messageSize;
	while((inpSize=read(0,buff,BUFFER_SIZE))!=0){
		write(connSock,buff,inpSize);
		messageSize=read(connSock,buff,BUFFER_SIZE);
		write(1,buff,messageSize);
	}
	close(connSock);
	return 0;
}

