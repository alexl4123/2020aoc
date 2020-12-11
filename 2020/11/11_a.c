#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>


//-----------------------------------------------------------------------

static int allAdjacentSeatsEmpty(int seats[100][100],int rowSize, int colSize, int y, int x);

static int fourOrMoreAdjacentOcc(int seats[100][100],int rowSize, int colSize, int y, int x);

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
        } else if(buf == '\n') {
            rowSize++;
            curRow++;
            curCol = 0;
        }

    }

    fprintf(stdout,"%d-%d\n",rowSize,colSize);
    int changed = 1;
    int occ = 0;

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
                } else if((seats[y][x] == 2) && fourOrMoreAdjacentOcc(seats,rowSize,colSize,y,x)) {
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
    int count = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
                if((y+i < rowSize) && ((y+i) >= 0) && ((x+j) < colSize) && ((x+j) >= 0)) {
                    if((seats[y+i][x+j] == 1) || (seats[y+i][x+j] == 0)) {
                        if((i != 0) || (j != 0)) {
                        count++;
                        }
                    }

                } else {
                    count++;
                }
           
        }
    }
    
    if(count >= 8) {
        return 1;
    } else {
        return 0;
    }

}

static int fourOrMoreAdjacentOcc(int seats[100][100],int rowSize, int colSize, int y, int x) {
    int count = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i != 0 || j != 0) {
                if((y+i < rowSize) && ((y+i) >= 0) && ((x+j) < colSize) && ((x+j) >= 0)) {
                    if(seats[y+i][x+j] == 2) {
                        count++;
                    }

                }
            }
        }
    }

    if(count >= 4) {
        return 1;
    } else {
        return 0;
    }

}
