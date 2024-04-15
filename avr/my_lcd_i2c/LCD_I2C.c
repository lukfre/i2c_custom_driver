#include "LCD_I2C.h"
#include <util/delay.h>
#include <assert.h>
#include "../i2c/comunication.h"
#include<stdio.h>

inline uint8_t LCD_I2C_write(LCD_I2C* lcd, uint8_t value) {
	LCD_I2C_send(lcd,value, Rs);
	return 1;
}

void LCD_I2C_scrollDisplayLeft(LCD_I2C* lcd) {
	LCD_I2C_command(lcd,LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void LCD_I2C_print(LCD_I2C*lcd, char * str){
    while(*str){
        LCD_I2C_write(lcd, (uint8_t)(*str));
        str++;
    }
}

static void LCD_I2C_begin(LCD_I2C * lcd){
    if(lcd->_rows > 1){
        lcd->_displayfunction |= LCD_2LINE;
    }

    lcd->_numlines = lcd->_rows;
    /* not supporting one row lcd */

    _delay_ms(50);
    LCD_I2C_expanderWrite(lcd, lcd->_backlightval);
    _delay_ms(1000);

    LCD_I2C_write4bits(lcd, 0x03 <<4);
    _delay_ms(5);

    LCD_I2C_write4bits(lcd, 0x03 <<4);
    _delay_ms(5);

    LCD_I2C_write4bits(lcd, 0x03 <<4);
    _delay_ms(2);

    LCD_I2C_write4bits(lcd, 0x02 <<4);

    LCD_I2C_command(lcd, LCD_FUNCTIONSET | lcd->_displayfunction);

    lcd->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    LCD_I2C_display(lcd);
    LCD_I2C_clear(lcd);

    lcd->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    LCD_I2C_command(lcd, LCD_ENTRYMODESET | lcd->_displaymode);
    LCD_I2C_home(lcd);
}

/* HIGH LEVEL COMMANDS: for user */
void LCD_I2C_init(LCD_I2C* lcd, uint8_t addr, uint8_t cols, uint8_t rows){
    com_init();
    lcd->_addr = addr;
    lcd->_cols = cols;
    lcd->_rows = rows;
    lcd->_backlightval = LCD_BACKLIGHT;   

    lcd->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x10DOTS;
    LCD_I2C_begin(lcd);
    //printf("[DEBUG] LCD_I2C_init exited\n");
}

void LCD_I2C_clear(LCD_I2C*lcd){
    LCD_I2C_command(lcd, LCD_CLEARDISPLAY);
    _delay_ms(2); //TODO: check time 2000 microseconds.
}
void LCD_I2C_display(LCD_I2C* lcd){
    lcd->_displaycontrol |= LCD_DISPLAYON;
    LCD_I2C_command(lcd, LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}

void LCD_I2C_backlight(LCD_I2C*lcd){
    lcd->_backlightval = LCD_BACKLIGHT;
    LCD_I2C_expanderWrite(lcd, 0);
}
void LCD_I2C_setCursor(LCD_I2C* lcd, uint8_t col, uint8_t row){
    int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if(row > lcd->_numlines){
        row = 3;
    }
    LCD_I2C_command(lcd, LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void LCD_I2C_home(LCD_I2C *lcd){
	LCD_I2C_command(lcd, LCD_RETURNHOME);  // set cursor position to zero
	_delay_us(2000); //TODO: check time 2000 microseconds.
}

/*********** mid level commands, for sending data/cmds */

inline void LCD_I2C_command(LCD_I2C* lcd, uint8_t value){
    LCD_I2C_send(lcd, value, 0);
}

/************ low level data pushing commands **********/

void LCD_I2C_send(LCD_I2C*lcd, uint8_t value, uint8_t mode) {
	uint8_t highnib=value&0xF0;
	uint8_t lownib=(value<<4)&0xF0;
    LCD_I2C_write4bits(lcd, (highnib)|mode);
	LCD_I2C_write4bits(lcd, (lownib)|mode); 
}

void LCD_I2C_write4bits(LCD_I2C* lcd, uint8_t value){
    LCD_I2C_expanderWrite(lcd,value);
    LCD_I2C_pulseEnable(lcd,value);
}
void LCD_I2C_expanderWrite(LCD_I2C* lcd, uint8_t _data){                                        
	com_beginTrasmission(lcd->_addr);
	com_write((int)(_data) | lcd->_backlightval);
	com_endTrasmission(1);
}

void LCD_I2C_pulseEnable(LCD_I2C* lcd, uint8_t _data){
	LCD_I2C_expanderWrite(lcd, _data | En);	// En high
	//delayMicroseconds(1);		// enable pulse must be >450ns
	_delay_us(1); //TODO:check time
	LCD_I2C_expanderWrite(lcd, _data & ~En);	// En low
	//delayMicroseconds(50);		// commands need > 37us to settle
    _delay_us(50); //TODO check time
}

void LCD_I2C_createChar(LCD_I2C*lcd, uint8_t location, const uint8_t * charmap) {
	location &= 0x7; // we only have 8 locations 0-7
	LCD_I2C_command(lcd,LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		LCD_I2C_write(lcd,charmap[i]);
	}
}