#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../i2c/comunication.h"
#include "../my_lcd_i2c/LCD_I2C.h"
#include "../avr_common/uart.h" // this includes the printf and initializes it


int main(void){
    uint8_t i;
    printf_init(); 
    sei();
    printf("myAVR loaded \n");
    int8_t ret = 0;
    //i2c_init();
    //uint8_t buf = {0x03 <<4};
    //uint8_t ret = i2c_writeTo(0x27,&buf, 1, 1,0);
    //LCD_I2C_init(&lcd,0x27,16,2);

    uint8_t add [] = {0x27,0x26};
    uint8_t rows[] = {2,4};
    uint8_t cols[] = {16,20};
    LCD_I2C lcd[2];
 
    i=0;
    int j;
    for(j=0;j<2;j++){
        printf("lcd_init(%d,%d,%d)\n",add[j],cols[j],rows[j]);
        LCD_I2C_init(&lcd[j],add[j],cols[j],rows[j]);
        LCD_I2C_backlight(&lcd[j]);
        _delay_ms(2000);
        LCD_I2C_clear(&lcd[j]);
        LCD_I2C_setCursor(&lcd[j], 0,0);

    }

    char  c = 'A';
    for(i=0;i<50;i++){
        LCD_I2C_write(&lcd[0],c+i);
        LCD_I2C_write(&lcd[1],c+i);

        if(c+i == 'h')
            printf("h = %d A = %d, (=%d)", (int) c+i, (int) c, (int)i);
    }

    while(1){
        LCD_I2C_scrollDisplayLeft(&lcd[0]);
        _delay_ms(500);
    }
    return ret;
}
