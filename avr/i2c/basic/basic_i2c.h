#pragma once
#define BUFFER_LENGTH 32
#include<inttypes.h>

#define I2C_READ 1
#define I2C_WRITE 0

void I2C_init_master(void);
void I2C_start(void);
void I2C_repeated_start(void);

void I2C_write_address(uint8_t);
void I2C_read_address(uint8_t);
void I2C_write_data(uint8_t);
uint8_t I2C_read_data(void);
void I2C_stop(void);
