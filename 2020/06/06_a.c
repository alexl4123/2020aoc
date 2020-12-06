#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {

    if(argc != 3) {
        fprintf(stderr,"Usage: %s -i <file>\n",argv[0]);
        return EXIT_FAILURE;
    }

    int opt;
    char * fileName = NULL;
    while((opt = getopt(argc,argv,"i:")) != -1) {
        switch(opt) {
            case 'i':
                fileName = optarg;
                break;
            default: 
                fprintf(stderr,"Usage: %s -i <file>\n",argv[0]);
                return EXIT_FAILURE;    
        }
    }

    int fd = open(fileName, O_RDONLY);

    if(fd < 0) {
        fprintf(stderr,"[%s]: Error opening file: %s\n",argv[0], fileName); 
        return EXIT_FAILURE;
    }
    

    int alph[26];
    memset(&alph,0,sizeof(int)*26);

    char buf, last;
    ssize_t size;
    long sum = 0;

    while((size = read(fd,&buf,1)) > 0) {
        if(buf >= 97 && buf <= 122) {
            int letter = buf - 97;
            if(alph[letter] == 0) {
                sum = sum + 1;
            }
            alph[letter] = 1;
        }


        if(buf == '\n' && last == '\n') {
            memset(&alph,0,sizeof(int) * 26);
        }

        last = buf;
    }
    
    fprintf(stdout, "[%s]: The total sum is: %ld \n", argv[0], sum);


    close(fd);

    return EXIT_SUCCESS;
}
