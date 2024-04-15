#pragma once
#include <stdint.h>
#define BUFFER_LENGTH 32
#define WIRE_HAS_END 1

uint8_t rxBuffer[BUFFER_LENGTH];
uint8_t rxBufferIndex;
uint8_t rxBufferLength;

uint8_t txAddress;
uint8_t txBuffer[BUFFER_LENGTH];
uint8_t txBufferIndex;
uint8_t txBufferLength;

uint8_t transmitting;

void com_init(void);
void com_beginTrasmission(uint8_t);
uint8_t com_endTrasmission(uint8_t);
int8_t com_write(uint8_t);
