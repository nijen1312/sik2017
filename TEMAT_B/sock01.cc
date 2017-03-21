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
		int recievedBytes;

		struct sockaddr_in Addres;
		memset(&Addres,0,sizeof(Addres));
		Addres.sin_family=AF_INET;
		Addres.sin_addr.s_addr=INADDR_ANY;
		Addres.sin_port=htons(PORT);
		i=bind(sock,(struct sockaddr *)&Addres,sizeof(Addres));
		printf("serwer: bind()=%i\n",i);
		struct sockaddr_in incomingAddress;
		unsigned int recieved_len = sizeof(incomingAddress);
		for(int licznik=1;licznik<=2;licznik++){
			recievedBytes=recvfrom(sock,buffer,BUFFER_SIZE,0,(struct sockaddr *)&incomingAddress,&recieved_len);
			sendto(sock,buffer,recievedBytes,0,(struct sockaddr *)&incomingAddress,recieved_len);
		}

		// czekamy na dokladnie 2 klientow !
		//
		//for(int licznik=1;licznik<=2;licznik++)
		//{
		//printf("serwer: czekam na klienta nr %i\n",licznik);
		//int nowy_sock=accept(sock,NULL,NULL);
		//printf("serwer: accept()=%i\n",nowy_sock);
		//printf("serwer: poczatek obslugi klienta nr %i\n",licznik);

		//char buf[100];
		//i=read(nowy_sock,buf,99);
		//buf[i]=0;
		//printf("serwer: read()=%i buf=%s\n",i,buf);

		//j=write(nowy_sock,buf,i);
		//printf("serwer: write()=%i\n",j);
		//// jak widac serwer dziala jak 'echo'
		//// odsyla to co przyszlo !

		//close(nowy_sock);
		//}

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
	struct sockaddr_in Addres;
	int sock=socket (AF_INET,SOCK_DGRAM,0);
	memset(&Addres,0,sizeof(Addres));
	Addres.sin_family=AF_INET;
	Addres.sin_addr.s_addr=INADDR_ANY;
	Addres.sin_port=htons(PORT);
	i=bind(sock,(struct sockaddr *)&Addres,sizeof(Addres));
	struct sockaddr_in adres;
	memset(&adres,0,sizeof(adres));
	adres.sin_family=AF_INET;
	//adres.sin_addr.s_addr=inet_addr(AdresCyfrowyKropkowy("venus"));
	//adres.sin_addr.s_addr=inet_addr("1.2.3.4");
	adres.sin_addr.s_addr=INADDR_ANY;
	adres.sin_port=htons(PORT);
	sendto(sock,c,strlen(c),0,(struct sockaddr *)&incomingAddress,recieved_len);
	i=connect(sock,(struct sockaddr *)&adres,sizeof(adres));
	printf("klient: connect()=%i\n",i);

	i=write(sock,c,strlen(c));
	printf("klient: write()=%i\n",i);

	char buf[100];
	i=read(sock,buf,99);
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

