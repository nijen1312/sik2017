/*
   Gniazdka, najprostszy przyklad ...
   ----------------------------------
   */

#include "unix.h"
#define PORT 7000
#define BUFFER_SIZE 1024

void ObsSygnalu(int)
{ printf("pojawil sie sygnal SIGPIPE !!! (pid==%i)\n",getpid());
}
char *AdresCyfrowyKropkowy(char *name);

main()
{
	signal(SIGPIPE,ObsSygnalu);
	int pid,status,i,j;
	char buffer[BUFFER_SIZE];
	pid=fork();
	if(pid==0) // pp: tutaj bedzie serwer ...
	{
		printf("serwer: pid==%i\n",getpid()); 
		int sock=socket (AF_INET,SOCK_DGRAM,0);
		int recievedBytes,sentBytes;
		struct sockaddr_in serverAddress;
		memset(&serverAddress,0,sizeof(serverAddress));
		serverAddress.sin_family=AF_INET;
		serverAddress.sin_addr.s_addr=inet_addr("127.0.0.1");
		serverAddress.sin_port=htons(PORT);
		i=bind(sock,(struct sockaddr *)&serverAddress,sizeof(serverAddress));
		printf("serwer: bind()=%i\n",i);
		struct sockaddr_in incomingAddress;
		unsigned int incoming_len = sizeof(incomingAddress);
		for(int counter=1;counter<=2;counter++){
			recievedBytes=recvfrom(sock,buffer,BUFFER_SIZE,0,(struct sockaddr *)&incomingAddress,&incoming_len);
			printf("serwer: read()=%i buf=%s\n",recievedBytes,buffer);
			sentBytes=sendto(sock,buffer,recievedBytes,0,(struct sockaddr *)&incomingAddress,incoming_len);
			printf("serwer: write()=%i\n",sentBytes);
		}
		close(sock);
		exit(0);
	}
	else // pm: tutaj bedzie klient ...
	{
		printf("klient: pid==%i\n",getpid()); 
		extern void klient(char *);
		sleep(1);
		printf("klient: pierwsze polaczenie ...\n");
		klient("1234567890");
		sleep(1);
		printf("klient: drugie polaczenie ...\n");
		klient("QWERTYUIOP");
		i=wait(&status);
		printf("klient: wait()=%i %04X\n",i,status);
	}
}

void klient(char *c)
{
	int i,j;
	struct sockaddr_in clientAddress;
	int sock=socket (AF_INET,SOCK_DGRAM,0);
	memset(&clientAddress,0,sizeof(clientAddress));
	clientAddress.sin_family=AF_INET;
	clientAddress.sin_addr.s_addr=INADDR_ANY;
	clientAddress.sin_port=htons(0);
	i=bind(sock,(struct sockaddr *)&clientAddress,sizeof(clientAddress));
	struct sockaddr_in targetAddress;
	unsigned int target_len = sizeof(targetAddress);
	memset(&targetAddress,0,sizeof(targetAddress));
	targetAddress.sin_family=AF_INET;
	targetAddress.sin_addr.s_addr=inet_addr("127.0.0.1");
	targetAddress.sin_port=htons(PORT);
	i=sendto(sock,c,strlen(c),0,(struct sockaddr *)&targetAddress,target_len);
	printf("klient: write()=%i\n",i);
	char buf[100];
	i=recvfrom(sock,buf,99,0,(struct sockaddr *)&targetAddress,&target_len);
	buf[i]=0;
	printf("klient: read()=%i buf=%s\n",i,buf);
	close(sock);
}

char *AdresCyfrowyKropkowy(char *name)
{
	struct hostent *hostp;
	if ((hostp=(struct hostent *)gethostbyname(name))!=NULL)
	{
		return inet_ntoa(*(in_addr*)*hostp->h_addr_list);
		// jak dlugo ten wynik istnieje ??????
	}
}

