
// Program for Master Mode
// Check Code2 for Slave Mode Program
#include<avr/io.h>
#include<util/delay.h>
#include<inttypes.h>
#include<stdio.h> 

#include "../avr_common/uart.h" // this includes the printf and initializes it
#include "../i2c/basic/basic_i2c.h" //include i2c primitives
 
unsigned char address=0x20, read=1, write=0;
unsigned char write_data=0x01, recv_data;
 
int main(void){
	
	printf_init();
	printf("Hi \n");
	_delay_ms(2000);
	DDRB=0xff;	//8 registri in WRITE
	I2C_init_master();  // Function to initialize TWI
	while(1){
		if(write_data==0x00) 
			write_data=1;
 		printf("Before \n");
		I2C_start(); // Function to send start condition
		I2C_write_address(address+write); // Function to write address and data direction bit(write) on SDA
		I2C_write_data(write_data);      // Function to write data in slave
		I2C_stop(); // Function to send stop condition
 
 		printf("Before \n");
		_delay_ms(10); // Delay of 10 mili second
 
		I2C_start(); 
		I2C_write_address(address+read); // Function to write address and data direction bit(read) on SDA
		recv_data = I2C_read_data(); // Function to read data from slave 
		I2C_stop();
 
		_delay_ms(1000); 
		write_data = write_data * 2;
		printf("After \n");
		printf("%d\n",recv_data);
		printf("%d\n",write_data);
	}
}
 