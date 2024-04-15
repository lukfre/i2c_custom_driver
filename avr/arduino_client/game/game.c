#include "game.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "../../avr_common/utils.h"

#define TYPE_INIZIALIZE 2

void initialize(LCD_I2C* lcd, game * myGame){
  initializeStruct(myGame);
  // set up the LCD's number of columns and rows lcd.begin(cols,rows):
  LCD_I2C_init(lcd, LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);
  LCD_I2C_backlight(lcd);

  //Create special char , before initialized on graphics
  int i,j;
  for (i = 0; i < 5; ++i) {
    LCD_I2C_createChar(lcd, i, (graphics + i*8));
  }

  #if (TYPE_INIZIALIZE == 0 )
   for(i=0;i<LCD_HEIGHT; i++){
    for(j=0;j<LCD_COLS;j++){
      myGame->world[i][j] = 'A'+j;
    }
  } 
  #elif(TYPE_INIZIALIZE == 1)
  for (j = 0; j < LCD_COLS; ++j){
        
        if(j >= AVATARCOL && j <= (AVATARCOL + 6 - myGame->difficult)){
            myGame->world[myGame->avatarPositionRow][j] = ' ';
            myGame->world[0][j] = ' ';
            myGame->world[1][j] = ' ';
            continue;
        }
        int val = (rand()%10 < 7) ? ' ': SIMBOLOSTACLE;
        myGame->world[0][j] = val;
        if(val == SIMBOLOSTACLE){
          myGame->world[1][j] = ' ';
          j++;
          myGame->world[0][j] = ' ';
          myGame->world[1][j] = ' ';
        }else{
          val = (rand()%10 < 7) ? ' ': SIMBOLOSTACLE;
          myGame->world[1][j] = val;
          if(val == SIMBOLOSTACLE) {
            j++;
            myGame->world[0][j] = ' ';
            myGame->world[1][j] = ' ';
          }
        }    
  }
  #elif (TYPE_INIZIALIZE == 2)
     for(i=0;i<LCD_HEIGHT; i++){
      for(j=0;j<LCD_COLS;j++){
        myGame->world[i][j] = SPRITE_BLOCK_EMPTY;
      }
    } 
    uint8_t row = 0;

  for(i=7;i<LCD_COLS;i++){
    myGame->world[row][i++] = SIMBOLOSTACLE;
    if(row > 0 )
      row = 0;
    else
      row = 1;
  }

  #else
  for (j = 0; j < LCD_COLS-1; ++j){
    uint8_t currentPos = j;
    uint8_t precPos = precModule(currentPos, LCD_COLS);
    uint8_t nextPos = (currentPos+1)%LCD_COLS;
      myGame->world[0][j] = SPRITE_BLOCK_EMPTY;
      myGame->world[1][j] = SPRITE_BLOCK_EMPTY;

    /* for the first, don't insert obstacles */
    if(j >= AVATARCOL && j <= (AVATARCOL + 6 - myGame->difficult)){
      continue;
    }
    //1 caso:
    /* xN
       00
     */
    if(myGame->world[0][precPos] == SIMBOLOSTACLE && myGame->world[1][precPos] == SPRITE_BLOCK_EMPTY){
        if(rand()%100 < 40){
          myGame->world[0][currentPos] = SIMBOLOSTACLE;
        }else {
            myGame->world[1][nextPos] = SIMBOLOSTACLE;
        }
    }
    /* 00
       XN
     */
    else if(myGame->world[1][precPos] == SIMBOLOSTACLE && myGame->world[0][precPos] == SPRITE_BLOCK_EMPTY){
        if(rand()%100 < 50){
          myGame->world[1][j] = SIMBOLOSTACLE;
        }else{
          myGame->world[0][nextPos] = SIMBOLOSTACLE;
        }
    }else
    /* 0N
       0N
     */
    if(myGame->world[1][precPos] == SPRITE_BLOCK_EMPTY && myGame->world[0][precPos] == SPRITE_BLOCK_EMPTY){
        if(rand()%10 < 4){
          myGame->world[1][j] = SIMBOLOSTACLE;
        }else if(rand()%10 < 4){
          myGame->world[0][j] = SIMBOLOSTACLE;
        }
    }
  }

  #endif
}



void splashScreen(LCD_I2C* lcd, game * myGame){
  int i;
  LCD_I2C_setCursor(lcd,0,0);
  LCD_I2C_write(lcd,0);
  LCD_I2C_print(lcd,"I2c VideoGame ");
  LCD_I2C_write(lcd,0);
  LCD_I2C_setCursor(lcd,0,1);
  for(i=0;i<16;i++){ 
    LCD_I2C_write(lcd,2);
  }
}
void quitScreen(LCD_I2C* lcd,game* myGame){
  LCD_I2C_clear(lcd);
  char buffer[16];
  memset(&buffer, 0, sizeof(buffer)); // zero out the buffer    
  sprintf(buffer, "Good %s!", myGame->nickname);
  LCD_I2C_setCursor(lcd,0,0);
  LCD_I2C_print(lcd,buffer);
  sprintf(buffer, "Your score: %d",(int)myGame->score);
  LCD_I2C_setCursor(lcd,0,1);
  LCD_I2C_print(lcd,buffer);
}

void statusScreen(LCD_I2C * lcdBig, game* gamePtr){
  char tmp[32];
  LCD_I2C_clear(lcdBig);
  memset(tmp, 0, sizeof(tmp)); 
  LCD_I2C_setCursor(lcdBig, 0,0);
  sprintf(tmp, "Score: %3d", (int)gamePtr->score);
  LCD_I2C_print(lcdBig, tmp);
  memset(tmp, 0, sizeof(tmp));    
  LCD_I2C_setCursor(lcdBig, 0,1);
  sprintf(tmp, "Col: %2d Row: %2d", (int)gamePtr->avatarPositionCol, (int)gamePtr->avatarPositionRow);
  LCD_I2C_print(lcdBig, tmp);
  memset(tmp, 0, sizeof(tmp));     
  LCD_I2C_setCursor(lcdBig, 0,2);
  sprintf(tmp, "Level: %2d", (int)gamePtr->level);
  LCD_I2C_print(lcdBig, tmp);
}

void howScreen(LCD_I2C* lcd,game* myGame){
  LCD_I2C_clear(lcd);
  char buffer[16];
  memset(&buffer, 0, sizeof(buffer)); // zero out the buffer    
  sprintf(buffer, "Avoid: %c", SIMBOLOSTACLE);
  LCD_I2C_setCursor(lcd,0,0);
  LCD_I2C_print(lcd,buffer);
  sprintf(buffer, "Capture: %c",MONEY);
  LCD_I2C_setCursor(lcd,0,1);
  LCD_I2C_print(lcd,buffer);
}

void byeScreen(LCD_I2C* lcd, game* myGame){
  LCD_I2C_clear(lcd);
  char buffer[16];
  memset(&buffer, 0, sizeof(buffer)); // zero out the buffer    
  sprintf(buffer, "Bye! Rerun from ");
  LCD_I2C_setCursor(lcd,0,0);
  LCD_I2C_print(lcd,buffer);
  sprintf(buffer, "PC to play again");
  LCD_I2C_setCursor(lcd,0,1);
  LCD_I2C_print(lcd,buffer);
}

void initializeStruct(game* myGame){
  myGame->difficult = 0;
  myGame->avatarImage = 0;
  myGame->avatarPositionRow = 0;
  myGame->avatarPositionRowPrec = 0;
  myGame->avatarPositionCol = AVATARCOL;
  myGame->score = 0;
  myGame->level = 0;
}

void renderize(LCD_I2C* lcd, game * myGame){
  int i,j;
  for(i=0;i<LCD_HEIGHT;i++){
    LCD_I2C_setCursor(lcd,0,i);
    for(j=0;j<LCD_COLS;j++){
        LCD_I2C_write(lcd,myGame->world[i][j]);
    }
  }
}
