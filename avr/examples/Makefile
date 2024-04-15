# AVR makefile stub

# to clean a folder
# from command line
#$> make clean

# to build the folder
#$> make 

# to upload a file
# from command line
# $> make <main_filename>.hex

#
# remember to give yourself the read/write permissions on the
# serial line
# $> sudo addgroup <your username> dialout
# reboot



# !!!!!!!!!!!!!!!! MODIFY HERE !!!!!!!!!!!!!!!! 

# put here the file containing the main routine
# to be uploaded on the avr
# you can add multiple files, they will be all generated

#BINS=int0.elf pin_change.elf master.elf slave.elf lcd_test.elf
BINS=lcd_test.elf
# put here the additional .o files you want to generate
# one .c file for each .o should be present
OBJS=../avr_common/uart.o ../i2c/comunication.o  #../i2c/i2c_interrupt/i2c.o
OBJS+= ../i2c/interrupt/twi.o
OBJS+= ../my_lcd_i2c/LCD_I2C.o
#OBJS+= ../i2c/primitive/basic_i2c.o
# put here the additional header files needed for compilation

HEADERS=../avr_common/uart.h ../i2c/comunication.h  #../i2c/i2c_interrupt/i2c_status.h ../i2c/i2c_interrupt/i2c.h
HEADERS+= ../i2c/interrupt/twi.h
#HEADERS+= ../i2c/primitive/basic_i2c.h
HEADERS+= ../my_lcd_i2c/LCD_I2C.h

# the file below contains the actual rules

include ../avr_common/avr.mk

# !!!!!!!!!!!!!!!! MODIFY HERE !!!!!!!!!!!!!!!! 



