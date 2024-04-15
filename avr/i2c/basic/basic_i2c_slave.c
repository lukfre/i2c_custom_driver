#include<assert.h> //check if work with avr
#include"basic_i2c_slave.h"
#include<avr/io.h>
#include<util/delay.h>
#include<inttypes.h>

void I2C_init_slave(void){ // Function to initilaize slave
	TWAR=0x20; // Fill slave address to TWAR
}
 
void I2C_write_slave(uint8_t write_data) {// Function to write data
    TWDR= write_data;           // Fill TWDR register whith the data to be sent 
    TWCR= (1<<TWEN)|(1<<TWINT);   // Enable TWI, Clear TWI interrupt flag 
    while((TWSR & 0xF8) != 0xC0); // Wait for the acknowledgement
}
 
void I2C_match_write_slave(void) {//Function to match the slave address and slave dirction bit(write) 
    while((TWSR & 0xF8)!= 0xA8){ // Loop till correct acknoledgement have been received
        // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
        TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); 
        while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
    }
}
 
uint8_t I2C_read_slave(void){
// Clear TWI interrupt flag,Get acknowlegement, Enable TWI
    uint8_t recv_data;
    TWCR= (1<<TWINT)|(1<<TWEA)|(1<<TWEN); 
    while (!(TWCR & (1<<TWINT))); // Wait for TWINT flag
    while((TWSR & 0xF8)!=0x80); // Wait for acknowledgement
    recv_data=TWDR; // Get value from TWDR
    PORTB=recv_data; // send the receive value on PORTB
    return recv_data;
}
 
void I2C_match_read_slave(void){ //Function to match the slave address and slave dirction bit(read)

    while((TWSR & 0xF8)!= 0x60){  // Loop till correct acknoledgement have been received
        // Get acknowlegement, Enable TWI, Clear TWI interrupt flag
        TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT); 
        while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
    }
}
 

