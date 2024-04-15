#include "comunication.h"
#include "interrupt/twi.h"

void com_init(){
  twi_init();
}
void com_beginTrasmission(uint8_t address){
    transmitting = 1;
    txAddress = address;
    txBufferIndex = 0;
    txBufferLength = 0;
}

uint8_t com_endTrasmission(uint8_t sendStop){

    uint8_t ret = twi_writeTo(txAddress, txBuffer, txBufferLength, 1, sendStop);
    transmitting = 0;
    txBufferIndex = 0;
    txBufferLength = 0;
    return ret;
}

int8_t com_write(uint8_t data)
{
  if(transmitting){
  // in master transmitter mode
    // don't bother if buffer is full
    if(txBufferLength >= BUFFER_LENGTH){
      //setWriteError(); TODO: write and check this function 
      return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer   
    txBufferLength = txBufferIndex;
  }else{
  // in slave send mode
    // reply to master
    twi_transmit(&data, 1);
  }
  return 1;
}