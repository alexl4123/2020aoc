#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include<math.h>
#include <limits.h>

#define neighbourCount 80

typedef enum Operation {
    Nothing = 0,
    Addition = 1,
    Multiplication = 2
} Operation;

typedef enum NestReason {
    Void = 0,
    Brace = 1,
    Multi = 2
} NestReason;

//--------------------------------------------

int main (int argc, char * argv[]) {

    if(argc != 2) {
        fprintf(stderr,"Wrong arguments given!\n");
    }

    char * fileName = argv[1];

    int fd = open(fileName,O_RDONLY);

    if(fd == -1)  {
        fprintf(stderr,"Could not open file!\n");
        return EXIT_FAILURE;
    }

    char buf;
    ssize_t readSize;

    long long sum;

    int maxNesting = 100;
    long long sums[maxNesting];
    Operation ops[maxNesting];
    NestReason reasons[maxNesting];

    for(int i = 0; i < maxNesting; i++) {
        sums[i] = 0;
        ops[i] = Nothing;
        reasons[i] = Void;
    }

    int nestingLevel = 0;

    while((readSize = read(fd,&buf,1)) > 0) {
        
        if(buf >= '0' && buf <= '9') {
            int curNum = buf - '0';
            if(ops[nestingLevel] == Addition) {
                sums[nestingLevel] = sums[nestingLevel] + curNum; 
            } else if(ops[nestingLevel] == Multiplication) {
                sums[nestingLevel] = sums[nestingLevel] * curNum;
            } else {
                sums[nestingLevel] = curNum;
            }
        } else if(buf == '+') {
            ops[nestingLevel] = Addition;
        } else if(buf == '*') {
            ops[nestingLevel] = Multiplication;
            nestingLevel++;
            sums[nestingLevel] = 0;
            ops[nestingLevel] = Nothing;
            reasons[nestingLevel] = Multi;

        } else if(buf == '(') {
            nestingLevel++;
            
            sums[nestingLevel] = 0;
            ops[nestingLevel] = Nothing;
            reasons[nestingLevel] = Brace;
        } else if(buf == ')') {
            
            while(reasons[nestingLevel]==Multi) {
                nestingLevel--;
                if(ops[nestingLevel] == Addition) {
                    sums[nestingLevel] = sums[nestingLevel] + sums[nestingLevel+1];
                } else if(ops[nestingLevel] == Multiplication) {
                    sums[nestingLevel] = sums[nestingLevel] * sums[nestingLevel+1];
                } else {
                    sums[nestingLevel] = sums[nestingLevel+1];
                }    
                sums[nestingLevel+1] = 0;
                ops[nestingLevel+1] = Nothing; 
                reasons[nestingLevel+1] = Void;
            }
            nestingLevel--;
            if(ops[nestingLevel] == Addition) {
               sums[nestingLevel] = sums[nestingLevel] + sums[nestingLevel+1];
            } else if(ops[nestingLevel] == Multiplication) {
                sums[nestingLevel] = sums[nestingLevel] * sums[nestingLevel+1];
            } else {
                sums[nestingLevel] = sums[nestingLevel+1];
            }    
            sums[nestingLevel+1] = 0;
            ops[nestingLevel+1] = Nothing; 
            reasons[nestingLevel+1] = Void;
            
            
       } else if(buf == '\n') {
           while(reasons[nestingLevel]==Multi) {               
                nestingLevel--;
                if(ops[nestingLevel] == Addition) {
                    sums[nestingLevel] = sums[nestingLevel] + sums[nestingLevel+1];
                } else if(ops[nestingLevel] == Multiplication) {
                    sums[nestingLevel] = sums[nestingLevel] * sums[nestingLevel+1];
                } else {
                    sums[nestingLevel] = sums[nestingLevel+1];
                }    
                sums[nestingLevel+1] = 0;
                ops[nestingLevel+1] = Nothing; 
                reasons[nestingLevel+1] = Void;

            }
            if(nestingLevel != 0) {
                fprintf(stderr,"Something with the nesting failed.\n");
                break;
            }

            fprintf(stdout,"[%s]: This lines sum: %lld \n",argv[0], sums[nestingLevel]);
            sum = sum + sums[nestingLevel];

            for(int i = 0; i < maxNesting; i++) {
                sums[i] = 0;
                ops[i] = Nothing;
            }
        } else if(buf == ' ') {
            //Allowed
        } else {
            fprintf(stderr,"[%s]: Unknown char: %c \n",argv[0],buf);
            break;
        }

        if(buf != ' ') {
            fprintf(stdout,"[%s]: Cur stack: %d %lld %d\n",argv[0], nestingLevel, sums[nestingLevel], ops[nestingLevel]);
        }       
    }

    fprintf(stdout,"[%s]: All sum: %lld \n",argv[0], sum);
    close(fd);
}
