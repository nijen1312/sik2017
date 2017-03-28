#include "unix.h"
#define LISTENING_PORT 10000
#define BUFFER_SIZE 1024
int main ()
{
	char buffer[BUFFER_SIZE];
	char separationMark='.';
	char output[BUFFER_SIZE];
	int count;
	int recievedBytes, sentBytes,recieved_len,i;
	int listSock = socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in listAddr;
	struct sockaddr_in recievedAddr;
	recieved_len=sizeof(recievedAddr);
	memset(&recievedAddr,0,sizeof(recievedAddr));
	memset(&listAddr,0,sizeof(listAddr));
	listAddr.sin_family=AF_INET;
	listAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	listAddr.sin_port=htons(LISTENING_PORT);
	bind(listSock,(struct sockaddr *)&listAddr,sizeof(listAddr));
	while(1){
		recievedBytes=recvfrom(listSock,buffer,BUFFER_SIZE,0,(struct sockaddr *)&recievedAddr,&recieved_len);
		count=0;
		for(i=0;i<recievedBytes;i++){
			if(i<recievedBytes-2){
				output[count++]=buffer[i];
				output[count++]='.';
			}else if(i<recievedBytes){
				output[count++]=buffer[i];
			}
		}
		sentBytes=sendto(listSock,output,count,0,(struct sockaddr *)&recievedAddr,recieved_len);
	}
	return 0;
}

