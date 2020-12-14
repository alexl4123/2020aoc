#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include <limits.h>

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

    
    int maxSize = 100;
    int size = 0;
    int * busses = malloc(sizeof(int) * maxSize);

    int num = 0;
    while((readSize = read(fd,&buf,1)) > 0) {
        if((buf >= '0') && (buf <= '9')) {
            int digit = buf - '0';
            num = (10*num) + digit;
        } else if(buf == ',')  {
            if(num > 0) {
                busses[size] = num;
            } else {
                busses[size] = 1;
            }
            num = 0;
            size++;
        } else if(buf == '\n') {
            if(num > 0) {
                busses[size] = num;
                size++;
            } else {
                busses[size] = 1;
                size++;
            }
            num = 0;
        }
    }

    long long * nums = malloc(sizeof(long long) * size);
    if(nums == NULL) {
        fprintf(stderr,"malloc failed.\n");
        free(busses);
        return EXIT_FAILURE;
    }    
  
    long long ans = 0;
    long long mod = 1;
    for(int i = 0; i < size; i++) {
        fprintf(stdout,"i: %d, num: %d, ans: %lld mod: %lld \n",i,busses[i],ans,mod);
        while(((ans+i) % busses[i]) != 0) {
            ans += mod;
        }
        mod *= busses[i];
    }

    fprintf(stdout,"%lld \n",ans);

    free(busses);
    return EXIT_SUCCESS;

}


