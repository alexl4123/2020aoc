#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <regex.h>

regex_t regex;
int reti;

typedef struct Passport
{
    int sByr;
    int sIyr;
    int sEyr;
    int sHgt;
    int sHcl;
    int sEcl;
    int sPid;
    int sCid;

    char *byr;
    char *iyr;
    char *eyr;
    char *hgt;
    char *hcl;
    char *ecl;
    char *pid;
    char *cid;

} Passport;

int main (int argc, char *argv[])
{

    //------------Parse_input_arguments-------------------

    if (argc != 3)
	{
	    fprintf (stderr, "[%s]: Usage: %s -i input\n", argv[0], argv[0]);
	}

    int opt;
    char *inputFilename = NULL;

    while ((opt = getopt (argc, argv, "i:")) != -1)
	{
	    switch (opt)
		{
		case 'i':
		    inputFilename = optarg;
		    break;
		default:
		    fprintf (stderr, "[%s]: Usage: %s -i input\n", argv[0],
			     argv[0]);
		    return EXIT_FAILURE;
		}
	}

    //-------------------------------------------------------

    int fd = open (inputFilename, O_RDONLY);

    if (fd == -1)
	{
	    fprintf (stderr, "[%s]: Error opening file: %s\n", argv[0],
		     inputFilename);
	    return EXIT_FAILURE;
	}

    //--------------------------------------------------------

    char buf;

    //----------------
    //constants
    const char *byr = "byr", *iyr = "iyr", *eyr = "eyr", *hgt = "hgt", *hcl =
	"hcl", *ecl = "ecl", *pid = "pid", *cid = "cid";
    Passport curPassport;
    //----------------

    const char *arr[7];
    arr[0] = "byr";
    arr[1] = "iyr";
    arr[2] = "eyr";
    arr[3] = "hgt";
    arr[4] = "hcl";
    arr[5] = "ecl";
    arr[6] = "pid";
    //cid not needed

    //Initialized with 0
    int sArr[7];
    char lRead[4];

    int readingProp = 1;
    int validPassportCount = 0;

    char last, beforeLast;

    //pid is biggest with 9 chars
    char bigBuf[9];
    int curCharCount;

    while (read (fd, &buf, 1) > 0)
	{
	    fprintf (stdout, "%c", buf);

	    if (readingProp == 1)
		{
		    char read[4];
		    read[0] = beforeLast;
		    read[1] = last;
		    read[2] = buf;
		    for (int i = 0; i < 7; i++)
			{

			    if (strcmp (arr[i], read) == 0)
				{
				    //If property found, set true
				    sArr[i] = 1;


				    lRead[0] = read[0];
				    lRead[1] = read[1];
				    lRead[2] = read[2];
				    lRead[3] = '\0';
				    //Not reading properties anymore
				    readingProp = 0;
				}
			}
		}
	    else if (readingProp == 2 && curCharCount < 9)
		{

		    bigBuf[curCharCount] = buf;
		    curCharCount++;
		}
	    else if (readingProp == 2 && curCharCount >= 9)
		{
		    curCharCount++;
		}

	    if (buf == ':')
		{
		    readingProp = 2;
		}


	    if (buf == ' ' || buf == '\n')
		{
		    if (readingProp == 2)
			{
			    if (strcmp (lRead, arr[0]) == 0
				&& curCharCount == 5)
				{
				    //byr
				    int i = strtol (bigBuf, NULL, 10);
				    if (i >= 1920 && i <= 2002)
					{
					    sArr[0] = 2;
					    fprintf (stdout, "(valid)");
					}
				}
			    else if (strcmp (lRead, arr[1]) == 0
				     && curCharCount == 5)
				{
				    //iyr
				    int i = strtol (bigBuf, NULL, 10);
				    if (i >= 2010 && i <= 2020)
					{
					    sArr[1] = 2;
					    fprintf (stdout, "(valid)");
					}
				}
			    else if (strcmp (lRead, arr[2]) == 0
				     && curCharCount == 5)
				{
				    //eyr
				    int i = strtol (bigBuf, NULL, 10);
				    if (i >= 2020 && i <= 2030)
					{
					    sArr[2] = 2;
					    fprintf (stdout, "(valid)");
					}
				}
			    else if (strcmp (lRead, arr[3]) == 0)
				{
				    //hgt
				    int i = strtol (bigBuf, NULL, 10);

				    if (bigBuf[3] == 'c' && bigBuf[4] == 'm')
					{
					    if (i >= 150 && i <= 193)
						{
						    sArr[3] = 2;
						    fprintf (stdout,
							     "(valid)");
						}
					}
				    else if (bigBuf[2] == 'i'
					     && bigBuf[3] == 'n')
					{
					    if (i >= 59 && i <= 76)
						{
						    sArr[3] = 2;
						    fprintf (stdout,
							     "(valid)");
						}
					}

				}
			    else if (strcmp (lRead, arr[4]) == 0
				     && curCharCount == 8)
				{
				    //hcl
				    if (bigBuf[0] == '#')
					{

					    int s = 0;
					    for (int i = 1; i < 7; i++)
						{
						    if ((bigBuf[i] >= 48
							 && bigBuf[i] <= 57)
							|| (bigBuf[i] >= 97
							    && bigBuf[i] <=
							    122))
							{
							    s++;
							}
						}

					    if (s == 6)
						{
						    sArr[4] = 2;
						    fprintf (stdout,
							     "(valid)");
						}
					}

				}
			    else if (strcmp (lRead, arr[5]) == 0
				     && curCharCount == 4)
				{
				    //ecl
				    char str[3];
				    str[0] = bigBuf[0];
				    str[1] = bigBuf[1];
				    str[2] = bigBuf[2];

				    if ((strcmp (str, "amb") == 0)
					|| (strcmp (str, "blu") == 0)
					|| (strcmp (str, "brn") == 0)
					|| (strcmp (str, "gry") == 0)
					|| (strcmp (str, "grn") == 0)
					|| (strcmp (str, "hzl") == 0)
					|| (strcmp (str, "oth") == 0))
					{
					    sArr[5] = 2;
					    fprintf (stdout, "(valid)");
					}
				    fprintf (stdout, "(valid)");
				}
			    else if (strcmp (lRead, arr[6]) == 0
				     && curCharCount == 10)
				{
				    //pid
				    int s = 0;
				    for (int i = 0; i < 9; i++)
					{
					    if ((bigBuf[i] >= 48
						 && bigBuf[i] <= 57))
						{
						    s++;
						}
					}

				    if (s == 9)
					{
					    sArr[6] = 2;
					    fprintf (stdout, "(valid)");
					}

				}


			    curCharCount = 0;
			    for (int i = 0; i < 9; i++)
				{
				    bigBuf[i] = 0;
				}
			}
		    readingProp = 1;
		}
	    if (buf == '\n' && last == '\n')
		{
		    fprintf (stdout, "(nextpsw)");
		    int sum = 0;
		    //Empty line aka new passport.
		    for (int i = 0; i < 7; i++)
			{
			    sum += sArr[i];
			    sArr[i] = 0;
			}
		    fprintf (stdout, "(%d)", sum);
		    if (sum == 14)
			{
			    fprintf (stdout, "(isvalid)");
			    validPassportCount++;
			}
		}

	    if (readingProp == 1)
		{
		    beforeLast = last;
		    last = buf;
		}


	}

    fprintf (stdout, "[%s]: There are %d valid passports.\n", argv[0],
	     validPassportCount);

    return EXIT_SUCCESS;

}
