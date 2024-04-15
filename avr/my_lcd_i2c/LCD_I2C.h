#pragma once
#include <inttypes.h>

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0x04 //B00000100  // Enable bit
#define Rw 0x02 //B00000010  // Read/Write bit
#define Rs 0x01 //1B00000001  // Register select bit

typedef struct LCD_I2C {
    uint8_t _addr;
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _numlines;
    uint8_t _cols;
    uint8_t _rows;
    uint8_t _backlightval;
} LCD_I2C;

void LCD_I2C_init(LCD_I2C* , uint8_t add, uint8_t cols, uint8_t rows);
void LCD_I2C_backlight(LCD_I2C*);
void LCD_I2C_send(LCD_I2C* , uint8_t, uint8_t);
void LCD_I2C_clear(LCD_I2C *);
uint8_t LCD_I2C_write(LCD_I2C *, uint8_t);



void LCD_I2C_command(LCD_I2C*, uint8_t);
void LCD_I2C_pulseEnable(LCD_I2C*, uint8_t);
void LCD_I2C_expanderWrite(LCD_I2C*, uint8_t);
void LCD_I2C_write4bits(LCD_I2C*, uint8_t);
void LCD_I2C_display(LCD_I2C*);
void LCD_I2C_home(LCD_I2C*);

void LCD_I2C_print(LCD_I2C* , char *);
void LCD_I2C_createChar(LCD_I2C *, uint8_t , const uint8_t *);
void LCD_I2C_setCursor(LCD_I2C* lcd, uint8_t col, uint8_t row);
void LCD_I2C_scrollDisplayLeft(LCD_I2C* lcd);