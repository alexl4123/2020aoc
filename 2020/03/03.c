/**
 * @File 03.c
 * @Author Alexander Beiser
 * @Date 03.12.2020
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

//-------------------STATIC-----------------------------

static int traversing (int fd, int inc, int lines);

typedef struct Traversal
{

    int right;
    int down;
    long output;
} Traversal;
//---------------------MAIN-------------------------------

int main (int argc, char *argv[])
{

    //---------------PARSE_INPUT_ARGS---------------------
    if (argc != 3)
	{
	    fprintf (stderr, "Usage: %s -i inputFile\n", argv[0]);
	    return EXIT_FAILURE;

	}

    int opt = 0;
    char *inputFilePath = NULL;

    while ((opt = getopt (argc, argv, "i:")) != -1)
	{
	    switch (opt)
		{
		case 'i':
		    inputFilePath = optarg;
		    break;
		default:
		    fprintf (stderr, "Usage: %s -i inputFile\n", argv[0]);
		    return EXIT_FAILURE;
		}
	}

    //--------------OPEN_FILE-------------------------------

    int fd = open (inputFilePath, O_RDWR);

    if (fd == -1)
	{
	    fprintf (stderr, "[%s]: Error opening file - %s\n", argv[0],
		     inputFilePath);
	    return EXIT_FAILURE;
	}

    //------------------PROGRAM-------------------------------


    Traversal traversals[5];
    traversals[0] = (Traversal) {1, 1, 0};
    traversals[1] = (Traversal) {3, 1, 0};
    traversals[2] = (Traversal) {5, 1, 0};
    traversals[3] = (Traversal) {7, 1, 0};
    traversals[4] = (Traversal) {1, 2, 0};

    long mult = 1;

    for (int i = 0; i < 5; i++)
	{
	    traversals[i].output =
		traversing (fd, traversals[i].right, traversals[i].down);
	    mult = (mult * traversals[i].output);

	    fprintf (stdout,
		     "[%s]: The tree count for right: %d down: %d is: %ld\n",
		     argv[0], traversals[i].right, traversals[i].down,
		     traversals[i].output);
	}

    fprintf (stdout, "[%s]: The tree mult is: %ld\n", argv[0], mult);

    //----------------CLEANUP--------------------------------

    close (fd);
    return EXIT_SUCCESS;
}

static int traversing (int fd, int inc, int lines)
{
    //Set file position to beginning of file.
    lseek (fd, 0, SEEK_SET);

    char buf, tree = '#', noTree = '.';

    int curLineIndex = 0;

    int nextPos = inc;

    int lineCount = 0;

    int treeCount = 0;

    //boolean
    int lineRead = lines;


    while (read (fd, &buf, 1) > 0)
	{

	    if ((nextPos == curLineIndex) && (lineRead == 0) && (buf != '\n'))
		{
		    if (buf == tree)
			{
			    treeCount++;
			}
		    else
			{
			}

		    nextPos += inc;
		    lineRead = lines;
		}
	    else if (buf != '\n')
		{
		}


	    if (buf == '\n')
		{
		    if (nextPos >= curLineIndex)
			{
			    nextPos = nextPos % (curLineIndex);
			}
		    curLineIndex = 0;

		    lineRead--;
		    lineCount++;
		}
	    else
		{
		    curLineIndex++;
		}
	}
    return treeCount;
}
