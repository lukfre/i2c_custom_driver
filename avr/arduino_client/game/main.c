/***
 * Inizializza lcd, crea uart e aspetta da uart
 * printa su LCD: Premi START per inviare (da tastiera)
 * 
 */
void setupAVR(){
	
	
}
/***
 *
 * 
 **/
void setupPartita(){
    //printUART("Inserisci nickname");
    //readUARTBloccante(&nickname);
    //printUART("Inserisci difficoltà");
    //0 - Miranda
    //1 - Cuomo
    //2 - D'Amore
    //3 - Grisetti
    //readUARTBloccante(&difficult)
    //invia ACK partita pronta
}
/***
 * Legge la struttura dati Game e printa su LCD 
 * 
 ***/
void renderGrafica(){
    scrollDisplayLeft(&game,TODO RANDOM VALUE){
	//aggiorna i caratteri che sono shiftati male
    }

/***
 * 
 * 
 **/
 
void step(){
	
	
}

/***
 * Crea le strutture dati del gioco, inizia
 * e rimane in attesa dei comandi dalla UART
 * 
 ***/
int avviaGioco(){
    //setup
    int colliso = 0;
	game = initialize(); //La struct che contiene tutte le info sulla partita corrente 
    while(1){
	    renderize(&game);
	    if(colliso){
	        break;
	    }
	    //letturaNonBloccanteMossa(Su/Giu)
	    if(letto){
	        //modificaStruttura
				
	    }
	    step();
	    colliso = checkCollisione();

	    delay();	//in caso mettere dopo renderGrafica()
    }
		
    //printLCD GAME OVER
    //printLCD SCORE
    //printUART Sei scarso, Ritenta
    //printUART SCORE
}

int main(){

    setupAVR();
	LCD_I2C lcd;
	LCD_I2C_init(&lcd, 0x27, 16, 2);

	
    while(1){
	//leggi start da uart BLOCCANTE
	setupPartita();
	//countDown-> 3,2,1 VIA
	avviaGioco();
		
    }
}
