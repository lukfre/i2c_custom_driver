#include "serial_lib.h"

/// LOW LEVEL ///
int serial_init(const char* name) {
    int fd = open(name, O_RDWR | O_NOCTTY | O_SYNC);  // READ/WRITE permission
                                   
    if (fd < 0){
        printf("[ERROR] %d in opening %s\n", errno, name);
        exit(1);
    }
    printf("\n  %s Opened successfully ", name);
    fd_flag = 1;
    struct termios SerialPortSettings;
    memset(&SerialPortSettings, 0, sizeof SerialPortSettings);
    if (tcgetattr(fd, &SerialPortSettings) != 0){
        printf("[ERROR] %d tcgetattr failed!\n", errno);
        exit(1);
    }
    cfsetispeed(&SerialPortSettings, BAUDRATE); // set read speed
    cfsetospeed(&SerialPortSettings, BAUDRATE); // set write speed
    
    cfmakeraw(&SerialPortSettings);

    SerialPortSettings.c_cflag |= (CLOCAL | CREAD);
    SerialPortSettings.c_cflag &= ~PARENB;  // no parity
    SerialPortSettings.c_cflag &= ~CSTOPB;  // 1 stop bit
    SerialPortSettings.c_cflag &= ~CSIZE;   // mask 
    SerialPortSettings.c_cflag |= CS8;      // 8-bit chars
    SerialPortSettings.c_cflag &= ~CRTSCTS;     // disable hardware flow control
    SerialPortSettings.c_cc[VMIN]  = 1;             // minimum number of characters for noncanonical read
    SerialPortSettings.c_cc[VTIME] = 5;             // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &SerialPortSettings) != 0){       // Set the attributes to the termios structure
        printf("\n[ERROR] in setting termios attributes\n");
        exit(1);
    }
    printf("\n  - BaudRate  = B19200\n  - StopBits  = 1\n  - Parity    = none\n  - DataBits  = 8\n  - Flow ctrl = none\n  - c_cflag   = %d\t\t(should be 3262 for 8N1@19200)\n", SerialPortSettings.c_cflag);
    
    printf("\n  Syncing"); fflush(stdout);
    syncing(fd, 5);
    return fd;
}

void syncing(int fd, int time){
    sleep(time);       // if you don't sleep here polling does not succed
    int retval = 0, i = 0;
    fds.fd = fd;
    fds.events = POLLIN;
    int fl = 0;
    while(1){
        printf(".");
        fflush(stdout);
        i++;
        retval = poll(&fds, 1, SYNC_TIMEOUT);
        if (DEBUG)          printf("retval = %d\n", retval);
        if (retval == -1)   perror("poll():");
        else if (retval){   // ready to read without blocking
            serial_read(fd, read_buf, BUFFER_SIZE);
            _reset_buffer(read_buf, BUFFER_SIZE);
            fl = 1;
        }
        else if (retval == 0){
            if (fl = 1){            //ho letto almeno una volta, non ha scritto altro --> posso iniziare
                printf("done! It tooks about %.2f seconds (not so accurate though)\n", i*0.1 + time);
                sprintf(write_buf, "R\n");
                serial_write(fd, write_buf);
                sleep(1);
                return;
            }
        }
    }
}

int serial_write(int fd, const char* str){
    const char * writtenString = str;
    int i = 0;
    while (*str /*|| i < size*/){
        int res = write(fd, str, 1);
        if (res == -1){
            if (errno == EINTR)
                continue;
            printf(">>[ERROR] %d in writing strings\n", errno);
            return -1;
        }
        else if (res == 0)
            continue;
        ++str;
        ++i;
    }
    if (DEBUG)
        printf(">> Successfully wrote %d bytes: (%s)\n", i, writtenString);
    return 0;
}

int serial_read(int fd, char* str, int size){
    int i = 0;
    if (DEBUG)  printf("serial_read\n");
    while(i < size){
        int res = read(fd, str, 1);
        if (DEBUG)  printf("res = %d ", res);
        if (res == -1){
            if (errno == EINTR)
                continue;
            printf(">>[ERROR] %d in reading strings\n", errno);
            return -1;
        }
        else if (res == 0)
            break;
        if (DEBUG)  printf("|%c|\n", *str);
        ++str;
        ++i;
        if (i > 0 && (*(str-1) == '\n' || *(str-1) == '\0'))
            break;  // interrompo la lettura se ricevo newline o \0
    }
    if (DEBUG)
        printf("<< Successfully read %d bytes\n", i);
    return 0;
}

void _reset_buffer(char* buf, int size){
    for (int i = 0; i < size; i++)
        buf[i] = '\0';
}

void _gamemode_on(){
    tcgetattr(STDIN_FILENO, &termios);
    termios.c_lflag &= ~ECHO;   //disable terminal echo
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
    system("stty raw");     //set terminal raw mode
}

void _gamemode_off(){
    system("stty cooked");      //set terminal cooked mode
    termios.c_lflag |= ECHO;    //re-enable terminal echo
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios);
}

/// HIGH LEVEL /// aka fiorellini

void keyboard_read(){
    fgets(write_buf, BUFFER_SIZE, stdin);   //no scanf("%s", write_buf);
    write_buf[BUFFER_SIZE - 2] = '\n';
    write_buf[BUFFER_SIZE - 1] = '\0';
    if (DEBUG)
        printf("sending: '%s' \n", write_buf);
}

char* communicate(int fd){
    serial_write(fd, write_buf);
    serial_read(fd, read_buf, BUFFER_SIZE);
    char* tmp = (char*) malloc(sizeof(char) * BUFFER_SIZE);
    strcpy(tmp, read_buf);
    if (DEBUG)
        printf("response: '%s'\n", tmp);
    _reset_buffer(read_buf, BUFFER_SIZE);
    return tmp;
}