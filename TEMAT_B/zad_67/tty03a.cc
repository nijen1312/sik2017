/*  
   "rozmowa" z modemem
   -------------------
   
   --> to jest wersja bez "aktywnego czekania";
       mozna obserwowac wykorzystanie procesora 
         ps -O ppid,pcpu -t pts/1
	
*/

#include <termio.h>
#include <unistd.h>
#include <stdio.h>
//#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

int koniec=0;

main()
{
  struct termio t1, t2, t3, t4; 

  ioctl(0,TCGETA,&t1); t2=t1;
  t2.c_lflag &= ~ECHO;
  t2.c_lflag &= ~ICANON;
  ioctl(0,TCSETA,&t2);
  
  int modem = open("/dev/ttyS3", O_RDWR);
  if(modem==-1) {
    perror("modem = open()"); goto XXX; 
  }
  ioctl(modem,TCGETA,&t3); t4=t3;
  t4.c_lflag &= ~ECHO;
  t4.c_lflag &= ~ICANON;
  t4.c_iflag &= ~ICRNL;
  ioctl(modem,TCSETA,&t4);

  extern void ObslugaSIGINT(int);
  signal(SIGINT,ObslugaSIGINT);
    /* aby bylo mozliwe zakonczenie programu,
	   polaczone z przywrocenim poczatkowej konfiguracji
	   terminala i modemu !!!
    */
  
  int i; char c; 
  fd_set Read,Read0;

  FD_ZERO(&Read0);
  FD_SET(0, &Read0);
  FD_SET(modem, &Read0);
  
  while(1) {
    Read=Read0;
    i=select(FD_SETSIZE, &Read, NULL, NULL, 0);
	if(i==-1) { perror("select"); break; }

    if( FD_ISSET(0,&Read) ) { 
	  read(0,&c,1); write(modem,&c,1);
	}
    
    if( FD_ISSET(modem,&Read) ) {
	  read(modem,&c,1); write(1,&c,1);
	}
  
    if(koniec==1) break;
  }

  ioctl(modem,TCSETA,&t3);
  XXX: 
  ioctl(0,TCSETA,&t1);

  printf("koniec tty03\n"); fflush(stdout);
}


void ObslugaSIGINT(int)
{
  koniec=1;
}

