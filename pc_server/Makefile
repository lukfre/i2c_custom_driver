CC=gcc

main: i2c_videogame.o 
	$(CC) -o i2c_videogame game_lib.h serial_lib.h i2c_videogame.c
	rm -f *.o

.PHONY: clean git

clean:
	rm -f i2c_videogame *.o *~