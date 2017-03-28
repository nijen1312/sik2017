/*
    "Potok" ze sledzeniem komunikatow
    wysylanych miedzy parami procesow
    ---------------------------------

    --> proc. macierzysty tworzy proces potomny dla kazdej komendy;
	kazda komenda komunikuje sie z proc. macierzystym
	poprzez dwa lacza (jednokierunkowe)

    --> "potok4" posiada tablice "L"
	ktorej element "L[i]" zawiera deskryptory pozwalajace
	pisac i czytac do/z komendy nr "i".

    --> "potok4" oczekuje na dane w deskryptorach
	tablicy "L[]" i odpowiednio je przepisuje
	(do oczekiwania na dane w zbiorze deskryptorow
	uzyto fun. "select()" - mozna by takze uzyc fun "poll()" !)

    --> sposob uzycia :
	      potok4 cat \| cat \| cat

    --> UWAGA: zbadaj zuzycie czasu procesora 
        przy pomocy polecenia:
          ps -O ppid,pcpu -t pts/1
          # oczywiscie, terminal moze byc inny (polecenie "tty")

*/

#include "unix.h"

// fun. do interpretowania linii z poleceniem ...
#define MAX_ILOSC_SLOW 50
#define MAX_ILOSC_KOMEND 30
int Komenda(char *linia, char **slowo);
int Potok(char **slowo, char ***komenda);

#define RAPORT_O_KOMUNIKATACH
// biorac to w komentarz wylaczasz raport o komunikatach

//#define TESTOWANIE
// biorac to w komentarz wylaczasz testowanie

struct Lacze { int do_czyt, do_pis; };
Lacze L [MAX_ILOSC_KOMEND];
int IloscKomend;
void ObslugaSIGPIPE(int i);

main(int argc, char **argv)
{
  if(argc<=1) {
    fprintf(stderr, "Sposob wywolania: "
      "potok4 komenda[0] \\| k[1] \\| k[2] ...\n");
    exit(1);
  }

  int i,j,k,l;
  char **komenda[MAX_ILOSC_KOMEND];

  // interpretacja argumentow programu
  //
  IloscKomend=Potok(argv+1, komenda);

  // tworzenie procesow potomnych
  // oraz jednej pary "lacz" na kazda komende
  //
  for(i=0; i<IloscKomend; i++)
  {
     Lacze L2 [2];
     pipe((int*)&L2[0]);
     pipe((int*)&L2[1]);

     int pid=fork();
     if(pid==0) // proces potomny
     {
       dup2(L2[0].do_czyt, 0);
       dup2(L2[1].do_pis, 1);
       close(L2[0].do_czyt);
       close(L2[0].do_pis);
       close(L2[1].do_czyt);
       close(L2[1].do_pis);
       
       for(j=0; j<i; j++) { // nalezy zamknac deskryptory 
                            // otwarte w poprzednich iteracjach ...
         close(L[j].do_czyt);
	 close(L[j].do_pis);
       }
       
       execvp(*komenda[i],komenda[i]);
       fprintf(stderr,"komenda[%i]: exec; Blad !!! (pid==%i)\n", i, getpid());
       exit(1);
     }

     // proces macierzysty
     L[i].do_pis=L2[0].do_pis;
     L[i].do_czyt=L2[1].do_czyt;
     close(L2[0].do_czyt);
     close(L2[1].do_pis);
  }

  // czekamy na mozliwosc pisania/czytania
  // do/z desk. tablicy "L[]" + "0" + "1"
  //
  fd_set Read0, Write0;
  fd_set Read, Write;
  char c;

  FD_ZERO(&Read0);
  FD_ZERO(&Write0);
  FD_SET(0, &Read0);
  FD_SET(1, &Write0);
  for(i=0; i<IloscKomend; i++) {
    FD_SET(L[i].do_czyt, &Read0);
    FD_SET(L[i].do_pis, &Write0);
  }

  signal(SIGPIPE, ObslugaSIGPIPE);
    // ten sygnal nie powinien sie pojawic ...

  while(1) {
    Read=Read0; Write=Write0;

    i=select(FD_SETSIZE, &Read, &Write, NULL, 0);
#ifdef TESTOWANIE
    fprintf(stderr,"potok4: select(&Read)=%i\n", i);
    fflush(stderr);
#endif
    if(i==-1) perror("potok4: select(); Blad !!!");

    //
    // przepisywanie, o ile to mozliwe ...
    //
    if(FD_ISSET(0, &Read)
    && FD_ISSET(L[0].do_pis, &Write)) {
      i=read(0, &c, 1);
      if(i==0) { // oznacza to, ze terminal "sie skonczyl" ...
  	    close(L[0].do_pis);
        FD_CLR(L[0].do_pis, &Write0);
        FD_CLR(0, &Read0);
#ifdef RAPORT_O_KOMUNIKATACH
        fprintf(stderr,"potok4: terminal sie skonczyl\n");
        fflush(stderr);
#endif
      } else {
	    j=write(L[0].do_pis, &c, i);
#ifdef RAPORT_O_KOMUNIKATACH
	    fprintf(stderr,"potok4: tty->%c>-k[0]\n", (c>=32)?c:'.');
        fflush(stderr);
#endif
      }
    }

    for(k=0; k<IloscKomend-1; k++)
    if(FD_ISSET(L[k].do_czyt, &Read)
    && FD_ISSET(L[k+1].do_pis, &Write)) {
      i=read(L[k].do_czyt, &c, 1);
      if(i==0) { // oznacza to, ze komenda nr "k" sie zakonczyla
		 // i zamknela swoje koncowki laczy ...
  	    close(L[k+1].do_pis);
        FD_CLR(L[k+1].do_pis, &Write0);
        FD_CLR(L[k].do_czyt, &Read0);
#ifdef RAPORT_O_KOMUNIKATACH
        fprintf(stderr,"potok4: komenda k[%i] sie zakonczyla\n", k);
        fflush(stderr);
#endif
      } else {
	    j=write(L[k+1].do_pis, &c, i);
#ifdef RAPORT_O_KOMUNIKATACH
	    fprintf(stderr,"potok4: k[%i]->%c>-k[%i]\n", 
	      k, (c>=32)?c:'.', k+1);
        fflush(stderr);
#endif
      }
    }

    if(FD_ISSET(L[IloscKomend-1].do_czyt, &Read)
    && FD_ISSET(1, &Write)) {
      i=read(L[IloscKomend-1].do_czyt, &c, 1);
      if(i==0) { // oznacza to, ze ostatnia komenda sie zakonczyla
	    // i zamknela swoje koncowki laczy ...
#ifdef RAPORT_O_KOMUNIKATACH
        fprintf(stderr,"potok4: komenda k[%i] sie zakonczyla\n", 
             IloscKomend-1);
        fflush(stderr);
#endif
	     break;
	      // wyjscie z petli z "select()"-em
      } else {
	    j=write(1, &c, i);
#ifdef RAPORT_O_KOMUNIKATACH
	    fprintf(stderr,"potok4: k[%i]->%c>-tty\n", 
	       IloscKomend-1, (c>=32)?c:'.');
        fflush(stderr);
#endif
      }
    }
  }

  // czekamy na wszystkie procesy potomne
  //
  fprintf(stderr,"potok4: wait ...\n");
  fflush(stderr);
  for(i=0; i<IloscKomend; i++)
  {
    int pid,status;
    pid=wait(&status);
    fprintf(stderr,"potok4: wait()=%i, status=%04X\n",pid,status);
    fflush(stderr);
  }
}

void ObslugaSIGPIPE(int i)
{
  fprintf(stderr, "potok4: wystapil sygnal SIGPIPE !!!\n");
}

// ------------------------------------------------

// rozbicie linii na ciag slow,
// elementy tablicy "slowo" wskazuja na kolejne slowa
// funkcja zwraca ilosc slow
// UWAGA: "linia" jest modyfikowana !
//
int Komenda(char *linia, char **slowo)
{
  char *c;
  int i=0;
  c=strtok(linia," ");
  while(c!=NULL && i<(MAX_ILOSC_SLOW-1)) {
    slowo[i]=c; i++;
    c=strtok(NULL," ");
  }
  slowo[i]=NULL;
  return i;
}

// rozbicie potoku na komendy
// UWAGA: "slowo" jest modyfikowane !
// UWAGA: kazdy znak "|" musi byc osobnym slowem !
//
int Potok(char **slowo, char ***komenda)
{
  int i=0, j=0;
  while( slowo[i]!=NULL && j<(MAX_ILOSC_KOMEND-1) ) {
    if(i==0) {
      komenda[j]=&slowo[i];
      j++;
    }
    if(strcmp(slowo[i],"|")==0) {
      komenda[j]=&slowo[i+1];
      j++;
      slowo[i]=NULL;
    }
    i++;
  }
  komenda[j]=NULL;
  return j;
}

