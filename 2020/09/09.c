#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

int main (int argc, char *argv[])
{

    //Input validation

    if (argc != 4)
	{
	    fprintf (stderr,
		     "[%s]: Error: Usage %s -i <FILE> <NumberOfLinesForCalculation\n",
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
			     "[%s]: Usage: %s -i <FILE> <NumberOfLinesForCalculation>\n",
			     argv[0], argv[0]);
		    return EXIT_FAILURE;
		}
	}

    int indexPosArg = optind;
    int numbersConsidered = -1;
    char *errBuf = NULL;

    if (argv[indexPosArg] != NULL)
	{
	    numbersConsidered = strtol (argv[indexPosArg], &errBuf, 10);
	}

    if (fileName == NULL || numbersConsidered < 1 || strlen (errBuf) > 0)
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


    int listSize = 128;
    long *preamble = malloc (sizeof (long) * listSize);
    int inputSize = 0;

    long curNum = 0;
    while ((readBytes = read (fd, &buf, 1)) > 0)
	{

	    if (buf >= '0' && buf <= '9')
		{
		    int bufAsInt = buf - '0';
		    curNum = (curNum * 10) + bufAsInt;
		}
	    else if (buf == '\n')
		{
		    preamble[inputSize] = curNum;
		    curNum = 0;
		    inputSize++;
		}
	    else
		{
		    fprintf (stderr, "<<OTHER_CHAR: %c>>\n", buf);
		}

	    if (inputSize >= (listSize - 1))
		{
		    int newListSize = listSize * 2;
		    long *preamble2 = malloc (sizeof (long) * newListSize);

		    for (int i = 0; i < inputSize; i++)
			{
			    preamble2[i] = preamble[i];
			}

		    free (preamble);

		    preamble = preamble2;
		    listSize = newListSize;
		}
	}

    //------------GET_INVALID_NUM------------------------------------

    int invalidNum = -1;

    for (int i = 0; i < inputSize; i++)
	{
	    if (i >= numbersConsidered)
		{
		    int start = i - numbersConsidered;
		    int end = i - 1;

		    int doesItSumUp = 0;
		    for (int j = start; j <= end; j++)
			{
			    for (int k = j + 1; k <= end; k++)
				{
				    if ((preamble[j] + preamble[k]) ==
					preamble[i])
					{
					    doesItSumUp = 1;
					}
				}
			}

		    if (doesItSumUp == 0)
			{
			    fprintf (stdout, ">! %ld line num %d!<\n",
				     preamble[i], i);
			    invalidNum = preamble[i];
			}
		}
	}

    //---------------------CALC_SUB_SUM------------------------------------

    for (int i = 0; i < inputSize; i++)
	{

	    int curIndex = i;
	    long curSum = 0;

	    long min = LONG_MAX;
	    long max = LONG_MIN;

	    while (curSum < invalidNum)
		{
		    if (preamble[curIndex] < min)
			{
			    min = preamble[curIndex];
			}
		    if (preamble[curIndex] > max)
			{
			    max = preamble[curIndex];
			}
		    curSum = curSum + preamble[curIndex];
		    curIndex++;

		}
	    curIndex--;

	    if (curSum == invalidNum && (i != curIndex))
		{
		    fprintf (stdout, ">!!%ld -> %ld -> %ld || %d -> %d !!<\n",
			     preamble[i], preamble[curIndex],
			     (preamble[i] + preamble[curIndex]), i, curIndex);
		    fprintf (stdout, ">!!MIN: %ld MAX: %ld SUM: %ld <!!\n",
			     min, max, (min + max));
		}
	}

    free (preamble);

    return EXIT_SUCCESS;
}
