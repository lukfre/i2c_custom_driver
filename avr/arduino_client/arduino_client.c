#include "arduino_client.h"
/*STD INCLUDES */
#include <avr/interrupt.h>
#include<stdlib.h>
#include<string.h>

/*CUSTOM INCLUDES */
#include "../avr_common/utils.h"
#include "../avr_common/uart.h"

#include "../my_lcd_i2c/LCD_I2C.h"

#include "game/game.h"

LCD_I2C lcd;
LCD_I2C lcdBig;
game myGame;
game* gamePtr = &myGame;
uint8_t secondScreen = 1;
int flag = 1;

void setupAVR(void){
    printf_init();
    sei();
    LCD_I2C_init(&lcd,LCD_ADDRESS,LCD_WIDTH,LCD_HEIGHT);
    LCD_I2C_backlight(&lcd);
    srand(0xDEAD);
    if(secondScreen){
        LCD_I2C_init(&lcdBig,0x26,20,4);
        LCD_I2C_backlight(&lcdBig);
    }
}
void printScreen(LCD_I2C* lcd, char* str, int reset){
    LCD_I2C_clear(lcd);
    if(reset) LCD_I2C_setCursor(lcd,0,0);
    LCD_I2C_print(lcd,str);
}

void printDebug(LCD_I2C* lcd, char* str){
    LCD_I2C_setCursor(lcd,0,3);
    LCD_I2C_print(lcd,str);
}
void print_game(void) {
    // la print sarebbe solo su seriale, non c'é diff tra stderr e stdout
    printf("game_diff-> %d, nick-> %s", gamePtr->difficult, gamePtr->nickname);
}

void client_start(uint8_t* buf) {
    // attendi START e leggi
    do {
        UART_getString(buf);
        debug_blink();
    } while(strcmp((char*)buf, "start\n") != 0);

    printf("%s", (char*)buf);
    return;
}

void game_setup(uint8_t* buf,LCD_I2C* lcd) {
    // nickname
    UART_getString(buf);
    // salva in struct game il nome giocatore
    strcpy(gamePtr->nickname, (char*)buf);
    printf("%s", (char*)buf);

    printScreen(lcd, "Not choose NARDI...",1);
    _delay_ms(500);
    UART_getString(buf);

    if(strcmp((char*)buf, "MIRANDA\n") == 0) {
    	gamePtr->difficult= MIRANDA;
    	printf("0\n");
	
    } else if(strcmp((char*)buf, "CUOMO\n") == 0) {
    	gamePtr->difficult= CUOMO;
    	printf("1\n");

    } else if(strcmp((char*)buf, "DAMORE\n") == 0) {
    	gamePtr->difficult= DAMORE;
    	printf("2\n");
	
    } else if(strcmp((char*)buf, "GRISETTI\n") == 0) {
    	gamePtr->difficult= GRISETTI;
    	printf("3\n");
       
    } else {
    	gamePtr->difficult= NARDI;
    	printf("4\n");
    }
    //print_game();

    debug_blink();
    return;
}

/*This is core of Game: read this to understanding game*/
void run_game(uint8_t* buf) {
    
    gamePtr->avatarPositionCol = AVATARCOL;
    renderize(&lcd, gamePtr);
    while(1) {
        led(1);
    	int8_t res = UART_getString_unblocking(buf,gamePtr->avatarPositionRow%15);
        led(0);
        
        gamePtr->avatarPositionRowPrec = gamePtr->avatarPositionRow;
        //Se nessun carattere è stato letto, non viene spostato l'omino.
        if(res == 0) {
    	    // no move
    	    // one step forward, with same position
    	}else if(strncmp((char*)buf, "w\n", 2) == 0) {
            if(gamePtr->avatarPositionRow > 0){
                gamePtr->avatarPositionRow =  (gamePtr->avatarPositionRow -1);
            }
            printf("W\n");
            // move up

    	} else if(strncmp((char*)buf, "s\n",2) == 0) {
            // move down
            printf("S\n");
            if(gamePtr->avatarPositionRow < LCD_HEIGHT-1){
                gamePtr->avatarPositionRow =  (gamePtr->avatarPositionRow +1);
            }

    	} else if(strncmp((char*)buf, "q\n",2) == 0) {
    	    // gestire il gameover
            //printf("Q\n");
            //printf("%d\n",(int) gamePtr->score);
            //quitScreen(&lcd,gamePtr);
            //_delay_ms(5000);
    	    //break;
            printf("M\n");
            printf("%d\n", (int) gamePtr->score);
            quitScreen(&lcd,gamePtr);
            //cli();
            //_delay_ms(5000);
            UART_getString(buf);
            if (buf[0] == 'y' || buf[0] == 'Y'){
                flag = 1;
                printf("1\n");
            }
            else {
                flag = 0;
                printf("0\n");
            }
            return;
    
    	} else {
            /*NEVER ENTER THERE: ONLY DEBUG */
            _delay_ms(500);
            debug_blink();
            _delay_ms(500);
            debug_blink();
            _delay_ms(500);
            debug_blink();
    	}
    	// controllo gameover
    	// one step forward
        //step
        LCD_I2C_scrollDisplayLeft(&lcd);

        /*VERY TRICKY 
        Poichè LCD FA SCROLLING AUTOMATICO, ogni colonna cambia posizione
        durante lo scroll. 
        Ed essendo modulare, con delle colonne invisibili sullo schermo,
        ogni tot. (39 la colonna torna ad essere la stessa)
        */
        
        
       /*Aggiunta Obiettivi da prendere */
        if((rand() % 10) < 2){
            LCD_I2C_setCursor(&lcd,(gamePtr->avatarPositionCol+15)%40,gamePtr->avatarPositionRow);
            LCD_I2C_write(&lcd,MONEY);
            gamePtr->world[gamePtr->avatarPositionRow][(gamePtr->avatarPositionCol+15)%40] = MONEY;
            
        }

        if(rand() %10 < gamePtr->level + 7){
            uint8_t currentPos = (gamePtr->avatarPositionCol+15)%LCD_COLS;
            uint8_t precPos = precModule(currentPos, LCD_COLS);
            uint8_t nextPos = (currentPos+1)%LCD_COLS;
            uint8_t row = (rand()%10 < 5) ? 1 : 0 ;
            if(gamePtr->world[0][precPos] == SPRITE_BLOCK_EMPTY && gamePtr->world[1][precPos] == SPRITE_BLOCK_EMPTY &&
                gamePtr->world[0][nextPos] == SPRITE_BLOCK_EMPTY && gamePtr->world[1][nextPos] == SPRITE_BLOCK_EMPTY &&
                gamePtr->world[0][currentPos] == SPRITE_BLOCK_EMPTY && gamePtr->world[1][currentPos] == SPRITE_BLOCK_EMPTY){
                    LCD_I2C_setCursor(&lcd, currentPos, row);
                    LCD_I2C_write(&lcd,SIMBOLOSTACLE);
                    gamePtr->world[row][currentPos] = SIMBOLOSTACLE;
                    if(secondScreen)
                        LCD_I2C_setCursor(&lcdBig, 0,3);
                        LCD_I2C_print(&lcdBig, "New obstacles!");
                }
        }
        /*Aggiunta ostacoli */
        /*Poichè lo SCROLL è automatico per questioni di tempo, 
        ad ogni iterazione bisogna aggiungere manualmente l'omino
        alla posizione AVATARCOL (3),
        Altrimenti shifterebbe sempre a SX
        */

        //Aggiunto cambio sprite durante lo auto-scroll switch tra bocca aperte e chiusa
        if(gamePtr->avatarImage == SPRITE_RUN1) {gamePtr->avatarImage = SPRITE_RUN2;}
        else if(gamePtr->avatarImage == SPRITE_RUN2) {gamePtr->avatarImage = SPRITE_RUN1;}

        LCD_I2C_setCursor(&lcd,gamePtr->avatarPositionCol,gamePtr->avatarPositionRow);
        LCD_I2C_write(&lcd,gamePtr->avatarImage);

        //riscrive nella casella prima dell'omino il valore corretto. (Altrimenti si duplicano gli omini)
        LCD_I2C_setCursor(&lcd,precModule(gamePtr->avatarPositionCol,LCD_COLS),gamePtr->avatarPositionRowPrec);
        LCD_I2C_write(&lcd,gamePtr->world[gamePtr->avatarPositionRowPrec][precModule(gamePtr->avatarPositionCol, LCD_COLS)]);

        //Check collisione 
        if(gamePtr->world[gamePtr->avatarPositionRow][gamePtr->avatarPositionCol] == SIMBOLOSTACLE){
            
            //led(1);
            printf("M\n");
            printf("%d\n", (int) gamePtr->score);
            quitScreen(&lcd,gamePtr);
    	    //cli();
            //_delay_ms(5000);
            UART_getString(buf);    //stringa del getchar, inevitabile
            UART_getString(buf);
            if (buf[0] == 'y' || buf[0] == 'Y'){
                flag = 1;
                printf("1\n");
            }
            else {
                flag = 0;
                printf("0\n");
            }
            return;
        }else{
            printf("L\n");
            //ADD BONUS
            if(gamePtr->world[gamePtr->avatarPositionRow][gamePtr->avatarPositionCol] == MONEY){
                //reset moneta catturata
                gamePtr->world[gamePtr->avatarPositionRow][gamePtr->avatarPositionCol] = SPRITE_BLOCK_EMPTY;
                gamePtr->score += (gamePtr->difficult+1)*50;    
            }
        }

        if(gamePtr->avatarPositionCol == 0){
            gamePtr->level++;
        }
        gamePtr->avatarPositionCol = (gamePtr->avatarPositionCol+1)%40;
        gamePtr->score += gamePtr->difficult*1;
        
        if(secondScreen){
            statusScreen(&lcdBig, gamePtr);
        }

        debug_blink();
        //_delay_ms(1000);
    }
}

int main(void) {
    setupAVR();
    initialize(&lcd, gamePtr);
    
    uint8_t buf[MAX_BUF];
    uint8_t buffone[MAX_BUF];
    splashScreen(&lcd, gamePtr);
    //UART_getString(buf);
    _delay_ms(500);
    while(1){
        printf("R\n");
        UART_getString_unblocking(buf, 2);  //aspetta al massimo 1 secondo la risposta
        if (buf[0] == 'R')
            break;
    }

    while(flag) {
        initialize(&lcd, gamePtr);
        printScreen(&lcd,"Write start",1);
        client_start(buf);

        printScreen(&lcd,"Setting up ",1);
	    game_setup(buffone, &lcd);
        
        printScreen(&lcd,gamePtr->nickname,1);
        _delay_ms(1000);
        printScreen(&lcd,"Get ready...",1);
        _delay_ms(1000);
        howScreen(&lcd, gamePtr);
        _delay_ms(2000);
	    run_game(buffone);
    }
    byeScreen(&lcd, gamePtr);
    _delay_ms(5000);
    LCD_I2C_clear(&lcd);
    return 0;
}
