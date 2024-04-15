#include "uart.h"
#include <avr/interrupt.h>
#include <string.h>

void UART_init(uint16_t myubrr){
    // Set baud rate
    UBRR0H = (uint8_t)(myubrr>>8);
    UBRR0L = (uint8_t)myubrr;

    UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);               /* 8-bit data */ 
    UCSR0B = (1<<RXEN0) | (1<<TXEN0); // | (1<<RXCIE0);   /* Enable RX and TX */  
	/*RXCIE0: RX Complete Interrupt Enable. Set to allow receive complete interrupts. */
}

void UART_putChar(uint8_t c){
    while ( !(UCSR0A & (1<<UDRE0)) );   // wait for transmission completed, looping on status bit
    UDR0 = c;                           // Start transmission
}

uint8_t UART_getChar(void){
    while (!(UCSR0A & (1<<RXC0)));    // Wait for incoming data, looping on status bit
    return UDR0;                        // Return the data 
}

// reads a string until the first newline or 0
// returns the size read
uint8_t UART_getString(uint8_t* buf){
    //fprintf(stderr, "[DEBUG] %s", (char*)buf);
    uint8_t* b0 = buf; //beginning of bufferr
    while(1){
	uint8_t c = UART_getChar();
	*buf = c;
	++buf;
	// reading a 0 terminates the string
	if (c == 0) {
	    // fprintf(stderr, "[DEBUG 1] %s", buf);
	    return buf-b0;
	}
	// reading a \n  or a \r return results
	// in forcedly terminating the string
	if(c == '\n' || c == '\r'){
	    *buf=0;
	    ++buf;
	    // fprintf(stderr, "[DEBUG 2] %s", buf);
	    return buf-b0;
	}
    }
}

uint8_t UART_getChar_unblocking(uint32_t time){

    uint8_t i= 0;
    do {
		i++;
		_delay_ms(MAX_TEMP);
		// if(returnOnArrive && !(UCSR0A & (1<<RXC0))) break;
    } while (i< MAX_TEMP-(time*2));    // Wait for incoming data, non blocking

	if((UCSR0A & (1<<RXC0)))
		return UDR0;
    else
		return 0;                        // Return the data 
}

uint8_t UART_getString_unblocking(uint8_t* buf, uint32_t time){
	memset(buf, 0, MAX_BUF);
    uint8_t* initBuffer = buf; //beginning of bufferr

	uint8_t c = UART_getChar_unblocking(time);
	if(c == 0) return 0;
	int i = 1;
    do{
		*buf = c;
		++buf;
		// reading a 0 terminates the string
		if (c == 0) {
			// fprintf(stderr, "[DEBUG 1] %s", buf);
			return buf-initBuffer;
		}
		// reading a \n  or a \r return results
		// in forcedly terminating the string
		if(c == '\n' || c == '\r'){
			*buf=0;
			++buf;
			// fprintf(stderr, "[DEBUG 2] %s", buf);
			return buf-initBuffer;
		}
		c = UART_getChar();
		i++;
    }while(i);

	return 0;
}

void UART_putString(uint8_t* buf){
    while(*buf){
	UART_putChar(*buf);
	++buf;
    }
}

/* PRINTF serialexs */
int usart_putchar_printf(char var, FILE *stream) {
    // translate \n to \r for br@y++ terminal
    if (var == '\n')
	UART_putChar('\r');
    UART_putChar(var);
    return 0;
}

// redirige gli stream strderr e stdout sul file della seriale di arduino, in scrittura 
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

void printf_init(void){
    stdout = &mystdout;
    stderr = &mystdout;
    // fire up the usart
    UART_init(MYUBRR);
}
