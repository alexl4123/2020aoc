#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <limits.h>


typedef enum Direction {Na=0,East=1,South=2,West=3,North=4,Left=5,Right=6,Forward=7} Direction;

typedef struct Instruction {
    Direction direction;
    int value;
} Instruction;

int main (int argc, char * argv[]) {

    if(argc != 2) {
        fprintf(stderr,"Wrong arguments given!\n");
    }

    char * fileName = argv[1];

    int fd = open(fileName,O_RDONLY);

    if(fd == -1) {
        fprintf(stderr,"Could not open file!\n");
        return EXIT_FAILURE;
    }
    
    char buf;
    ssize_t readSize;
    

    long minWaitTime = 0;
    
    int maxSize = 20;
    int size = 0;
    int * busses = malloc(sizeof(int) * maxSize);

    int num = 0;
    int firstLine = 1;
    while((readSize = read(fd,&buf,1)) > 0) {
        fprintf(stdout,"%c",buf);
        if((buf >= '0') && (buf <= '9')) {
            int digit = buf - '0';
            num = (10*num) + digit;
        } else if(buf == ',')  {
            if(num > 0) {
                busses[size] = num;
                size++;
                num = 0;
            }
        } else if(buf == '\n') {
            if(firstLine) {
                minWaitTime = num;
                firstLine = 0;
            }
            num = 0;
        }
    }

    long min = LONG_MAX;
    int id = 0;
    for(int i  = 0; i < size; i++) {
        int cur = busses[i];

        while(cur < minWaitTime) {
            cur = cur + busses[i];
        }

        if(cur < min) {
            min = cur;
            id = busses[i];
        }
    }

    long minWaited = min - minWaitTime;
    long res = minWaited * id;
    
    fprintf(stdout,"Min: %ld, ID: %d, minWaited: %ld, res: %ld  \n",min,id,minWaited,res);
   

    fprintf(stdout,"%ld wt.\n",minWaitTime);
    for(int i = 0; i < size; i++) {
        fprintf(stdout,"%d,",busses[i]);
    }
    fprintf(stdout,"\n");
   
    free(busses);
 
    return EXIT_SUCCESS;

}


