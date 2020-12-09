#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

int main(int argc, char * argv[]) {

    if(argc != 3) {
       fprintf(stderr,"[%s]: Error: Usage %s -i <FILE>",argv[0], argv[0]);
       return EXIT_FAILURE;

    }

    char* fileName = argv[2];

    char buf;
    ssize_t readBytes;

    int fd = open(fileName,O_RDONLY);

    if(fd == -1) {
        fprintf(stderr,"[%s]: Error opening file: %s", argv[0], fileName);
        return EXIT_FAILURE;
    }


    int listSize = 1001;
    long * preamble = malloc(sizeof(long) * listSize);
    int curPos = 0;

    long curNum = 0;
    while((readBytes = read(fd,&buf,1)) > 0) {

        if(buf >= '0' && buf <= '9') {
            int bufAsInt = buf - '0';
            curNum = (curNum * 10) + bufAsInt;
        } else if(buf == '\n') {
            preamble[curPos] = curNum;
            curNum = 0;
            curPos++;
        } else {
            fprintf(stderr,"<<OTHER_CHAR: %c>>\n",buf);
        }


    }

    int numbersConsidered = 25;
    int invalidNum = -1;

    for(int i = 0; i < curPos; i++) {
        if(i >= numbersConsidered) {
            int start = i - numbersConsidered;
            int end = i - 1;

            int doesItSumUp = 0;
            for(int j = start; j <= end; j++) {
                for(int k = j + 1; k <= end; k++) {
                    if((preamble[j] + preamble[k]) == preamble[i]) {
                        doesItSumUp = 1;
                        //fprintf(stdout,"<%ld + %ld = %ld || Line num: <%d + %d = %d>>\n",preamble[j], preamble[k], preamble[i], j, k, i);
                    }
                }
            }

            if(doesItSumUp == 0) {
                fprintf(stdout,">! %ld line num %d!<\n", preamble[i], i);
                invalidNum = preamble[i];
            }
        }
    }


    for(int i = 0; i < curPos; i++) {
        
        int curIndex = i;
        long curSum = 0;

        long min = LONG_MAX;
        long max = LONG_MIN;

        while(curSum < invalidNum) {
            if(preamble[curIndex] < min) {
                min = preamble[curIndex];
            }
            if(preamble[curIndex] > max) {
                max = preamble[curIndex];
            }
            curSum = curSum + preamble[curIndex];
            curIndex++;
    
        }
        curIndex--;

        if(curSum == invalidNum && (i != curIndex)) { 
            fprintf(stdout,">!!%ld -> %ld -> %ld || %d -> %d !!<\n",preamble[i],preamble[curIndex],(preamble[i]+preamble[curIndex]), i, curIndex);
            fprintf(stdout,">!!MIN: %ld MAX: %ld SUM: %ld <!!\n",min,max,(min+max));
        }
    }

    return EXIT_SUCCESS;
}
