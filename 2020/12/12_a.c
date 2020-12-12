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


    long curX = 0, curY = 0;
    int facing = 0;

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
            facing = (facing + curInst.value) % 360; 
        } else if(curInst.direction== Right) {
            facing = (facing - curInst.value);
            if(facing < 0) {
                //Special case, if facing is something like -450 degrees
                facing = (facing * (-1)) % 360;
                //Normal case
                facing = 360 - facing;
            }
        } else if(curInst.direction == Forward) {
            if(facing == 0) {
                curX = curX + curInst.value;
            } else if(facing == 90) {
                //North
                curY = curY + curInst.value;
            } else if(facing == 180) {
                curX = curX - curInst.value;
            } else if(facing == 270) {
                curY = curY - curInst.value;
            }
        } else {
            fprintf(stderr,"Should never happen.\n");
            return EXIT_FAILURE;
        }

        fprintf(stdout,"<%ld %ld | facing: %d>\n",curX,curY,facing);
    }

    if(curX < 0) {
        curX = (curX * (-1));       
    }
    if(curY < 0) {
        curY = (curY * (-1));
    }


    fprintf(stdout,"X: %ld, Y: %ld Dist: %ld\n",curX, curY, curX + curY);
        free(instructions);

   return EXIT_SUCCESS;

}


