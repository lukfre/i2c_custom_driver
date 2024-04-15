#include "uart.h"

void UART_init(void){
  // Set baud rate
  UBRR0H = (uint8_t)(MYUBRR>>8);
  UBRR0L = (uint8_t)MYUBRR;

  UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);               /* 8-bit data */ 
  UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);   /* Enable RX and TX */  
}

void UART_putChar(uint8_t c){
  while ( !(UCSR0A & (1<<UDRE0)) );   // wait for transmission completed, looping on status bit
  UDR0 = c;                           // Start transmission
}

uint8_t UART_getChar(void){
  while ( !(UCSR0A & (1<<RXC0)) );    // Wait for incoming data, looping on status bit
  return UDR0;                        // Return the data 
}

uint8_t UART_getString(uint8_t* buf){	// reads a string until the first newline or 0
  uint8_t* b0 = buf; //beginning of buffer
  while(1){
    uint8_t c = UART_getChar();
    *buf = c;
    ++buf;
    // reading a 0 terminates the string
    if (c == 0)
      return buf-b0;
    // reading a \n  or a \r return results
    // in forcedly terminating the string
    if(c == '\n' || c == '\r'){
      *buf=0;
      ++buf;
      return buf-b0;
    }
  }
}

void UART_putString(uint8_t* buf){
  while(*buf){
    UART_putChar(*buf);
    ++buf;
  }
}

// echo server per testare
int main(void) {
    UART_init();
    //while(1){
    //  char c = UART_getChar();
    //  UART_putChar(c);
    //}

    uint8_t buf[MAX_BUF];
    while (1){
      UART_getString(buf);
      UART_putString(buf);
      //_delay_ms(1000); // from delay.h, wait 1 sec
    }
    return 0;
}
