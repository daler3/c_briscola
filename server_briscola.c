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
#include "funzionibriscola.h"

//INIZIO MAIN
int main(int argc, char *argv[]) {  
  /*DICHIARAZIONI*/
	int sd, new_sd1, new_sd2, arraysd[2]; /*socket descriptors*/
	int i, cont, pilapunti[2], totaleprovv=0, winner, sdappoggio, alwayszero=0, finebriscola=0, porta, contmani=0, var; 
	char semaforo='g'; 
	char sem1='v'; 
	char sem2='r';  
	char messwin[15]="HAI VINTO!    \n";
	char messlos[15]="HAI PERSO!    \n";
	char messpari[15]="HAI PAREGGIATO\n";
  struct sockaddr_in server_addr, client_addr[2];
  carta briscola, cartamano1, cartamano2, cartatemp; 
  socklen_t client_len[2];
  listaDiCarte mazzo; //questo sarà il mazzo
  /*FINE DICHIARAZIONI*/

  /*INIZIALIZZAZIONI*/
	porta=atoi(argv[1]); /*acquisisco il numero di porta su cui poi mi connetterò*/
  /*inizializzo le client_len*/
	client_len[0]=sizeof(client_addr[0]);
	client_len[1]=sizeof(client_addr[1]);
	adrr_initialize (&server_addr, porta, INADDR_ANY);
	sd=socket(AF_INET, SOCK_STREAM, 0);
	inizializzalist(&mazzo); //inizializzato il mazzo
  /*FINE INIZIALIZZAZIONI*/

  printf("\n\n-----------------------------------------------------------------\n");
	for(i=0; i<2; i++){
		bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr));
		listen(sd, 5); 
		printf("In attesa di connessione: Player %d\n", i+1);
		arraysd[i]=accept(sd, (struct sockaddr *)&client_addr[i], &client_len[i]);
		if (arraysd[i]>=0){
			printf("   Ho accettato una connesione: Player %d \n", i+1);
		} else return 0;
	}
  new_sd1=arraysd[0];
  new_sd2=arraysd[1];

  /*invio un "semaforo" ai due client per riferire loro che entrambi i giocatori si sono connessi*/
	for(i=0; i<2; i++){
      inviacarattere(arraysd[i], &semaforo);
	}
  for(i=1; i<3; i++){
      inviaintero(arraysd[i-1], &i);  //invio il numero identificativo di player
  }
  printf("\n\n-----------------------------------------------------------------\n");
	printf("\n");
  //riempio e mescolo il mazzo
  riempiemescola(&mazzo);
	//stampaLista(&mazzo); 

  /*tolgo la briscola dal mazzo*/
	briscola=eliminaCarta(&mazzo); 
	printf("La briscola è: "); 
  stampacarta(briscola); 
	printf("Ora manderai LA BRISCOLA ai giocatori\n ");
	for(i=0; i<2; i++){
      inviacarta(arraysd[i], &briscola);
	}

	printf("Ora manderai le CARTE ai giocatori\n ");
	for(i=0; i<2; i++){
		for(cont=0; cont<3; cont++){
			if((mazzo)!=NULL){
			   cartatemp=eliminaCarta(&mazzo);
			   inviacarta(arraysd[i], &cartatemp);
			}
		}
	}
	printf("\n");
  printf("\n\n-----------------------------------------------------------------\n");
  printf("\nDurante la partita, il giocatore identificato come PRIMO è il primo che deve giocare durante la mano, quello identificato come SECONDO è il secondo a giocare durante la mano. Essi quindi possono scambiarsi durante la partita.\nIl giocatore che comincia la partita è invece il primo che si è connesso e sarà identificato come 'Player 1', mentre l'altro come 'Player2'. ");
  printf("\n\n-----------------------------------------------------------------\n");
  printf("\n----------------        INIZIO PARTITA!!      -------------------\n\n");

  /*inizio ciclo-partita*/
  while (finebriscola<4){	
        printf("\n\n-----------------------------------------------------------------\n");
        printf("\n\n-----------------------------------------------------------------\n");
        printf("\n Mano n. %d:\n ", contmani+1);
        printf("BRISCOLA: ");
        stampacarta(briscola);
	      printf("\nOra tutti hanno ricevuto le carte, il primo deve buttare\n");
        inviacarattere(arraysd[1], &sem2);
        inviacarattere(arraysd[0], &sem1);
        printf("\nOra riceverò la carta del primo..\n");
        reccarta(arraysd[0], &cartamano1);
        if(new_sd1==arraysd[0])
          printf(" -Player 1: ");
        else printf(" -Player 2: ");
        stampacarta(cartamano1);
        inviacarta(arraysd[1], &cartamano1);
        printf("\nOra riceverò la carta del secondo..\n");
        reccarta(arraysd[1], &cartamano2);
        if(new_sd1==arraysd[1])
          printf(" -Player 1: ");
        else printf(" -Player 2: ");
        stampacarta(cartamano2);
        inviacarta(arraysd[0], &cartamano2);
        printf("\n\n\nRiepilogo: \n");
        printf("  Carta ricevuta dal PRIMO: ");
        stampacarta(cartamano1);
        printf("\n  Carta ricevuta dal SECONDO: ");
        stampacarta(cartamano2);

        //calcolo il punteggio e poi vedo chi ha vinto 
        if(cartamano1.tipo<5){}
        else {
    	    if(cartamano1.tipo<8)
    	       totaleprovv=totaleprovv+cartamano1.tipo-3;
    	    else 
    	       totaleprovv=totaleprovv+cartamano1.tipo+2;
        }   
        if(cartamano2.tipo<5){}
        else {
        	if(cartamano2.tipo<8)
    	       totaleprovv=totaleprovv+cartamano2.tipo-3;
    	    else 
    	       totaleprovv=totaleprovv+cartamano2.tipo+2;
        }
        //funzione vincita che mi dice chi ha vinto, giocatore 1 o 2, basandosi su cartamano1 o cartamano2
        winner=vincita(cartamano1, cartamano2, briscola.seme);
        if (winner==0) {
            inviaintero(arraysd[0], &totaleprovv);
            inviaintero(arraysd[1], &alwayszero);
            inviastringa(arraysd[0], &messwin[0], sizeof(messwin));
            inviastringa(arraysd[1], &messlos[0], sizeof(messlos));
            printf("\nHa vinto il PRIMO "); 
            if(new_sd1==arraysd[0])
                printf(" (Player 1) ");
            else printf(" (Player 2) ");
            printf(" con %d punti. La mano successiva inizierà ancora lui \n", totaleprovv);
            totaleprovv=0; //risetto a zero il totale provvisorio
         } //ha vinto arraysd[0] e l'ordine resta invariato
        if (winner==1) {
            inviaintero(arraysd[1], &totaleprovv);
            inviaintero(arraysd[0], &alwayszero);
            inviastringa(arraysd[1], &messwin[0], sizeof(messwin));
            inviastringa(arraysd[0], &messlos[0], sizeof(messlos));
            /*inverto la posizione dei descrittori dei socket nell'array in modo da avere in posizione zero il primo che inizierà il turno successivo*/
    	      sdappoggio=arraysd[0];
    	      arraysd[0]=arraysd[1];
    	      arraysd[1]=sdappoggio; 
            printf("\nHa vinto il SECONDO ");
            if(new_sd1==arraysd[1])
                printf(" (Player 1) ");
            else printf(" (Player 2) ");
            printf(" con %d punti. La mano successiva questa volta inizierà lui \n", totaleprovv);
            totaleprovv=0; //risetto a zero il totale provvisorio
        } //ha vinto arraysd[1] e l'ordine cambia

        if (finebriscola==0){
        	for(i=0; i<2; i++){
    	       	if (mazzo!=NULL){
    			       cartatemp=eliminaCarta(&mazzo);
    		      }
    	       	else {
    			       cartatemp=briscola; 
    		      	 finebriscola=finebriscola+1; 
              }
                inviacarta(arraysd[i], &cartatemp);
        	}
        } else finebriscola=finebriscola+1; 
        printf("\n-----------------------------------------------------------------\n");
        printf("\nHo mandato le nuove carte ai giocatori.\n");
        contmani++;
  }
/*fine ciclo partita*/

  printf("\n\n-----------------------------------------------------------------\n");
  printf("-----------------------------------------------------------------\n");
  printf("-----------------------------------------------------------------\n");
  printf("\n----------------        FINE PARTITA!!      ---------------------\n\n");
  printf("-----------------------------------------------------------------\n");

//ora mi faccio mandare i due totali dei punti e li confronto
	for(cont=0; cont<2; cont++){
      recintero(arraysd[cont], &pilapunti[cont]);
  }
  if(pilapunti[1]>pilapunti[0]){
      inviastringa(arraysd[1], &messwin[0], sizeof(messwin));
      inviastringa(arraysd[0], &messlos[0], sizeof(messlos));
      if(new_sd1==arraysd[1])
        var=1;
      else var=2;
      printf("\nLa partita è stata vinta dal SECONDO giocatore (colui che nell'ultima mano ha giocato per SECONDO), ovvero il Player %d, con %d punti !!!\n\n", var, pilapunti[1]);
  } 
  if(pilapunti[0]>pilapunti[1]){
      inviastringa(arraysd[0], &messwin[0], sizeof(messwin));
      inviastringa(arraysd[1], &messlos[0], sizeof(messlos));
      if(new_sd1==arraysd[0])
        var=1;
      else var=2;
      printf("\nLa partita è stata vinta dal PRIMO giocatore (colui che nell'ultima mano ha giocato per PRIMO), ovvero il Player %d, con %d punti !!!\n\n", var, pilapunti[0]);
  }
  if(pilapunti[0]==pilapunti[1]){
      for(cont=0; cont<2; cont++){
          inviastringa(arraysd[cont], &messpari[0], sizeof(messpari));
      }
      printf("\nLa partita è stata pareggiata!!\n\n");

  }
  printf("-----------------------------------------------------------------\n\n");
  
  /*chiudo i descrittori dei socket*/
	close(arraysd[0]);
	close(arraysd[1]);
	close(sd);
	return 0; 
}
/*FINE MAIN*/