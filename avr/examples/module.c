#include <stdio.h>
#include<inttypes.h>
int precModule(int offset){
    return (offset == 0) ? 39 : offset -1; 
}

/*This is a simple Main File for testing Operator Module */
int main(){

    int16_t offset = 3;
    int i = 0;
    while(i++ < 100){
        printf("%d - %d\n",offset, (offset+15)%40);
        offset = (offset+1)%40;
    }
    return 0;
}