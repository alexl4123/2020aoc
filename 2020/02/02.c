/**
 * @file: 02.c
 * @author: Alexander Beiser
 * @date: 02.12.2020
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//-----------------STATIC_DEFINITIONS---------------------------------

static int isLevelOneValidLine (int bounds[2], char pswPol, char *line);
static int isLevelTwoValidLine (int bounds[2], char pswPol, char *line);

//------------------MAIN------------------------------------------------

int main (int argc, char *argv[])
{

    //Input validation    
    if (argc != 4)
	{
	    fprintf (stderr, "[%s]: Error, synopsis: %s -l level file\n",
		     argv[0], argv[0]);
	    return EXIT_FAILURE;
	}

    int opt, level;
    while ((opt = getopt (argc, argv, "l:")) != -1)
	{
	    switch (opt)
		{
		case 'l':
		    level = strtol (optarg, NULL, 10);
		    if ((level > 2) || (level < 1))
			{
			    fprintf (stderr,
				     "[%s]: Invalid input argument value: %s.\n",
				     argv[0], optarg);
			    return EXIT_FAILURE;
			}
		    break;
		default:
		    fprintf (stderr, "[%s]: Usage: %s -l level input\n",
			     argv[0], argv[0]);
		    return EXIT_FAILURE;
		}
	}

    int validLines = 0;
    char *line = NULL;
    size_t len;

    char *delim1 = " ", *delim2 = "-";
    FILE *file = fopen (argv[optind], "r");

    if (file == NULL)
	{
	    fprintf (stderr,
		     "[%s]: Error opening file: %s (check if exists).\n",
		     argv[0], argv[optind]);
	    return EXIT_FAILURE;
	}

    while (getline (&line, &len, file) != -1)
	{

	    //-----------SPLIT_LINE------------------------

	    char *first = strtok (line, delim1);
	    char *second = strtok (NULL, delim1);
	    char *third = strtok (NULL, delim1);

	    if ((first == NULL) || (second == NULL) || (third == NULL)
		|| (strtok (NULL, delim1) != NULL))
		{
		    if (strlen (line) > 0)
			{
			    fprintf (stderr,
				     "[%s]: Error, could not split line: %s \n",
				     argv[0], line);
			}
		    free (line);
		    return EXIT_FAILURE;
		}

	    //---------------GET_MIN_MAX------------------------

	    int bounds[2] = { 0, 0 };
	    char *lowerString = strtok (first, delim2);
	    char *upperString = strtok (NULL, delim2);

	    if ((lowerString == NULL) || (upperString == NULL))
		{
		    fprintf (stderr,
			     "[%s]: Error, could not parse input at line: %s \n",
			     argv[0], line);
		    free (line);
		    return EXIT_FAILURE;
		}

	    //Convert to int
	    bounds[0] = strtol (lowerString, NULL, 10);
	    bounds[1] = strtol (upperString, NULL, 10);


	    //----------------READ_CHAR--------------------------

	    char pswPol = second[0];

	    //----------------Check_String

	    if (level == 1)
		{
		    validLines =
			validLines + isLevelOneValidLine (bounds, pswPol,
							  third);
		}
	    else if (level == 2)
		{
		    validLines =
			validLines + isLevelTwoValidLine (bounds, pswPol,
							  third);
		}
	    else
		{
		    fprintf (stderr, "[%s]: Unexpected error.", argv[0]);
		    return EXIT_FAILURE;
		}

	}

    fprintf (stdout, "[%s]: There are %d valid lines. \n", argv[0],
	     validLines);
    
    fclose(file);
    
    return EXIT_SUCCESS;
}

//-------------------------STATIC_IMPLEMENTATIONS--------------------------

static int isLevelOneValidLine (int bounds[2], char pswPol, char *third)
{
    int count = 0;
    for (int i = 0; i < strlen (third); i++)
	{
	    if (third[i] == pswPol)
		{
		    count++;
		}
	}

    if (count >= bounds[0] && count <= bounds[1])
	{
	    return 1;
	}
    else
	{
	    return 0;
	}
}

static int isLevelTwoValidLine (int bounds[2], char pswPol, char *third)
{
    if (third[bounds[0] - 1] == pswPol)
	{
	    if (third[bounds[1] - 1] != pswPol)
		{
		    return 1;
		}
	    else
		{
		    return 0;
		}
	}
    else
	{
	    if (third[bounds[1] - 1] == pswPol)
		{
		    return 1;
		}
	    else
		{
		    return 0;
		}
	}
}
