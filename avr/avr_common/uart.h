#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include "../../avr/my_lcd_i2c/LCD_I2C.h"

#define BAUD 19200
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUF 1024
#define MAX_TEMP 20

void UART_init(uint16_t myubrr);
void UART_putChar(uint8_t c);
uint8_t UART_getChar(void);
uint8_t UART_getChar_unblocking(uint32_t);


uint8_t UART_getString(uint8_t* buf);
uint8_t UART_getString_unblocking(uint8_t* buf, uint32_t);
void UART_putString(uint8_t* buf);
int usart_putchar_printf(char var, FILE *stream);
void printf_init(void);
