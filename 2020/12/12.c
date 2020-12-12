#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

typedef enum Direction {Na=0,East=1,South=2,West=3,North=4,Left=5,Right=6,Forward=7} Direction;

typedef struct Instruction {
    Direction direction;
    int value;
} Instruction;

int main (int argc, char * argv[]) {

    char * fileName = argv[1];


    int fd = open(fileName,O_RDONLY);

    if(fd == -1) {
        return EXIT_FAILURE;
    }
    
    char buf;
    ssize_t readSize;
    
    int maxLength = 1000;
    Instruction * instructions = malloc(sizeof(Instruction) * maxLength);
    int length = 0;

    int num = 0;
    Instruction curInst;
    while((readSize = read(fd,&buf,1)) > 0) {
        if(buf == 'E') {
            curInst = (Instruction) {East,-1};
        } else if(buf=='S') {
            curInst = (Instruction) {South,-1};
        } else if(buf=='W') {
            curInst = (Instruction) {West,-1};
        } else if(buf=='N') {
            curInst = (Instruction) {North,-1};
        } else if(buf=='L') {
            curInst = (Instruction) {Left,-1};
        } else if(buf=='R') {
            curInst = (Instruction) {Right,-1};
        } else if(buf=='F') {
            curInst = (Instruction) {Forward,-1};
        } else if (buf >= '0' && buf <= '9') {
            int castSign = buf - '0';
            num = (num*10)+castSign;
        } else if(buf == '\n') {
            curInst.value = num;
            instructions[length] = curInst;

            num = 0;
            length++;
        }
    }
    

    long curX = 10, curY = 1;
    long shipX = 0, shipY = 0;

    for(int i = 0; i < length; i++) {
        Instruction curInst = instructions[i];

        if(curInst.direction == East) {
            curX = curX + curInst.value;
        } else if(curInst.direction == South) {
            curY = curY - curInst.value;
        } else if(curInst.direction == West) {
            curX = curX - curInst.value;
        } else if(curInst.direction == North) {
            curY = curY + curInst.value;
        } else if(curInst.direction == Left) {
            if((curInst.value % 360) == 0) {

            } else if((curInst.value % 360) == 90) {
                long tmp = curX;

                curX = -curY;
                curY = tmp;           
            } else if((curInst.value % 360) == 180) {
                curY = -curY;
                curX = -curX;
            } else if((curInst.value % 360) == 270) {
                long tmp = curY;

                curY = -curX;
                curX = tmp;
            } else {
                fprintf(stderr,"Left-Should never happen.\n");
                fprintf(stderr,"%d %d\n",curInst.direction,curInst.value);
                return EXIT_FAILURE;
            }
        } else if(curInst.direction== Right) {
            if((curInst.value % 360) == 0) {

            } else if((curInst.value % 360) == 90) {
                long tmp = curY;

                curY = -curX;
                curX = tmp;
            } else if((curInst.value % 360) == 180) {
                curY = -curY;
                curX = -curX;
            } else if((curInst.value % 360) == 270) {
                long tmp = curX;

                curX = -curY;
                curY = tmp;
            } else {
                fprintf(stderr,"Right - should never happen.\n");
                fprintf(stderr,"%d %d\n",curInst.direction,curInst.value);
                return EXIT_FAILURE;
            }
        } else if(curInst.direction == Forward) {
            shipX = shipX + (curX * curInst.value);
            shipY = shipY + (curY * curInst.value);
        } else {
            fprintf(stderr,"Should never happen.\n");
            return EXIT_FAILURE;
        }

        fprintf(stdout,"<%ld %ld | %ld %ld>\n",curX,curY,shipX,shipY);
    }

    if(shipX < 0) {
        shipX = (shipX * (-1));       
    }
    if(shipY < 0) {
        shipY = (shipY * (-1));
    }


    fprintf(stdout,"X: %ld, Y: %ld Dist: %ld\n",shipX, shipY, shipX + shipY);
        free(instructions);

   return EXIT_SUCCESS;

}


