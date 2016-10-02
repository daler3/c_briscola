/*
---------------------------------------------------------------------------------------
- Progetto Software - 
Realizzazione del gioco "Briscola" 1 vs 1 in rete locale, tramite l'utilizzo dei socket
---------------------------------------------------------------------------------------
Autore: Daniele Romanini
---------------------------------------------------------------------------------------
*/

/*definisco i tipi per la struttura carta*/
typedef enum {DENARI, SPADE, BASTONI, COPPE} semecarta;
typedef enum {DUE, QUATTRO, CINQUE, SEI, SETTE, FANTE, CAVALLO, RE, TRE, ASSO} tipocarta;
typedef struct {
  semecarta seme;
  tipocarta tipo;
}carta; 

/* definisco il tipo Boolean */
typedef enum {false, true} boolean; 

/*definizione di lista*/
typedef struct puntLista {
  carta cartacorrente;
  struct puntLista *prox; 
} cardstruct; 
typedef cardstruct *listaDiCarte;  

/*definizione del tipo per la lunghezza dell'indirizzo socket*/
typedef uint32_t socklen_t;

/*funzioni di liste generiche*/
void inizializzalist(listaDiCarte* ); 
void insInTesta(listaDiCarte* , carta );
void stampaLista(listaDiCarte* );
boolean ListaVuota(cardstruct* );

/*inizializzazione di array*/
void inizializza(char[], int );
/*funzioni per gestire le carte e il mazzo*/
carta eliminaCarta (listaDiCarte* );
void eliminaCartaSpecifica (listaDiCarte* , carta);
void riempimentomazzo (listaDiCarte* );
void mescolamazzo(listaDiCarte* );
void stampamazzo(listaDiCarte* );
void stampacarta(carta );
void mescolamazzoarray(carta *);
void riempiemescola (listaDiCarte *); 
/*funzioni per gestire la scelta delle carte*/
int scegliere ();
int choice(int );
/*funzione per decretare il vincitore di una mano*/
int vincita (carta, carta, semecarta );

/*funzione per inizializzare la struttura indirizzo*/
void adrr_initialize(struct sockaddr_in* , int , long ); 

/*funzioni per gestire i socket: invio e ricezione*/
void inviaintero(int , int* );
void recintero(int , int* );
void inviacarattere(int , char* );
void reccarattere(int , char* );
void inviacarta(int , carta* );
void reccarta(int , carta* );
void inviastringa(int , char* , int );
void recstringa(int , char* , int );