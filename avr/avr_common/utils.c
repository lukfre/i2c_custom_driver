#include"utils.h"
#include <avr/interrupt.h>
#include<stdlib.h>
#include<avr/io.h>
#include<util/delay.h>


/* Easy Blink function for DEBUGGING*/
void debug_blink(void) {
    const uint8_t mask=(1<<7);
    DDRB |= mask;
    
    PORTB = mask;
    _delay_ms(10); // from delay.h, wait 1 sec
    PORTB &= !(1<<7);
}

/* Led ON/OFF for DEBUGGING1*/
void led(uint8_t on){
    const uint8_t mask=(1<<7);
    DDRB |= mask;    
    if(on)
        PORTB = mask;
    else
        PORTB &= !(1<<7);
}

int precModule(int offset,int numCols){
    return (offset == 0) ? numCols-1 : offset -1; 
}