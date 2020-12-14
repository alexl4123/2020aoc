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
    long long index;
    long long value;
    int mask[36];
} MemInstruction;

typedef struct MemoryItem {
    long long index;
    long long value;
}MemoryItem;

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
    for(int i = 0; i < cBufSize; i++) {
        cBuf[i] = 0;
    }

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
   
    /*
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
    */

    int maxMemSize = 600;
    int memSize = 0;
    fprintf(stdout,"malloc size %ld \n",(long) (maxMemSize * sizeof(long long)));
    MemoryItem * memory = malloc(sizeof(long long) * 2 * maxMemSize);
    int * curMask;
    int curBinaryNum[36];

    for(int i = 0; i < maxMemSize; i++) {
        memory[i] = (MemoryItem) {0,0};
    }

    for(int i = 0; i < 36; i++) {
        curBinaryNum[i] = 0;
    }

    for(int i = 0; i < size; i++) {
        //fprintf(stdout,"next one \n");
        if(instructions[i].type == 1) {
            curMask = instructions[i].mask;
        } else {
            //----------DEBUG_OUTPUT---------------------
            /*fprintf(stdout,"curMask = ");

            for(int j = 0; j < 36; j++) {
                fprintf(stdout,"%d",curMask[j]);
            }
            fprintf(stdout,"\n");
            */

            //----------------DEC_TO_BIN--------------
            long tmp = instructions[i].index;
            int tmpSize = 0;
            for(tmpSize = 0; tmp > 0; tmpSize++) {
                curBinaryNum[tmpSize] = tmp%2;
                tmp = tmp/2;
            }

            //----------------DEBUG_OUTPUT------------------
            /*
            fprintf(stdout,"Index: %d, bin-index: ",instructions[i].index);
            for(int j = 0; j < tmpSize; j++) {
                fprintf(stdout,"%d",curBinaryNum[j]);
            }
            fprintf(stdout,"\n");
            */

            //------------APPLY_BITMASK----------------------
            long long bitMaskSize = 1;
            //fprintf(stdout,"New index: ");
            for(int j = 0; j < 36; j++) {
                if(curMask[j] == 1) {
                    curBinaryNum[j] = 1;
                } else if(curMask[j] == -1) {
                    curBinaryNum[j] = -1;
                    bitMaskSize = bitMaskSize * 2;
                }
                //fprintf(stdout,"%d",curBinaryNum[j]);
            }
            //fprintf(stdout,"\n");
            

            //---------GENERATE_INDEXES--------------------
            size_t mallocSize = (sizeof(int) * 36 * bitMaskSize);
            fprintf(stdout,"Malloc inner: %zu \n",mallocSize);
            int ** bitMaskList = malloc(mallocSize);
            //fprintf(stdout,"Bitmasksize: %lld\n",bitMaskSize);
            for(int bit = 0; bit < bitMaskSize; bit++) {
                bitMaskList[bit] = malloc(sizeof(int) * 36);
                //----SET_TO_0--------
                int tmpBinaryNum[36];
                for(int tmpBinaryNumIndex = 0; tmpBinaryNumIndex < 36; tmpBinaryNumIndex++) {
                    tmpBinaryNum[tmpBinaryNumIndex] = 0;
                }

                //---CONV_CUR_ADDR_TO_BIN--------------
                tmp = bit;
                for(tmpSize = 0; tmp > 0; tmpSize++) {
                    tmpBinaryNum[tmpSize] = tmp%2;
                    tmp = tmp/2;
                }

                //---
                int xIndex = 0;
                for(int j = 0; j < 36; j++) {
                    if(curBinaryNum[j] == -1) {
                        bitMaskList[bit][j] = tmpBinaryNum[xIndex];
                        xIndex++;
                    } else {
                        bitMaskList[bit][j] = curBinaryNum[j];
                    }
                }            
            }

            //----------DEBUG_OUTPUT----------------------
            /*
            fprintf(stdout,"%lld Bitmasks: \n",bitMaskSize);
            for(int j = 0; j < bitMaskSize; j++) {
                fprintf(stdout,"Bit-mask: ");
                for(int k = 0; k < 36; k++) {
                    fprintf(stdout,"%d",bitMaskList[j][k]);
                }
                fprintf(stdout,"\n");
            }*/
            
            //---------------WRITE_TO_MEM---------------------

            for(long long bitMaskIndex = 0; bitMaskIndex < bitMaskSize; bitMaskIndex++) {
                long long dec = 0;
                for(int j = 0; j < 36; j++) {
                    dec += bitMaskList[bitMaskIndex][j] * pow(2,j);
                }

                long long insertIndex = 0;
                for(int j = 0; j < memSize; j++) {
                    if(((long long) memory[j].index) == ((long long) dec)) {
                        insertIndex = j;
                        break;
                    } 
                    insertIndex++;
                            
                    
                }
                if(insertIndex == memSize) { 
                    memSize++;
                }

                if(memSize> maxMemSize) {
                    long tempMaxSize = 2*maxMemSize;
                    MemoryItem * tmpMemory = malloc(sizeof(MemoryItem) * tempMaxSize);
                    for(int j = 0; j < tempMaxSize; j++) {
                        tmpMemory[j] = (MemoryItem) {0,0};
                    }
                    for(int j = 0; j < maxMemSize; j++) {
                        tmpMemory[j] = memory[j];
                    }
                    free(memory);
                    memory = tmpMemory;
                    maxMemSize = tempMaxSize;
                }

                long long val = instructions[i].value;
                memory[insertIndex] = (MemoryItem) {(long long) dec,(long long) val};
                
                if(memSize > maxMemSize) {
                    fprintf(stderr,"memSize > maxMemSize\n");
                    
                    for(int j = 0; j < bitMaskSize; j++) {
                        free(bitMaskList[j]);
                    }
                    free(bitMaskList);       
                    free(memory); 
                    return EXIT_FAILURE;
                }


                //fprintf(stdout,"BitMaskIndex: %lld Index: %d ToWriteIndex: %lld Value: %lld \n",bitMaskIndex, insertIndex, dec,instructions[i].value);

                for(int i = 0; i < 36; i++) {
                    curBinaryNum[i] = 0;
                }
            }
            
            for(int j = 0; j < bitMaskSize; j++) {
                free(bitMaskList[j]);
            }
            free(bitMaskList);
        }
    }

    unsigned long long sum = 0;
    for(int i = 0; i < memSize ; i++) {
        sum += memory[i].value;
        fprintf(stdout,"mem[%d] -> Index = %lld; Value = %lld\n",i,memory[i].index,memory[i].value);
    }

    fprintf(stdout,"Sum: %llu\n",sum);
    
    free(memory);
    free(instructions);
   return EXIT_SUCCESS;

}


