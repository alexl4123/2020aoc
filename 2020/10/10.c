#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

static void insertInto(int ** arr, int num,int curSize);

static long distinctPaths(int * arr, int curPos, int size);

int main (int argc, char * argv[]) {

    char * fileName = argv[1];


    int fd = open(fileName,O_RDONLY);

    if(fd == -1) {
        return EXIT_FAILURE;
    }
    
    char buf;
    ssize_t readSize;

    int maxArrSize = 100;
    int * arr = malloc(sizeof(int) * maxArrSize);
    arr[0] = 0;
    int curNum = 0;
    int arrSize = 1;

    while((readSize = read(fd,&buf,1)) > 0) {
        
        if(buf >= '0' && buf <= '9') {
            int curDigit = (buf-'0');
            curNum = (curNum * 10) + curDigit;
        } else if(buf == '\n') {
            insertInto(&arr,curNum,arrSize);

            curNum = 0;
            arrSize++;
        }
    }

    int diffOneJolt = 0, diffThreeJolt = 0;
    for(int i = 0; i < arrSize; i++) {
        if(i > 0) {
            int diff = (arr[i] - arr[i-1]);
            if(diff == 1) {
                diffOneJolt++;
            } else if(diff == 3) {
                diffThreeJolt++;
            }
        }
    }

    long distinct = distinctPaths(arr,0,arrSize);

    diffOneJolt++;
    diffThreeJolt++;

    fprintf(stdout,"Diff one jolt: %d Diff Three jolt: %d Mult: %d\n",diffOneJolt,diffThreeJolt,(diffOneJolt*diffThreeJolt));
    fprintf(stdout,"Distinct is: %ld\n",distinct);
    free(arr);
    return EXIT_SUCCESS;

}


static void insertInto(int ** arr, int num,int curSize) {
    
    int i = 0;
    int curNum = num;
    for(i = 0; i < curSize; i++) {
        if(curNum < (*arr)[i]) {
            int tmp = (*arr)[i];
            (*arr)[i] = curNum;
            curNum = tmp;
        }
    }
    (*arr)[i] = curNum;
}

static long distinctPaths(int * arr, int curPos, int size) {
    
    long * help = malloc(sizeof(long) * size);
    memset(help,0,sizeof(int) * size);
    for(int i = size-1; i >= 0; i--) {
        int count = 0;
        for(int j = i+1; j < i+4; j++) {
            if((j < size) && (arr[j]-arr[i] < 4)) {
                count++;
            }
        }
        if(count == 0) {
            help[i] = 1;
        } else if(count == 1) {
            help[i] = help[i+1];
        } else if(count == 2) {
            help[i] = help[i+1] + help[i+2];
        } else if(count == 3) {
            help[i] = help[i+1] + help[i+2] + help[i+3];
        }
        fprintf(stdout,"%ld\n",help[i]);
    }    
   
    free(help);

    return 0;

}
