#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include<math.h>
#include <limits.h>

typedef struct MemInstruction {
    int type;
    int index;
    int value;
    int mask[36];
} MemInstruction;


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

    char cBufSize = 6;
    char cBuf[6];
    int maxSize = 600;
    int size = 0;
    MemInstruction * instructions = malloc(sizeof(MemInstruction) * maxSize);
    MemInstruction curInstruction;

    int mode = 0;
    
    long curNum = 0;
    int curDigit = 0;

    while((readSize = read(fd,&buf,1)) > 0) {
        for(int i = (cBufSize-2); i >= 0; i--) {
           cBuf[i+1] = cBuf[i];
        }
        cBuf[0] = buf;

        if(cBuf[0] == 'k' && cBuf[1] == 's' && cBuf[2] == 'a' && cBuf[3] == 'm') {
            mode = 1;
            curDigit = 35;
            curInstruction = (MemInstruction) {1,-1,-1};
        } else if(mode == 1 && (buf == 'X' || buf == '0' || buf == '1')) {
            if(buf == 'X') {
                curInstruction.mask[curDigit] = -1;
            } else if(buf == '0') {
                curInstruction.mask[curDigit] = 0;
            } else if( buf == '1') {
                curInstruction.mask[curDigit] = 1;
            }
            curDigit--;
        } else if(cBuf[0] == 'm' && cBuf[1] == 'e' && cBuf[2] == 'm') {
            mode = 2;
            curInstruction = (MemInstruction) {2,0,0};
        } else if(mode == 2 && buf >= '0' && buf <= '9') {
            curDigit = buf - '0';
            curNum = (curNum * 10) + curDigit;
        } else if(mode == 2 && buf == ']') {
            curInstruction.index = curNum;
            curNum = 0;
        } else if(buf == '\n') {
            curInstruction.value = curNum;
            instructions[size] = curInstruction;

            mode = 0;
            curNum = 0;
            curDigit = 0;
            size++;
        }
    }
   
    int maxArrIndex = INT_MIN;
    int minArrIndex = INT_MAX;

    for(int i = 0; i < size; i++) {
        if(instructions[i].type == 2) {
            if(instructions[i].index > maxArrIndex) {
                maxArrIndex = instructions[i].index;
            } 
            if(instructions[i].index < minArrIndex) {
                minArrIndex = instructions[i].index;
            }
        }    
    }
    fprintf(stdout,"max: %d, min:%d \n",maxArrIndex, minArrIndex);

    long long memory[(maxArrIndex+1)-minArrIndex];
    int * curMask;
    int curBinaryNum[36];

    for(int i = 0; i < (maxArrIndex+1)-minArrIndex; i++) {
        memory[i] = 0;
    }

    for(int i = 0; i < 36; i++) {
        curBinaryNum[i] = 0;
    }

    for(int i = 0; i < size; i++) {
        if(instructions[i].type == 1) {
            curMask = instructions[i].mask;
        } else {
            fprintf(stdout,"curMask = ");

            for(int j = 0; j < 36; j++) {
                fprintf(stdout,"%d",curMask[j]);
            }
            fprintf(stdout,"\n");

            long tmp = instructions[i].value;
            int tmpSize = 0;
            for(tmpSize = 0; tmp > 0; tmpSize++) {
                curBinaryNum[tmpSize] = tmp%2;
                tmp = tmp/2;
            }
            fprintf(stdout,"Num: %d, bin-num: ",instructions[i].value);
            for(int j = 0; j < tmpSize; j++) {
                fprintf(stdout,"%d",curBinaryNum[j]);
            }
            fprintf(stdout,"\n");

            fprintf(stdout,"New num: ");
            for(int j = 0; j < 36; j++) {
                if(curMask[j] == 1 || curMask[j] == 0) {
                    curBinaryNum[j] = curMask[j];
                }
                fprintf(stdout,"%d",curBinaryNum[j]);
            }
            fprintf(stdout,"\n");
            
            long long dec = 0;
            for(int j = 0; j < 36; j++) {
                dec += curBinaryNum[j] * pow(2,j);
            }
            memory[(instructions[i].index-minArrIndex)] = dec;

            fprintf(stdout,"Dec: %lld \n",dec);

            for(int i = 0; i < 36; i++) {
                curBinaryNum[i] = 0;
            }
        }
    }

    unsigned long long sum = 0;
    for(int i = 0; i < ((maxArrIndex+1)-minArrIndex); i++) {
        sum += memory[i];
        fprintf(stdout,"mem[%d] = %lld\n",i,memory[i]);
    }

    fprintf(stdout,"Sum: %llu\n",sum);

    free(instructions);
   return EXIT_SUCCESS;

}


