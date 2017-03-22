#include "unix.h"
#define LISTENING_PORT 10000
#define BUFFER_SIZE 1024
int main ()
{

	int connSock = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in connAddr;
	unsigned int conn_len=sizeof(connAddr);
	memset(&connAddr,0,sizeof(connAddr));
	connAddr.sin_family=AF_INET;
	connAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	connAddr.sin_port=htons(LISTENING_PORT);
	struct sockaddr_in clientAddr;
	memset(&clientAddr,0,sizeof(clientAddr));
	clientAddr.sin_family=AF_INET;
	clientAddr.sin_addr.s_addr=INADDR_ANY;
	clientAddr.sin_port=htons(0);
	bind(connSock,(struct sockaddr *)&clientAddr,sizeof(clientAddr));
	char buff[BUFFER_SIZE];
	int inpSize, sentBytes, messageSize;
	while((inpSize=read(0,buff,BUFFER_SIZE))!=0){
		sentBytes=sendto(connSock,buff,inpSize,0,(struct sockaddr *)&connAddr,conn_len);
		messageSize=recvfrom(connSock,buff,BUFFER_SIZE,0,(struct sockaddr *)&connAddr,&conn_len);
		write(1,buff,messageSize);
	}
	close(connSock);
	return 0;
}

