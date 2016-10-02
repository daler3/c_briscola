/*
---------------------------------------------------------------------------------------
- Progetto Software - 
Realizzazione del gioco "Briscola" 1 vs 1 in rete locale, tramite l'utilizzo dei socket
---------------------------------------------------------------------------------------
Autore: Daniele Romanini
---------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <termios.h>
#include "funzionibriscola.h"

//funzione che riempie e mescola il mazzo
void riempiemescola (listaDiCarte *p){
  carta mazzo[40];
  int cont1, cont2, i=0, cont; 
  for(cont1=0; cont1<4; cont1++){
    for(cont2=0; cont2<10; cont2++){
      mazzo[i].seme=cont1; 
      mazzo[i].tipo=cont2;
      i++;
    }
  }
  mescolamazzoarray(mazzo); //il mazzo è riempito, chiamo la funzione per mescolarlo
  for (cont=0; cont<40; cont++){
    insInTesta(p, mazzo[cont]); 
  }
}


//funzione che mescola un array di carte 
void mescolamazzoarray(carta *mazzo){
  carta cartaappoggio; 
  int i=0, j=0, p;
  for (p=10; p!=0; p--){
    for(i=40; i!=0; i--){
      srand(time(NULL));
      j=rand()%40;
      cartaappoggio=mazzo[i-1]; 
      mazzo[i-1]=mazzo[j];
      mazzo[j]=cartaappoggio;
    } 
    for(i=40; i!=0; i--){
      srand(time(NULL));
      j=rand()%i;
      cartaappoggio=mazzo[i-1]; 
      mazzo[i-1]=mazzo[j];
      mazzo[j]=cartaappoggio;
    } 
  }
}

//invia intero 
void inviaintero(int d, int *dato){
  if ((send(d, dato, sizeof(int), 0))<0){
    printf("Errore nel mandare il dato. \n");
    return;
  }
  return; 
}

//ricevi intero
void recintero(int d, int *dato){
  if ((recv(d, dato, sizeof(int), 0))<0){
    printf("Errore nel ricevere il dato\n");
    return;
  }
  return; 
}

//invia carattere
void inviacarattere(int d, char *dato){
  if ((send(d, dato, sizeof(char), 0))<0){
    printf("Errore nel mandare il carattere \n");
    return;
  }
  return; 
}

//ricevi carattere
void reccarattere(int d, char *dato){
  if ((recv(d, dato, sizeof(char), 0))<0){
    printf("Errore nel ricevere il carattere \n");
    return;
  }
  return; 
}

//invia carta
void inviacarta(int d, carta *dato){
  if ((send(d, (void*)dato, sizeof(carta), 0))<0){
    printf("Errore nel mandare la carta\n");
    return;
  }
  return; 
}

//ricevi carta
void reccarta(int d, carta *dato){
  if ((recv(d, (void*)dato, sizeof(carta), 0))<0){
    printf("Errore nel ricevere la carta\n");
    return;
  }
  return; 
}

//invia stringa
void inviastringa(int d, char* dato, int dim){
  if ((send(d, dato, dim, 0))<0){
    printf("Errore nel mandare il messaggio \n");
    return;
  }
  return; 
}

//ricevi stringa
void recstringa(int d, char* dato, int dim){
  if ((recv(d, dato, dim, 0))<0){
    printf("Errore nel ricevere il messaggio \n");
    return;
  }
  return; 
}


//funzioni
int scegliere (){
  int scelta;
	while (scanf("%d", &scelta) != 1) {
      printf("\nInserisci un numero valido \n");
     	while (getchar() != '\n');
  }
   	return scelta; 
}

//funzione vincita
int vincita (carta prima, carta seconda, semecarta semebriscola){
    if (prima.seme == seconda.seme){
      if(prima.tipo>seconda.tipo)
        return 0; 
      else return 1; 
    } else {  //i semi delle carte sono diversi
      if (prima.seme==semebriscola)   
        return 0; 
      if (seconda.seme==semebriscola)
        return 1; 
    }
    return 0; 
}

//funzione insInTesta
void insInTesta (listaDiCarte *p, carta inscarta) {
  cardstruct *punt; 
  punt=malloc(sizeof(cardstruct)); 
  punt->cartacorrente=inscarta; 
  punt->prox=*p; 
  *p=punt;
}


//funzione che elimina una carta specifica
void eliminaCartaSpecifica(listaDiCarte *l, carta cartapassata) {
  cardstruct *puntCorr, *puntPrec;
  puntPrec=NULL;
  puntCorr=*l;
  int var=0; 
  while(puntCorr!=NULL && var==0) {
    if(cartapassata.seme==puntCorr->cartacorrente.seme && cartapassata.tipo==puntCorr->cartacorrente.tipo){
      var=1;
    } else{
      puntPrec=puntCorr;
      puntCorr=puntCorr->prox;
    }
  }
  
  if(puntCorr!=NULL) {
    if (puntCorr==*l)
      *l=puntCorr->prox; //è proprio *l che devo eliminare
    else
      puntPrec->prox=puntCorr->prox; //bypasso
    free(puntCorr);
  }
}

//funzione che elimina un elemento
carta eliminaCarta (listaDiCarte *p) {
  cardstruct *punt; 
  carta cartadelete; 
  if ((*p)!=NULL){
    punt=*p;
    cartadelete=punt->cartacorrente;
    *p=(*p)->prox; 
    free (punt);
  }
  return cartadelete; 
}

//funzione che riempie il mazzo
void riempimentomazzo (listaDiCarte *p){
  carta cartadainserire; 
  int cont1, cont2; 
  for(cont1=0; cont1<4; cont1++){
    for(cont2=0; cont2<10; cont2++){
      cartadainserire.seme=cont1; 
      cartadainserire.tipo=cont2; 
      insInTesta (p, cartadainserire); //chiamofunzioneperinserirelaarta
    }
  }
}

//funzione che mescola il mazzo
void mescolamazzo(listaDiCarte *punt){
  cardstruct *puntcorr1, *puntcorr2;
  puntcorr1=*punt; 
  puntcorr2=*punt;  
  carta cartaappoggio; 
  int i=0, j=0, p, k=0;
  for (p=30; p!=0; p--){
    for(i=0; i<40; i++){
      srand(time(NULL));
      j=rand()%40;
        for(k=0; k<j; k++){
          puntcorr1=*punt;
          puntcorr1=puntcorr1->prox; 
        }
      cartaappoggio=puntcorr2->cartacorrente;  
      puntcorr2->cartacorrente=puntcorr1->cartacorrente;
      puntcorr1->cartacorrente=cartaappoggio;

      puntcorr2=puntcorr2->prox;
    } 
  }
}
//funzione che stampa una carta
void stampacarta(carta card){
  char* semiarray[] = {"DENARI", "SPADE", "BASTONI", "COPPE"};
  char* tipiarray[] = {"DUE", "QUATTRO", "CINQUE", "SEI", "SETTE", "FANTE", "CAVALLO", "RE", "TRE", "ASSO"};
  printf("  %s di %s\n", tipiarray[card.tipo], semiarray[card.seme]);
}

//funzioni
void adrr_initialize(struct sockaddr_in* indirizzo, int port, long IPaddr){
  indirizzo->sin_family = AF_INET;
  indirizzo->sin_port=htons((u_short) port);
  indirizzo->sin_addr.s_addr=IPaddr; 
}

void inizializza(char array[], int dim){
  int i=0; 
  for (i=0; i<dim; i++){
    array[i]='\0';
  }
}

//funzione controllo lista vuota
boolean ListaVuota(cardstruct *l) {
  if (l==NULL) return true; 
  else return false; 
}

//funz che stampa il contenuto di una lista 
void stampaLista (listaDiCarte *p) {
  cardstruct *punt; 
  punt=*p;
  int i=0; 
  while(punt!=NULL){
    printf("  %d.: ", i+1);
    stampacarta (punt->cartacorrente);
    punt=punt->prox;  
    i++;
  }
  printf("\n");
}

//funzione inizializza lista
void inizializzalist (listaDiCarte *p) {
 *p=NULL;
}


int choice(int cont) {
	   int scelta; 
	   tcflush(STDIN_FILENO, TCIFLUSH);
      if (cont<18){
        printf("\nOra è il tuo turno. Scegli quale carta buttare, premendo 1, 2 o 3: ");
        scelta=scegliere();
        printf("\nAttendi...\n");
        while (scelta!=1 && scelta !=2 && scelta !=3){
          printf("\nErrore nell'effettuare la scelta. Rifai. Premi 1, 2 o 3: ");
          scelta=scegliere();
          printf("\nAttendi...\n");
        }
      }else if (cont==18){
        printf("\nOra è il tuo turno. Scegli quale carta buttare, premendo 1 o 2: ");
        scelta=scegliere();
        printf("\nAttendi...\n");
        while (scelta!=1 && scelta !=2){
          printf("\nErrore nell'effettuare la scelta. Rifai. Premi 1 o 2: ");
          scelta=scegliere();
          printf("\nAttendi...\n");
        }
      } else if (cont==19){
        printf("\nOra è il tuo turno. Scegli quale carta buttare, premendo 1: ");
        scelta=scegliere();
        printf("\nAttendi...\n");
        while (scelta!=1){
          printf("\nErrore nell'effettuare la scelta. Rifai. Premi 1: ");
          scelta=scegliere();
          printf("\nAttendi...\n");
        }
      }
return scelta; 
}