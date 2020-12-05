#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>

int main (int argc, char *argv[])
{

    if (argc != 3)
	{
	    fprintf (stderr, "[%s]: Usage: %s -i Input\n", argv[0], argv[0]);
	    return EXIT_FAILURE;
	}

    char *inputFileName = NULL;
    int opt;

    while ((opt = getopt (argc, argv, "i:")) != -1)
	{
	    switch (opt)
		{
		case 'i':
		    inputFileName = optarg;
		    break;
		default:
		    fprintf (stderr, "[%s]: Usage %s -i Input\n", argv[0],
			     argv[0]);
		    return EXIT_FAILURE;
		}
	}

    //--------OPEN_FILE------------------ 

    int fd = open (inputFileName, O_RDONLY);

    if (fd < 0)
	{
	    fprintf (stderr, "[%s]: Could not open file %s.\n", argv[0],
		     inputFileName);
	    return EXIT_FAILURE;
	}

    //---------------------------------

    char buf;
    ssize_t count;

    int front = 0, back = 127, left = 0, right = 7;

    int row = 0, col = 0;
    int maxId = 0, minId = INT_MAX;

    int bufSize = 32;
    int *arrBuf = malloc (sizeof (int) * bufSize);
    memset (arrBuf, 0, sizeof (int) * bufSize);

    //Read to end of file.
    while ((count = read (fd, &buf, 1)) > 0)
	{
	    if (buf == 'F')
		{
		    //E.g. half of 127 -> 128 -> 64 -> 63
		    back = back - (((back - front) + 1) / 2);
		}
	    else if (buf == 'B')
		{
		    //E.g. half of 127 -> 128 -> 64
		    front = front + (((back - front) + 1) / 2);
		}
	    else if (buf == 'L')
		{
		    right = right - (((right - left) + 1) / 2);
		}
	    else if (buf == 'R')
		{
		    left = left + (((right - left) + 1) / 2);
		}
	    else if (buf == '\n')
		{
		    //End of line

		    if (front == back)
			{
			    row = front;
			}
		    if (left == right)
			{
			    col = left;
			}

		    int id = ((row * 8) + col);
		    if (id > maxId)
			{
			    maxId = id;
			}
		    if (id < minId)
			{
			    minId = id;
			}

		    if (id > bufSize)
			{
			    int newBufSize = (2 * id);
			    int *newBuf = malloc (sizeof (int) * newBufSize);

			    memset (newBuf, 0, sizeof (int) * newBufSize);
			    memcpy (newBuf, arrBuf, (sizeof (int) * bufSize));
			    free (arrBuf);
			    arrBuf = newBuf;
			    bufSize = newBufSize;
			}

		    arrBuf[id] = 1;

		    front = 0;
		    back = 127;
		    left = 0;
		    right = 7;

		    row = 0;
		    col = 0;
		}
	    else
		{
		    fprintf (stdout, "[%s]: Unexpected char: %c. \n", argv[0],
			     buf);
		}
	}

    fprintf (stdout, "[%s]: The highest seat id is: %d. \n", argv[0], maxId);

    for (int i = minId; i <= maxId; i++)
	{
	    if (arrBuf[i] == 0)
		{
		    fprintf (stdout, "[%s]: My seat: %d. \n", argv[0], i);
		}
	}

    free (arrBuf);
    if (close (fd) == -1)
	{
	    fprintf (stderr, "[%s]: Error closing file.\n", argv[0]);
	    return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}
