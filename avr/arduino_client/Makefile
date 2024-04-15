# AVR makefile stub

# to clean a folder
# from command line
#$> make clean

# to build the folder
#$> make 

# to upload a file
# from command line
# $> make <main_filename>.hex

# !!!!!!!!!!!!!!!! MODIFY HERE !!!!!!!!!!!!!!!! 

# put here the file containing the main routine
# to be uploaded on the avr
# you can add multiple files, they will be all generated

BINS=arduino_client.elf

# put here the additional .o files you want to generate
# one .c file for each .o should be present

OBJS= ../avr_common/uart.o ../avr_common/utils.o
OBJS+= ../i2c/interrupt/twi.o ../i2c/comunication.o ../my_lcd_i2c/LCD_I2C.o
OBJS+= game/game.o


HEADERS= ../avr_common/uart.h ../avr_common/utils.h
HEADERS+= ../i2c/interrupt/twi.h ../i2c/comunication.h ../my_lcd_i2c/LCD_I2C.h
HEADERS+= game/game.h

include ../avr_common/avr.mk


