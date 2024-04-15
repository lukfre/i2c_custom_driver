#pragma once
#define BUFFER_LENGTH 32
#include<inttypes.h>

#define I2C_READ 1
#define I2C_WRITE 0

void I2C_init_slave(void);
void I2C_match_read_slave(void);
uint8_t I2C_read_slave(void);
void I2C_match_write_slave(void);
void I2C_write_slave(uint8_t);
 