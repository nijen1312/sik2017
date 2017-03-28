
/* -------------------------------------------------------
   program "potok"
   -------------------------------------------------------

   wywolanie : potok prog1 prog2 prog3 ...

   program ten tworzy procesy potomne z podanymi programami
   i laczy je "w potok" (dziala jak "prog1|prog2|prog3")

   sposob uzycia :
     potok ls cat sort sort
     potok cat cat sort sort

   PYTANIA:
     -> co by sie stalo gdyby zostawic lacza otwarte ?
        (odpowiedz na to pytanie jest b. istotna !)
     -> jaki warunek musza spelniac argumenty
        programu "potok" (filtry) ?
*/

#include "unix.h"

int socketPairs [10][2];
int IloscProg;

void ZamknijLacza()
{
  for(int i=0;i<IloscProg-1;i++)
  { 
    close(socketPairs[i][0]);
    close(socketPairs[i][1]);

  }
}


//       potok prog1 prog2 prog3
// argv: 0     1     2     3
//
main(int argc, char **argv)
{
  int i,j;
  IloscProg=argc-1;
  for(i=0;i<IloscProg;i++){
	  socketpair(AF_UNIX,SOCK_STREAM,0,socketPairs[i]);
  }

  for(i=0;i<IloscProg;i++)
  {
     int pid;
     pid=fork();
     if(pid==0) // proces potomny
       {
         if(i==0) // pierwszy proces (przeadresowujemy tylko stdout)
         {
	   dup2(socketPairs[i][0],1);
           ZamknijLacza();
           execlp(argv[i+1],argv[i+1],NULL);
           fprintf(stderr,"Blad exec (pid==%i) !!!\n",getpid());
           exit(1);
         }
         if(i!=0 && i!=IloscProg-1) // przeadresowujemy stdin i stdout
         { 
	   
	   dup2(socketPairs[i][0],1);
           dup2(socketPairs[i-1][1],0);

           ZamknijLacza();
           execlp(argv[i+1],argv[i+1],NULL);
           fprintf(stderr,"Blad exec (pid==%i) !!!\n",getpid());
           exit(1);
         }
         if(i==IloscProg-1) // ostatni proces (przeadresowujemy tylko stdin)
         {
           dup2(socketPairs[i-1][1],0);
           ZamknijLacza();
           execlp(argv[i+1],argv[i+1],NULL);
           fprintf(stderr,"Blad exec (pid==%i) !!!\n",getpid());
           exit(1);
         }
       }
  }

  // proces macierzysty zamyka wszystkie lacza ...
  ZamknijLacza();

  // czekamy na wszystkie procesy potomne ...
  //
  for(i=0;i<IloscProg;i++)
  {
    int pid,status;
    pid=wait(&status);
    fprintf(stderr,"wait; pid=%i status=%04X\n",pid,status);
  }
}


