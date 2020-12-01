/**
 * file main.c
 * author Alexander Beiser
 * date 01.12.2020
 *
 * This file reads a file and sums two lines up, 
 * so that the sum of them is 2020 and then calcs 
 * the multiplication of those two lines.
 */
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{   
    if(argc != 2) {
        fprintf(stdout,"synopsis: %s filepath\n",argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen (argv[1], "r");

    //-----------READ_ALL_LINES-----------------------
    char *line = NULL;
    size_t lineLength = 0;

    int bufSize = 32;
    int *buf = malloc (sizeof (int) * bufSize);

    int index = 0;
    while (getline (&line, &lineLength, file) != 1)
	{

	    //base 10 conversion
	    int val = strtol (line, NULL, 10);
	    buf[index] = val;


	    if (index >= (bufSize - 1))
		{
		    int newBufSize = 2 * bufSize;
		    int *newBuf = malloc (sizeof (int) * newBufSize);

		    for (int i = 0; i < bufSize; i++)
			{
			    newBuf[i] = buf[i];
			}

		    free (buf);

		    buf = NULL;
            buf = newBuf;
            
            bufSize = newBufSize;
		}
        
	    index++;
	}
    free(line);
    fclose(file);


    int solFound = 0, solFound2 = 0;
    int solution = 2020;

    for (int i = 0; i < index; i++)
	{
        
        for(int j = (i+1); j < index; j++) {
            if((buf[i] + buf[j]) == solution) {
                
                solFound = buf[i] * buf[j];            
                fprintf(stdout,"[%s]: Solution (2) found with vars: %d %d \n", argv[0], buf[i], buf[j]);
                fprintf(stdout,"[%s]: %d \n",argv[0], solFound);
            }
            //Checking for part b (three vals)
            for(int k = (j + 1); k < index; k++) {
                if((buf[i] + buf[j] + buf[k]) == solution) {
                    solFound2 = (buf[i] * buf[j] * buf[k]);
                    fprintf(stdout,"[%s]: Solution (3) found with vars: %d %d %d \n", argv[0], buf[i], buf[j], buf[k]);
                    fprintf(stdout,"[%s]: %d \n",argv[0], solFound2);
                }   

            }
        }
        if((solFound != 0) && (solFound2 != 0)) {
            break;
        }
	}

    


    free(buf);

    return EXIT_SUCCESS;
}
