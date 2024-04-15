// Program for Slave mode
#include<avr/io.h>
#include<util/delay.h>

#include"../i2c/basic/basic_i2c_slave.h"

unsigned char write_data,recv_data;
 
int main(void){
	DDRB=0xff;
	I2C_init_slave(); // Function to initilaize slave
	while(1){
		I2C_match_read_slave(); //Function to match the slave address and slave dirction bit(read) 
		recv_data = I2C_read_slave(); // Function to read data
 
		write_data=~recv_data; // Togglem the receive data
 
		I2C_match_write_slave(); //Function to match the slave address and slave dirction bit(write) 
		I2C_write_slave(write_data);       // Function to write data
	}
}
 