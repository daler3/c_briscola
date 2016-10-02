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
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h> //per tcflush
#include "funzionibriscola.h"

//INIZIO MAIN
int main(int argc, char *argv[]) {
	/*DICHIARAZIONI*/
	int sd, new_sd=-1; //socket descriptor
	int i, scelta, pilapunti=0, puntiprovv, ncartepescate=0, contmani=0, porta, conttemp=0, nplayer;
	/*per gestire i semi e i tipi delle carte*/ 
	char semaforo, onetwo, messaggiovincita[15]; 
	carta briscola, cartavv, cartatemp; 
	struct sockaddr_in server_addr;
	listaDiCarte mano;
  cardstruct* punttemp;
	/*FINE DICHIARAZIONI*/

	/*INIZIALIZZAZIONI*/
  inizializza(messaggiovincita, 15);
	porta=atoi(argv[2]);
	adrr_initialize (&server_addr, porta, (long)inet_addr(argv[1]));
	sd=socket(AF_INET, SOCK_STREAM, 0);	
	inizializzalist(&mano); 

  printf("\n\n-----------------------------------------------------------------\n");
	/*Connessione*/
	new_sd=connect(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(new_sd<0){
		printf("Errore connessione\n");
		return 0; 
	} else printf ("\nSei connesso. Attendi...\n");

  printf("\n\n-----------------------------------------------------------------\n");

	/*ricevo semaforo: significa che anche l'altro giocatore si è connesso*/
	reccarattere(sd, &semaforo);
  recintero(sd, &nplayer); //ho ricevuto il mio nplayer. Sarò o il Player 1 o il Player 2.

	printf("Ora riceverai LA BRISCOLA...\n ");
  reccarta(sd, &briscola);
  printf("La BRISCOLA è: "); 
  stampacarta(briscola); 
  
	printf("Ora riceverai le tue carte...");
	for(i=0; i<3; i++){
		  reccarta(sd, &cartatemp);
    	insInTesta(&mano, cartatemp);
  } 
  ncartepescate=3; 

  printf("\n\n-----------------------------------------------------------------\n");
  printf ("\nTu sei il Player %d \n", nplayer); 
  if(nplayer==1) printf("Inizierai tu la partita\n ");
  else printf("Il tuo avversario inizierà la partita\n ");

  printf("\n\n-----------------------------------------------------------------\n");
  printf("\n----------------        INIZIO PARTITA!!      -------------------\n\n");
	/*INIZIO CICLO PARTITA */
	for(contmani=0; contmani!=20; contmani++) {
		  printf("\n\n-----------------------------------------------------------------\n");
      printf("\n\n-----------------------------------------------------------------\n");
		  printf("\n Mano n. %d - Player %d \n ", contmani+1, nplayer);
    	printf("Ecco la tua mano:\n");
    	stampaLista(&mano);
      printf("Ricordati che la BRISCOLA è: ");
      stampacarta(briscola);
    	reccarattere(sd, &onetwo); //per sincronizzare 

	    if (onetwo=='v'){
		  	scelta=choice(contmani); //scelgo la carta da buttare
     		/*cerco la carta che ho scelto di giocare*/
		    punttemp=mano; 
     		conttemp=0; 
     		while (conttemp!=(scelta-1)){
          		punttemp=punttemp->prox; 
          		conttemp++;
      	}
      	inviacarta(sd, &(punttemp->cartacorrente));
		    eliminaCartaSpecifica (&mano, (punttemp->cartacorrente));
		    reccarta(sd, &cartavv);
	    	printf("\nLa carta che l'avversario ha giocato è:  ");
    		stampacarta(cartavv);
    	} else {
        printf("\nNon è il tuo turno. Attendi il tuo...\n");
        reccarta(sd, &cartavv);
	    	printf("\nLa carta che l'avversario ha giocato è:  ");
    		stampacarta(cartavv);
		    scelta=choice(contmani);
      	punttemp=mano; 
      	conttemp=0; 
     		while (conttemp!=(scelta-1)){
         		punttemp=punttemp->prox; 
          	conttemp++;
      	}
      	inviacarta(sd, &(punttemp->cartacorrente));
	    	eliminaCartaSpecifica (&mano, (punttemp->cartacorrente));
    	}

 	    recintero(sd, &puntiprovv);
 	    pilapunti=pilapunti+puntiprovv; 

 	    recstringa(sd, &messaggiovincita[0], sizeof(messaggiovincita));
      printf("\n\n-----------------------------------------------------------------\n");
	    printf("\n\n Esito manche: %s \n", messaggiovincita);
      printf(" RIEPILOGO MANCHE: "); 
      printf("\n -Carta da te giocata: ");
      stampacarta(punttemp->cartacorrente);
      printf(" -Carta giocata dall'avversario: ");
      stampacarta(cartavv);
      printf("\n Punti totalizzati nella manche: %d \n", puntiprovv);

    	//ricevo la carta SE POSSO RICEVERLA, QUINDI SE NON SONO GIA' STATE PESCATE TUTTE LE CARTE. 
    	if (ncartepescate<20){
        reccarta(sd, &cartatemp);
    		//reccarta(sd, &cartatemp);
        printf("\n\n-----------------------------------------------------------------\n");
    		printf("\nNuova carta pescata: ");
    		stampacarta(cartatemp);
	      insInTesta(&mano, cartatemp);
	      //stampacarta(cartatemp);
	      /*printf("Ed ecco la tua mano: \n");
	      stampaLista(&mano); */
    		ncartepescate++;
    	} 

	}//fine ciclo partita
  printf("\n\n-----------------------------------------------------------------\n");
  printf("-----------------------------------------------------------------\n");
  printf("-----------------------------------------------------------------\n");
  printf("\n----------------        FINE PARTITA!!      ---------------------\n\n");

	//fine partita, mando i miei punteggi e ricevo il messaggio di vittoria o perdita 
	printf("\nPunti totalizzati nella partita: %d\n", pilapunti);
	inviaintero(sd, &pilapunti);
	recstringa(sd, &messaggiovincita[0], sizeof(messaggiovincita));
  printf("\nESITO PARTITA: %s la partita!! con %d punti\n", messaggiovincita, pilapunti);
  printf("-----------------------------------------------------------------\n\n");

  /*Chiudo i descrittori dei socket*/
  close(new_sd);
  close(sd);
	return 0; 
} 
/*FINE MAIN*/