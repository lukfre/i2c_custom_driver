#ifndef _GAME_H
#define _GAME_H

// #include <sys/types.h>      // for uint8_t type
#include "../../my_lcd_i2c/LCD_I2C.h"


#define BUFFER_SIZE 32
#define LCD_ADDRESS 0x27
#define LCD_WIDTH 16
#define LCD_HEIGHT 2
#define LCD_WIDTH_BUFFER 24
#define SPRITE_BLOCK_EMPTY ' '
#define SPRITE_RUN1 0   //Identify index from graphics array
#define SPRITE_RUN2 1
#define SPRITE_ENEMY 2
#define SPRITE_SPOINT 3
#define SPRITE_BPOINT 4

#define LCD_COLS LCD_WIDTH + LCD_WIDTH_BUFFER
#define AVATARCOL 3
#define SIMBOLOSTACLE 2
#define MONEY 3
//const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

typedef struct game{
	char world[LCD_HEIGHT][LCD_COLS];
	uint8_t difficult;
	char nickname[BUFFER_SIZE];
	uint8_t avatarImage;
  uint8_t avatarPositionRow;
  uint8_t avatarPositionRowPrec;
  uint8_t avatarPositionCol;
	uint32_t score;
  uint8_t level;
} game;

static uint8_t  graphics[] = {
    // Run position 1 (Bocca aperta)
    0b00000,
    0b01110,
    0b11111,
    0b11100,
    0b11000,
    0b11100,
    0b11111,
    0b01110,
    // Run position 2 (Bocca chiusa)
    0b00000,
    0b01110,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    //Enemy
    0b00000,
    0b01110,
    0b11111,
    0b10101,
    0b11111,
    0b10001,
    0b11111,
    0b10001,
    //Small point
    0b00000,
    0b00000,
    0b00000,
    0b00100,
    0b01110,
    0b00100,
    0b00000,
    0b00000,
    //Big point
    0b00000,
    0b00000,
    0b00100,
    0b01110,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    // Run position 1 (Bocca aperta-invincibile)
    0b11111,
    0b10001,
    0b00000,
    0b00011,
    0b00111,
    0b00011,
    0b00000,
    0b10001,
    // Run position 2 (Bocca chiusa-invincibile)
    0b11111,
    0b10001,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b10001
  };



void initializeStruct(game* myGame);
void initialize(LCD_I2C* lcd, game* myGame);
void renderize(LCD_I2C* lcd,game* myGame);
void splashScreen(LCD_I2C* lcd,game* myGame);
void step(uint8_t newBlock,game* myGame);
void quitScreen(LCD_I2C* lcd,game* myGame);
void gameScreen(LCD_I2C* lcd,game* myGame);
void statusScreen(LCD_I2C*,game*);
void howScreen(LCD_I2C*,game*);
void byeScreen(LCD_I2C*,game*);
#endif