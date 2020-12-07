#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Container
{
    char *outerBag;
    int numberBagsOne;
    char *bagsOne;
    int numberBagsTwo;
    char *bagsTwo;
    int numberBagsThree;
    char *bagsThree;
    int numberBagsFour;
    char *bagsFour;
} Container;

static long calcB(char * bag, Container *bags[594]);

int main (int argc, char *argv[])
{

    //----------INPUT_VALIDATION-----------------------
    
    if (argc != 3)
	{
	    fprintf (stderr, "Usage: %s -i <FILE>", argv[0]);
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
                fprintf(stderr,"[%s]: usage: %s -i <FILE>\n",argv[0],argv[0]);
                return EXIT_FAILURE;
        }
    }

    //---------END_INPUT_VALIDATION-------------------

    FILE *file = fopen (fileName, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    if (file == NULL)
	{
	    fprintf (stderr, "[%s]: Error opening file!\n", argv[0]);
	    return EXIT_FAILURE;
	}

    int lineCount = 594;
    Container *containers[lineCount];

    char *delim = " ";

    int count = 0;
    char *important[lineCount];

    
    int maxLength = 200;
    Container * goldenBag = malloc (sizeof (char) * maxLength * 5 + sizeof (int) * 3);
	goldenBag->outerBag = malloc (sizeof (char) * maxLength);
	goldenBag->bagsOne = malloc (sizeof (char) * maxLength);
	goldenBag->bagsTwo = malloc (sizeof (char) * maxLength);
	goldenBag->bagsThree = malloc (sizeof (char) * maxLength);
	goldenBag->bagsFour = malloc (sizeof (char) * maxLength);

    int index = 0;
    while ((nread = getline (&line, &len, file)) != -1)
	{
	    Container *container =
		malloc (sizeof (char) * strlen (line) * 5 + sizeof (int) * 3);
	    container->outerBag = malloc (sizeof (char) * strlen (line));

	    container->outerBag[0] = 0;

	    char *string = strtok (line, delim);
	    strcpy (container->outerBag, string);

	    string = strtok (NULL, delim);
	    strcat (container->outerBag, string);


	    string = strtok (NULL, delim);
	    string = strtok (NULL, delim);

	    //-------------FIRST_INNER_BAG--------------------------
	    string = strtok (NULL, delim);
	    container->numberBagsOne = strtol (string, NULL, 10);

	    if (container->numberBagsOne > 0)
		{
		    container->bagsOne =
			malloc (sizeof (char) * strlen (line));
		    container->bagsOne[0] = 0;

		    string = strtok (NULL, delim);
		    strcpy (container->bagsOne, string);

		    string = strtok (NULL, delim);
		    strcat (container->bagsOne, string);
		    //-------------SECOND_INNER_BAG--------------------------
		    string = strtok (NULL, delim);

		    if ((strncmp (string, "bag.", sizeof (char) * 4) != 0)
			&& (strncmp (string, "bags.", sizeof (char) * 5) !=
			    0))
			{

			    string = strtok (NULL, delim);
			    container->numberBagsTwo =
				strtol (string, NULL, 10);

			    if (container->numberBagsTwo > 0)
				{
				    container->bagsTwo =
					malloc (sizeof (char) *
						strlen (line));
				    container->bagsTwo[0] = 0;


				    string = strtok (NULL, delim);
				    strcpy (container->bagsTwo, string);

				    string = strtok (NULL, delim);
				    strcat (container->bagsTwo, string);

				    //-------------THIRD_INNER_BAG--------------------------
				    string = strtok (NULL, delim);

				    if ((strncmp
					 (string, "bag.",
					  sizeof (char) * 4) != 0)
					&&
					(strncmp
					 (string, "bags.",
					  sizeof (char) * 5) != 0))
					{

					    string = strtok (NULL, delim);
					    container->numberBagsThree =
						strtol (string, NULL, 10);

					    if (container->numberBagsThree >
						0)
						{
						    container->bagsThree =
							malloc (sizeof (char)
								*
								strlen
								(line));
						    container->bagsThree[0] =
							0;


						    string =
							strtok (NULL, delim);
						    strcpy (container->
							    bagsThree,
							    string);

						    string =
							strtok (NULL, delim);
						    strcat (container->
							    bagsThree,
							    string);

						    //-------------FOURTH_INNER_BAG--------------------------
						    string =
							strtok (NULL, delim);

						    if ((strncmp
							 (string, "bag.",
							  sizeof (char) *
							  4) != 0)
							&&
							(strncmp
							 (string, "bags.",
							  sizeof (char) *
							  5) != 0))
							{

							    string =
								strtok (NULL,
									delim);
							    container->
								numberBagsFour
								=
								strtol
								(string, NULL,
								 10);

							    if (container->
								numberBagsFour
								> 0)
								{
								    container->
									bagsFour
									=
									malloc
									(sizeof
									 (char)
									 *
									 strlen
									 (line));
								    container->
									bagsFour
									[0] =
									0;


								    string =
									strtok
									(NULL,
									 delim);
								    strcpy
									(container->
									 bagsFour,
									 string);

								    string =
									strtok
									(NULL,
									 delim);
								    strcat
									(container->
									 bagsFour,
									 string);
								}
							}
						}
					}

				}
			}

		}


	    if ((container->bagsOne != NULL
		 && strcmp (container->bagsOne, "shinygold") == 0)
		|| (container->bagsTwo != NULL
		    && strcmp (container->bagsTwo, "shinygold") == 0)
		|| (container->bagsThree != NULL
		    && strcmp (container->bagsThree, "shinygold") == 0)
		|| (container->bagsFour != NULL
		    && strcmp (container->bagsFour, "shinygold") == 0))
		{
		    important[count] =
			malloc (strlen (container->outerBag) * sizeof (char));
		    strcpy (important[count], container->outerBag);
		    count++;
		}

       
        if((strcmp(container->outerBag,"shinygold") == 0)) {
            memcpy(goldenBag,container,(sizeof (char) * strlen (line) * 5 + sizeof (int) * 3));
        }
        


	    containers[index] = container;
	    index++;

	}
    free (line);

    fprintf(stdout,"Shiny gold count: %d\n",count);

    int lengthFinal = 0;
    char *final[lineCount];

    while (count > 0)
	{
	    char *cur = important[count - 1];

	    final[lengthFinal] = cur;
	    lengthFinal++;


	    count--;


	    for (int i = 0; i < lineCount; i++)
		{
		    if ((containers[i]->bagsOne != NULL
			 && strcmp (containers[i]->bagsOne, cur) == 0)
			|| (containers[i]->bagsTwo != NULL
			    && strcmp (containers[i]->bagsTwo, cur) == 0)
            || (containers[i]->bagsThree != NULL
                && strcmp(containers[i]->bagsThree,cur)==0)
            || (containers[i]->bagsFour != NULL
                && strcmp(containers[i]->bagsFour,cur)==0))
			{

			    int contains = 0;
			    for (int j = 0; j < lengthFinal; j++)
				{
				    if (strcmp
					(containers[i]->outerBag,
					 final[j]) == 0)
					{
					    contains = 1;
					    break;
					}
				}

                for(int j = 0; j < count; j++) {
                    if(strcmp(containers[i]->outerBag,
                                important[j]) == 0) {
                        contains = 1;
                        break;
                    }
                }
			    if (contains == 0)
				{
				    important[count] =
					malloc (strlen
						(containers[i]->outerBag) *
						sizeof (char));

				    strcpy (important[count],
					    containers[i]->outerBag);
				    count++;
				}
			}
		}
	}

    fprintf (stdout, "Count: %d\n", lengthFinal);

    
    //---------------------B---------------------------------   
    
    fprintf(stdout,"B: %ld\n",calcB("shinygold",containers)-1);

    //-----_FREE_--------------------
    for (int i = 0; i < lineCount; i++)
	{
	    if (containers[i]->outerBag != NULL)
		{
		    free (containers[i]->outerBag);
		}

	    if (containers[i]->bagsOne != NULL)
		{
		    free (containers[i]->bagsOne);
		}
	    if (containers[i]->bagsTwo != NULL)
		{
		    free (containers[i]->bagsTwo);
		}
	    free (containers[i]);
	}

}

//594 is line count #HACK
static long calcB(char * bag,Container *bags[594]) {
    Container * curBag;
    for(int i = 0; i < 594; i++) {
        if(strcmp(bags[i]->outerBag,bag) == 0) {
            curBag = bags[i];
            break;
        }
    }   
    if(curBag->numberBagsOne == 0) {
        return 1;
    }

    fprintf(stdout,"%s contains %d %s\n",bag,curBag->numberBagsOne,curBag->bagsOne);
    long retVal = (curBag->numberBagsOne * calcB(curBag->bagsOne,bags));
    fprintf(stdout,"So %ld have been added.\n",retVal);

    if(curBag->numberBagsTwo > 0) {
    fprintf(stdout,"%s contains %d %s\n",bag,curBag->numberBagsTwo,curBag->bagsTwo);
        long ret = (curBag->numberBagsTwo * calcB(curBag->bagsTwo,bags));

    retVal = retVal + ret;
    fprintf(stdout,"So %ld have been added.\n",ret);
    }
    if(curBag->numberBagsThree > 0) {
    fprintf(stdout,"%s contains %d %s\n",bag,curBag->numberBagsThree,curBag->bagsThree);
    long ret = (curBag->numberBagsThree * calcB(curBag->bagsThree,bags));
    retVal = retVal + ret;
    fprintf(stdout,"So %ld have been added.\n",ret);
    }
    if(curBag->numberBagsFour > 0) {
    fprintf(stdout,"%s contains %d %s\n",bag,curBag->numberBagsFour,curBag->bagsFour);
    long ret = (curBag->numberBagsFour * calcB(curBag->bagsFour,bags));
    retVal = retVal + ret;
    fprintf(stdout,"So %ld have been added.\n",ret);

    }

    retVal = retVal + 1;
    
    return retVal;
}
