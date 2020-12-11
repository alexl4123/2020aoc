#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>


//-----------------------------------------------------------------------

static int allAdjacentSeatsEmpty(int seats[100][100],int rowSize, int colSize, int y, int x);


//-----------------------------------------------------------------------

int main (int argc, char *argv[])
{

    //Input validation

    if (argc != 3)
    {
        fprintf (stderr,
                "[%s]: Error: Usage %s -i <FILE> \n",
                argv[0], argv[0]);
        return EXIT_FAILURE;
    }

    int opt;
    char *fileName = NULL;

    while ((opt = getopt (argc, argv, "i:")) != -1)
    {
        switch (opt)
        {
            case 'i':
                fileName = optarg;
                break;
            default:
                fprintf (stderr,
                        "[%s]: Usage: %s -i <FILE> \n",
                        argv[0], argv[0]);
                return EXIT_FAILURE;
        }
    }



    if (fileName == NULL)
    {
        fprintf (stderr,
                "[%s]: Something wrong with the option -i or one argument given.\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    //-----------------READ_FROM_FILE-----------------------------------------------------

    char buf;
    ssize_t readBytes;

    int fd = open (fileName, O_RDONLY);

    if (fd == -1)
    {
        fprintf (stderr, "[%s]: Error opening file: %s", argv[0],
                fileName);
        return EXIT_FAILURE;
    }


    int curCol = 0, curRow = 0;
    int colSize = 0, rowSize = 0;

    int seats[100][100];

    while ((readBytes = read (fd, &buf, 1)) > 0)
    {

        if(buf == 'L') {
            seats[curRow][curCol] = 1;
            curCol++;
            if(curCol > colSize) {
                colSize = curCol;
            }
        } else if(buf == '.') {
            seats[curRow][curCol] = 0;
            curCol++;
            if(curCol > colSize) {
                colSize = curCol;
            }
        } else if(buf == '#') {
            seats[curRow][curCol] = 2;
            curCol++;
            if(curCol > colSize) {
                colSize = curCol;
            }

        } else if(buf == '\n') {
            rowSize++;
            curRow++;
            curCol = 0;
        }

    }

    fprintf(stdout,"%d-%d\n",rowSize,colSize);
    int changed = 1;
    int occ = 0;

    for(int y = 0; y < rowSize; y++) {
        for(int x = 0; x < colSize; x++) {
            fprintf(stdout,"%d",seats[y][x]);
        }
        fprintf(stdout,"\n");
    }
    fprintf(stdout,"\n%d\n",seats[4][3]);

    allAdjacentSeatsEmpty(seats,rowSize,colSize,4,3);

    while(changed > 0) {
        int newSeats[100][100];
        for(int y = 0; y < rowSize; y++) {
            for(int x = 0; x < colSize; x++) {
                newSeats[y][x] = seats[y][x];
            }   
        }


        changed = 0;
        occ = 0;

        for(int y = 0; y < rowSize; y++) {
            for(int x = 0; x < colSize; x++) {

                fprintf(stdout,"%d",seats[y][x]);
                if((seats[y][x] == 1) && allAdjacentSeatsEmpty(seats,rowSize,colSize,y,x)) {
                    newSeats[y][x] = 2;
                    changed++;
                } else if((seats[y][x] == 2) && allAdjacentSeatsEmpty(seats,rowSize,colSize,y,x)) {
                    newSeats[y][x] = 1;
                    changed++;
                }

                if(seats[y][x] == 2) {
                    occ++;
                }

            }
            fprintf(stdout,"\n");
        }

        for(int y = 0; y < rowSize; y++) {
            for(int x = 0; x < colSize; x++) {
                seats[y][x] = newSeats[y][x];

            }
        }

        fprintf(stdout,"\nChanged: %d\n",changed);
    }

    fprintf(stdout,"Occ: %d\n",occ);

    return EXIT_SUCCESS;
}

static int allAdjacentSeatsEmpty(int seats[100][100],int rowSize, int colSize, int y, int x) {

    int vissibleSeats[8];

    //0 -> right
    for(int i = 1; i < colSize; i++) {
        if((i+x) < colSize && seats[y][x+i] != 0) {
            vissibleSeats[0] = seats[y][x+i];
            break;
        } else if((i+x) == colSize) {
            vissibleSeats[0] = 0;
            break;
        }   
    } 

    //1 -> Right down 
    for(int i = 1; i < rowSize; i++) {
        if((i+y) < rowSize && (i+x) < colSize && seats[y+i][x+i] != 0) {
            vissibleSeats[1] = seats[y+i][x+i];
            break;
        } else if((i+x) == colSize || (i+y) == rowSize) {
            vissibleSeats[1] = 0;
            break;
        }   

    }

    //2 -> down
    for(int i = 1; i < rowSize; i++) {
        if((i+y) < rowSize && seats[y+i][x] != 0) {
            vissibleSeats[2] = seats[y+i][x];
            break;
        } else if((i+y) == rowSize) {
            vissibleSeats[2] = 0;
            break;
        }   
    } 

    //3 -> left down 
    for(int i = 1; i < rowSize; i++) {
        if((i+y) < rowSize && (x-i) >= 0 && seats[y+i][x-i] != 0) {
            vissibleSeats[3] = seats[y+i][x-i];
            break;
        } else if((x-i) < 0 || (i+y) == rowSize) {
            vissibleSeats[3] = 0;
            break;
        }   
    }

    //4 -> left
    for(int i = 1; i < colSize; i++) {
        if((x-i) >= 0 && seats[y][x-i] != 0) {
            vissibleSeats[4] = seats[y][x-i];
            break;
        } else if((x-i) < 0) {
            vissibleSeats[4] = 0;
            break;
        }   
    } 

    //5 -> left up 
    for(int i = 1; i < rowSize; i++) {
        if((y-i) >= 0 && (x-i) >= 0 && seats[y-i][x-i] != 0) {
            vissibleSeats[5] = seats[y-i][x-i];
            break;
        } else if((x-i) < 0 || (y-i) < 0) {
            vissibleSeats[5] = 0;
            break;
        }   
    }

    //6 -> up
    for(int i = 1; i < rowSize; i++) {
        if((y-i) >= 0 && seats[y-i][x] != 0) {
            vissibleSeats[6] = seats[y-i][x];
            break;
        } else if((y-i) < 0) {
            vissibleSeats[6] = 0;
            break;
        }   
    } 

    //7 -> right up 
    for(int i = 1; i < rowSize; i++) {
        if((y-i) >= 0 && (x+i) < colSize && seats[y-i][x+i] != 0) {
            vissibleSeats[7] = seats[y-i][x+i];
            break;
        } else if((x+i) >= colSize || (y-i) < 0) {
            vissibleSeats[7] = 0;
            break;
        }   
    } 

    /*
       fprintf(stdout,"<");
       for(int i = 0; i < 8; i++) {
       fprintf(stdout,"%d ", vissibleSeats[i]);
       }
       fprintf(stdout,">\n");
       */

    int count = 0;
    for(int i = 0; i < 8; i++) {
        if(vissibleSeats[i] == 2) {
            count++;
        }
    }

    if(seats[y][x] == 1) {
        if(count == 0) {
            return 1;
        }

    } else if(seats[y][x] == 2) {
        if(count >= 5) {
            return 1;
        }
    }

    return 0;

}
