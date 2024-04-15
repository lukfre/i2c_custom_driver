#include "game_lib.h"

void print(const char* str, int reply){
    printf("  ");
    for(int i = 0; i < strlen(str)+4; i++)
        printf("_");
    printf("\n-+| %s |\n", str);
    printf("  ");
    for(int i = 0; i < strlen(str)+4; i++)
        printf("*");
    printf("\n");
    if (reply == REPLY)
        printf("> ");
}

void startgame(int fd){
    while(1){
        keyboard_read();
        if (strcmp(write_buf, "start\n") == 0)
            break;
        if (strcmp(write_buf, "quit\n") == 0)
            printf("  MUAHAHAHAHAHAHAHA YOU CAN'T QUIT\n");
        printf("  >> You have to *literally* write start, dumbass.\n> ");
    }
    char* tmp = communicate(fd);        // send START command
    free(tmp);

    print("Insert name", REPLY);    // send PLAYER's NAME
    keyboard_read();
    tmp = communicate(fd);
    char name[BUFFER_SIZE] = {'\0'};
    strcpy(name, tmp);
    free(tmp);
                                        // send DIFFICULTY
    print("You wanna play as MIRANDA, CUOMO, DAMORE or... GRISETTI? Choose.", REPLY);
    keyboard_read();
    tmp = communicate(fd);
    if (DEBUG)  printf("%d", atoi(tmp));
    if (atoi(tmp) == 4){

        /* TODO: Eliminare definitamente perchè da fastidio ad Arduino
         print("You have chosen NARDI. I suggest you to change your mind...", NO_REPLY);
        sleep(1);
        print("You wanna play as MIRANDA, CUOMO, DAMORE or... GRISETTI? Choose.", REPLY);
        keyboard_read();
        */ 
       print("You have chosen NARDI. Good luck.", NO_REPLY);
        // to do: disegnare meme dei simpson golf sul terminale
    }
    
    for(int i = 0; i < BUFFER_SIZE; i++){
        if (name[i] == '\n'){
            name[i] = '\0';
            break;
        }
    }

    sprintf(tmp, "Okay, here we go %s!", name);
    print(tmp, NO_REPLY);
    free(tmp);
    return;
}

void runbaby(int fd){

    int retval = 1;     // var to store poll() return value
    _gamemode_on();

    char c;
    char* tmp;
    while(1){
        retval = 1;
        c = getchar();
        switch(c){
            case 'w':
            case 'W':
                sprintf(write_buf, "w\n");
                tmp = communicate(fd);
                free(tmp);    
                break;
            case 's':
            case 'S':
                sprintf(write_buf, "s\n");
                tmp = communicate(fd);
                free(tmp);
                break;
            case 'q':
            case 'Q':
                _gamemode_off();
                sprintf(write_buf, "q\n");
                tmp = communicate(fd);
                free(tmp);
                retrieve_score(fd);
                return;
                //break;
            default:
                break;
        }
       
        // check for death
        while(retval){
            retval = poll(&fds, 1, POLL_TIMEOUT);     // check if fd has written something
            if (DEBUG)  printf("retval = %d\n", retval);
            
            if (retval == -1)
                perror("poll():");
            
            else if (retval){   // ready to read without blocking
                serial_read(fd, read_buf, BUFFER_SIZE);
                if (DEBUG)  printf("|%s|\n", read_buf);
                if (read_buf[0] == 'M'){
                    _reset_buffer(read_buf, BUFFER_SIZE);
                    _gamemode_off();
                    retrieve_score(fd);
                    return;
                }
                _reset_buffer(read_buf, BUFFER_SIZE);
            }
        }
    }
}

void retrieve_score(int fd){
    serial_read(fd, read_buf, BUFFER_SIZE);
    while(read_buf[0] == 'L' || read_buf[0] == 'M' || read_buf[0] == 'W' || read_buf[0] == 'S'){
        _reset_buffer(read_buf, BUFFER_SIZE);
        serial_read(fd, read_buf, BUFFER_SIZE);
    }
    printf("Your score: %s\n", read_buf);
    _reset_buffer(read_buf, BUFFER_SIZE);
}

void endgame(int fd){
    print("Bye!", NO_REPLY);
    if (fd_flag){
        if (close(fd) == -1)    // close the serial port
            perror("Error in close(): ");
    }
    fd_flag = 0;
}