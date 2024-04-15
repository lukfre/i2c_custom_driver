//#pragma once
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>

#define BAUDRATE 		B19200		// deafult baudrate
#define BUFFER_SIZE 	32			// default buffer size
#define POLL_TIMEOUT	1 			// poll timeout
#define SYNC_TIMEOUT	100

char read_buf [BUFFER_SIZE] = {'\0'};   // buffer to store the data to be read
char write_buf[BUFFER_SIZE] = {'\0'};   // buffer to store the data to be sent

int fd_flag = 0;	// set this to 1 *ONLY* when UART is open
int DEBUG = 0;		// DEBUG = 1 enables debug prints in the code, _very_ verbose

struct termios termios;
struct pollfd fds;

int serial_init(const char* name);
int serial_write(int fd, const char* str);
int serial_read(int fd, char* str, int size);
void _reset_buffer(char* buf, int size);
void keyboard_read();
char* communicate(int fd);
void _gamemode_on();
void _gamemode_off();
void syncing(int fd, int time);