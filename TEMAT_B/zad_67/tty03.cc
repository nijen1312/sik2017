/*  
   "rozmowa" z modemem
   -------------------
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
  // zakladamy, ze modem jest podlaczony przez port nr 3
  // UWAGA: ten plik moze czytac/pisac tylko "root"
  //
  if(modem==-1) {
    perror("modem = open()"); goto XXX; 
  }
  ioctl(modem,TCGETA,&t3); t4=t3;
  t4.c_lflag &= ~ECHO;
  t4.c_lflag &= ~ICANON;
  t4.c_iflag &= ~ICRNL;
  //t4.c_ispeed=B115200; // w "termio" tego nie ma !!!
  //t4.c_ospeed=B115200;
  ioctl(modem,TCSETA,&t4);

  // teraz wlaczamy tryb nieblokujacy ...
  //
  fcntl(0,F_SETFL, fcntl(0,F_GETFL)|O_NONBLOCK );
  fcntl(modem,F_SETFL, fcntl(modem,F_GETFL)|O_NONBLOCK );

  extern void ObslugaSIGINT(int);
  signal(SIGINT,ObslugaSIGINT);
    /* aby bylo mozliwe zakonczenie programu,
	   polaczone z przywrocenim poczatkowej konfiguracji
	   terminala i modemu !!!
    */
  
  int i; char c; 
  while(1) {
    i=read(0,&c,1);
	if(i==-1 && errno!=EAGAIN) break;
	if(i==1) write(modem,&c,1);
    
	i=read(modem,&c,1);
	if(i==-1 && errno!=EAGAIN) break;
	if(i==1) write(1,&c,1);
  
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

