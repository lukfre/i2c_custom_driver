#include "../my_lcd_i2c/LCD_I2C.h"
#include <stdint.h>
// livelli difficoltà
#define MIRANDA    0
#define CUOMO      1 
#define DAMORE     2
#define GRISETTI   3
#define NARDI      4

void print_game(void);    // DA IMPLEMENTARE
void client_start(uint8_t* buf);
void game_setup(uint8_t* buf, LCD_I2C* lcd);
void run_game(uint8_t* buf);

void debug_blink(void);
