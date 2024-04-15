#include<assert.h> //check if work with avr
#include"basic_i2c.h"
#include<avr/io.h>
#include<util/delay.h>
#include<inttypes.h>

//TWI = Two Wire
//TWCR = TWI Control Register
//TWINT = TWI Interrupt Flag

void I2C_init_master(){
	TWBR=0x01;    // Bit rate
	TWSR=(0<<TWPS1)|(0<<TWPS0);    // Setting prescalar bits
	// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)
}
void I2C_start(){
	//clear TWI interrupt flag, put start condition on SDA, enable TWI
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //send START condition
	while(!(TWCR & (1<<TWINT))); //wait till start condition is trasmitted
	while((TWSR & 0xF8) != 0x10); //check for the ACK
}

void I2C_repeated_start(){
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); 
	while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
	while((TWSR & 0xF8)!= 0x10); // Check for the acknoledgement
}

void I2C_write_address(uint8_t data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);	//clear TWI interrupt flag, enable TWI
	while(!(TWCR & (1<<TWINT)));	//wait until complete TWDR byte received
	while((TWSR & 0xF8) != 0x40); //check ACK
	
}
void I2C_write_data(uint8_t data){
	TWDR=data; // put data in TWDR
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x28); // Check for the acknoledgement
}

uint8_t I2C_read_data(){
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x58); // Check for the acknoledgement
	uint8_t recv_data=TWDR;
	return recv_data;
}
void I2C_stop(){
	//clear TWI interrupt flag, put stop condition on SDA, enable TWI
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO); // trasmit STOP condition
	while(!(TWCR & (1<<TWSTO)));	//wait until stop condition is tramistted
}
