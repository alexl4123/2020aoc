#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include<math.h>
#include <limits.h>

typedef struct HelperItem {
    int lastTurn;
    int count;
} HelperItem;

int main (int argc, char * argv[]) {

    fprintf(stdout,"test\n");
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

    int maxSize = 600;
    HelperItem * items = malloc(sizeof(HelperItem) * maxSize);

    if(items == NULL) {
        fprintf(stderr,"Malloc failed.\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout,"test\n");
    for(int i = 0; i < maxSize; i++) {
        items[i] = (HelperItem) {0,0};
    }

    long curNum = 0;
    int turn = 1;
    int lastNum = 0;
    fprintf(stdout,"test\n");
    
    while((readSize = read(fd,&buf,1)) > 0) {
        if(buf >= '0' && buf <= '9') {
            int curDigit = buf - '0';
            curNum = (curNum * 10) + curDigit;
        } else if(buf == ',') {
            int count = items[curNum].count;
            items[curNum] = (HelperItem) {turn,count+1};
            lastNum = curNum;

            turn++;

            curNum = 0;
        } else if(buf == '\n') {
            lastNum = curNum;
            turn++;
            curNum = 0;
        }
    }

    while(turn <= 2020) {
        fprintf(stdout,"Turn :%d: %d \n",turn,lastNum);

        int count = items[lastNum].count;
        
        if(items[lastNum].count == 0) {
            //Has never been before.
            items[lastNum] = (HelperItem) {turn-1,count+1};
            lastNum = 0;
        } else {
            int tmp = (turn-1) - (items[lastNum].lastTurn);
            items[lastNum] = (HelperItem) {turn-1,count+1};
            lastNum = tmp;            
        }

        if(lastNum >= maxSize) {
            int newMaxSize = 2 * maxSize;
            HelperItem * newItems = malloc(sizeof(HelperItem) * newMaxSize);
            for(int i = 0; i < newMaxSize; i++) {
                newItems[i] = (HelperItem) {0,0};
            }

            for(int i = 0; i < maxSize; i++) {
                newItems[i] = items[i];
            }

            free(items);
            items = newItems;
            maxSize = newMaxSize;    
        }

        turn++;
    }

    fprintf(stdout,"Number on 2020th turn: %d\n",lastNum);
   
    free(items);
 return EXIT_SUCCESS;

}


