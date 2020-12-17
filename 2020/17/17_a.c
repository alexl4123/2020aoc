#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include<math.h>
#include <limits.h>

#define neighbourCount 27

typedef struct Cube {
    int x;
    int y;
    int z;
    int neighbours[neighbourCount];
} Cube;

static void updateReferences(Cube ** cubes,int cubesSize);

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

    int maxCubes = 1000;
    int cubesSize = 0;
    Cube * cubes = malloc(sizeof(Cube) * maxCubes);

    if(cubes == NULL) {
        fprintf(stderr,"Malloc failed.\n");
        return EXIT_FAILURE;
    }

    for(int i = 0; i < maxCubes; i++) {
        cubes[i] = (Cube) {0,0,0}; 

        for(int j = 0; j < neighbourCount; j++) {
            cubes[i].neighbours[j] = 0;
        }
    }

    int x=-1,y=1,z=0;
    while((readSize = read(fd,&buf,1)) > 0) {

        if(buf == '.') {
            x++;
        } else if(buf == '#') {
            Cube cube = (Cube) {x,y,z};
            cubes[cubesSize] = cube;
            cubesSize++;
            x++;
        } else if(buf == '\n') {
            x = -1;
            y--;
        } else {
            fprintf(stderr,"[%s]: Unknown char %c\n",argv[0], buf);
            return EXIT_FAILURE;
        }   
    }

    for(int iteration = 0; iteration < 6; iteration++) {
        //Cycle

        updateReferences(&cubes,cubesSize);
        Cube * tmpCubes = malloc(sizeof(Cube) * maxCubes);
        int tmpCubeIndex = 0;
        for(int i = 0; i < maxCubes; i++) {
            tmpCubes[i] = (Cube) {0,0,0};
            for(int j = 0; j < neighbourCount; j++) {
                tmpCubes[i].neighbours[j] = 0;
            }
        }

        //Still ative
        for(int cubeIndex = 0; cubeIndex < cubesSize; cubeIndex++) {
            int count = 0;
            for(int neighIndex = 0; neighIndex < neighbourCount; neighIndex++) {
                if(cubes[cubeIndex].neighbours[neighIndex] == 1) {
                    count++;
                }
            }
            if((count == 2) || (count == 3)) {
                tmpCubes[tmpCubeIndex] = cubes[cubeIndex];
                tmpCubeIndex++;
            }
        }

        for(int cubeIndex = 0; cubeIndex < cubesSize; cubeIndex++) {
            int startX = cubes[cubeIndex].x + (-1);
            int startY = cubes[cubeIndex].y + (-1);
            int startZ = cubes[cubeIndex].z + (-1);

            for(int neighIndex = 0; neighIndex < neighbourCount; neighIndex++) {
                int x = startX + (neighIndex % 3);
                int y = startY + ((neighIndex/3)%3);
                int z = startZ + ((neighIndex/9)%3);

                if(cubes[cubeIndex].neighbours[neighIndex] == 0) {
                    int count = 0;
                    for(int newIndex = 0; newIndex < cubesSize; newIndex++) {
                        int dx = fabs(cubes[newIndex].x-x);
                        int dy = fabs(cubes[newIndex].y-y);
                        int dz = fabs(cubes[newIndex].z-z);         

                        if(dx <= 1 && dy <= 1 && dz <= 1) {
                            count++;
                        }                    
                    }
                    if(count == 3) {
                        int found = 0;
                        for(int tmpIndex = 0; tmpIndex < tmpCubeIndex; tmpIndex++) {
                            if((tmpCubes[tmpIndex].x == x) && (tmpCubes[tmpIndex].y == y) && (tmpCubes[tmpIndex].z == z)) {
                                found = 1;
                            }
                        }
                        if(!found) {
                            tmpCubes[tmpCubeIndex] = (Cube) {x,y,z};
                            tmpCubeIndex++;
                        }
                    }
                }

            }
        }   

        free(cubes);
        cubes = tmpCubes;
        cubesSize = tmpCubeIndex;

        fprintf(stdout,"Count: %d\n",cubesSize);
    }   


    free(cubes);
    return EXIT_SUCCESS;

}

static void updateReferences(Cube ** cubes,int cubesSize) {

    for(int cubeIndex = 0; cubeIndex < cubesSize; cubeIndex++) {
        int startX = -1;
        int startY = -1;
        int startZ = -1;

        fprintf(stdout,"Cube: %d : %d %d %d \n",cubeIndex,(*cubes)[cubeIndex].x,(*cubes)[cubeIndex].y,(*cubes)[cubeIndex].z);
        fprintf(stdout,"Has neighbours: ");
        for(int neighIndex = 0; neighIndex < neighbourCount; neighIndex++) {
            int x = (*cubes)[cubeIndex].x + startX + (neighIndex % 3);
            int y = (*cubes)[cubeIndex].y + startY + ((neighIndex/3)%3);
            int z = (*cubes)[cubeIndex].z + startZ + ((neighIndex/9)%3);

            int found = 0;
            if(((*cubes)[cubeIndex].x != x) || ((*cubes)[cubeIndex].y != y) || ((*cubes)[cubeIndex].z != z)) {
                for(int inListIndex = 0; inListIndex < cubesSize; inListIndex++) {
                    if((*cubes)[inListIndex].x == x && (*cubes)[inListIndex].y == y && (*cubes)[inListIndex].z == z) {
                        found = 1;
                        (*cubes)[cubeIndex].neighbours[neighIndex] = 1;
                    }
                }
            }

            if(!found) {
                (*cubes)[cubeIndex].neighbours[neighIndex] = 0;
            } else {
                fprintf(stdout,"(%d,%d,%d,)",x,y,z);
            }
        } 
        fprintf(stdout,"\n");
    }   

}
