#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//----------------TYPEDEFS_AND_CONSTANTS-----------------------------------

typedef enum Type
{ nSet = -1, nop = 0, acc = 1, jmp = 2 } Type;

typedef struct Instruction
{
    int id;
    Type type;
    int value;
} Instruction;

const char *sNop = "nop",
      *sAcc = "acc",
      *sJmp = "jmp";

//---------------STATIC_METHOD_DECLARATIONS------------------------------

static int openFileAndParseArgs(int argc, char * argv[]);

static void parseInput(int fd, Instruction ** instructions, int * size);

static void fixInput(Instruction * instructions, int size);

//------------------MAIN----------------------------------------------------------

int main (int argc, char *argv[])
{
    int fd = openFileAndParseArgs(argc,argv);
    if(fd == -1) {
        return EXIT_FAILURE;
    }

    Instruction * instructions;
    int size;
    parseInput(fd,&instructions,&size);
       
    fixInput(instructions,size);


    free (instructions);

    return EXIT_SUCCESS;
}

//-----------------------------------------------------------------------------------------

static int openFileAndParseArgs(int argc, char * argv[]) {

    //Opening file and parsing arguments.
    if (argc != 3)
	{
	    fprintf (stderr, "[%s]: Usage: %s -i <FILE>\n", argv[0], argv[0]);
	    return -1;
	}

    char *fileName = NULL;
    int opt;
    while((opt = getopt(argc,argv,"i:")) != -1) {
        switch(opt) {
            case 'i':
                fileName = optarg;
                break;
           default:
                fprintf(stderr,"[%s]: Usage: %s -i <FILE>\n",argv[0], argv[0]);
                return -1;
        }
    }

    int fd = open (fileName, O_RDONLY);

    if (fd == -1)
	{
	    fprintf (stderr, "[%s]: Error opening file %s.\n", argv[0],
		     fileName);
	}
    
    return fd;

}

//-----------------------------------------------------------------------------------------

static void parseInput(int fd, Instruction ** instructions, int * size) {
    
    int bufSize = 128;
    (*size) = 0;
    (*instructions) = malloc (sizeof (int) * 3 * bufSize);
    memset((*instructions),0,(sizeof(int) * 3 * bufSize));
    ssize_t readSize;
    char buf = 0;
    
    char last = 0, 
         secondLast = 0;
    Type curInstruction = nSet;
    int curNum = 0;
    //Read input
    while ((readSize = read (fd, &buf, 1)) > 0)
	{

	    if (buf >= 'a' && buf <= 'z')
		{
		    if (secondLast == sNop[0] && last == sNop[1]
			&& buf == sNop[2])
			{
			    curInstruction = nop;
			}
		    else if (secondLast == sAcc[0] && last == sAcc[1]
			     && buf == sAcc[2])
			{
			    curInstruction = acc;
			}
		    else if (secondLast == sJmp[0] && last == sJmp[1]
			     && buf == sJmp[2])
			{
			    curInstruction = jmp;
			}
		}
	    else if (buf >= '0' && buf <= '9')
		{
            int cur = buf - '0';

		    if (curNum >= 0)
			{
			    curNum = (10 * curNum) + cur;
			}
		    else
			{
			    curNum = (10 * curNum) - cur;
			}
		    if (last == '-')
			{
			    curNum = curNum * (-1);
			}
		}
	    else if (buf == '\n')
		{

		    (*instructions)[(*size)] = (Instruction)
		    {
		    (*size), curInstruction, curNum};

		    curNum = 0;
		    curInstruction = nSet;

		    (*size)++;
		}

	    if ((*size) >= (bufSize - 1))
		{
		    //Enlarge buf.
		    int newBufSize = bufSize * 2;
		    Instruction *newInstructions =
			malloc (sizeof (int) * 3 * newBufSize);

            for(int i = 0; i < (*size); i++) {
                newInstructions[i] = (*instructions)[i];
            }

		    free ((*instructions));

		    (*instructions) = newInstructions;

		    bufSize = newBufSize;
		}
	    secondLast = last;
	    last = buf;
	}

    close(fd);
}

//-----------------------------------------------------------------------------------

static void fixInput(Instruction * instructions, int size) {
  for (int j = 0; j < size; j++)
	{

	    fprintf (stdout, "<Evaluating>Id: %d, type: %d, num: %d\n",
		     instructions[j].id, instructions[j].type,
		     instructions[j].value);
	    if (instructions[j].type == nop)
		{
		    instructions[j].type = jmp;
		}
	    else if (instructions[j].type == jmp)
		{
		    instructions[j].type = nop;
		}


	    int flag = 1;
	    int stackPointer = 0;
	    Instruction *instructionStack =
		malloc (sizeof (int) * 3 * (size));
	    int globalVal = 0;
	    int curPos = 0;
	    while (flag == 1)
		{

		    instructionStack[stackPointer] = instructions[curPos];
		    stackPointer++;

		    if (instructions[curPos].type == nop)
			{
			    curPos++;
			}
		    else if (instructions[curPos].type == jmp)
			{
			    curPos = curPos + instructions[curPos].value;
			}
		    else if (instructions[curPos].type == acc)
			{
			    globalVal =
				globalVal + instructions[curPos].value;
			    curPos++;
			}

		    if (curPos >= size)
			{
			    fprintf (stdout, "Setting quit flag\n");
			    flag = 2;
			}
		    else
			{

			    for (int i = 0; i < stackPointer; i++)
				{
				    if (instructions[curPos].id ==
					instructionStack[i].id)
					{
					    fprintf(stdout,"Setting duplicate flag.\n");
					    flag = 0;
					}
				}
			}
		}

	    free (instructionStack);
	    if (flag == 2)
		{
		    fprintf (stdout, "!!Error at:\n");
		    fprintf (stdout, "Id: %d, type: %d, num: %d\n",
			     instructions[j].id, instructions[j].type,
			     instructions[j].value);
		    fprintf (stdout, "<!Global val is: %d>\n", globalVal);
		    break;
		}

	    if (instructions[j].type == nop)
		{
		    instructions[j].type = jmp;
		}
	    else if (instructions[j].type == jmp)
		{
		    instructions[j].type = nop;
		}
	}



}
