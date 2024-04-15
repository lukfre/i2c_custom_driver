#include <signal.h>
#include "serial_lib.c"
#include "game_lib.c"
#include "ascii_art.h"

int fd;

void the_handler(int sig){
    if (!fd_flag)
        return;
    switch(sig){
        case SIGTERM:
        case SIGINT:
        case SIGSTOP:
            if (close(fd) == -1)
                perror("Error in close(): ");
            fd_flag = 0;
            printf(">>[SIGNAL] UART correctly closed. Exiting.\n");
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char **argv){

    int signals[] = {SIGTERM, SIGINT, SIGSTOP};
    int num_sig = 3;
    struct sigaction action;
    action.sa_handler = the_handler;
    for (int i = 0; i < num_sig; i++)
        if (sigaction(signals[i], &action, NULL))
            perror ("Error in sigaction");

	if (argc < 2){
		print("Usage: ./i2c_videogame <SERIAL_DEVICE> <DEBUG_MODE>", NO_REPLY);
		exit(1);
	}
    if (*argv[1] == '0')
    	fd = serial_init("/dev/ttyACM0");
    else if (*argv[1] == '1')
    	fd = serial_init("/dev/ttyACM1");
    else {
    	printf("  Write the right serial device pls.\n");
    	exit(1);
    }
    if (argc > 2)
    	DEBUG = *argv[2];

    printf("%s\n", i2c_videogame);      //splash screen

    while(1) {
    	print("Write 'start' to start, 'quit' to quit :D", REPLY);
    	startgame(fd);
        print("Press w and s to move, q to quit!", NO_REPLY);
        printf("\n");
        runbaby(fd);
        print("Another run? (y/n)", REPLY);
        keyboard_read(write_buf);
        char* tmp = communicate(fd);
        free(tmp);
        if (strcmp(write_buf, "y\n") == 0 || strcmp(write_buf, "Y\n") == 0){
            continue;
        }
		else
			break;
    }
    endgame(fd);
    return 0;
}


// syncing                                          OK
// quit non funziona                                OK             
// CTRL+C non fa terminare il gioco su arduino      
// diverse velocità per le difficoltà
//         densità nemici