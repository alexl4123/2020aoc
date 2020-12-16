#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include<math.h>
#include <limits.h>

typedef struct Field {
    int id;
    int lowerA;
    int higherA;
    int lowerB;
    int higherB;
} Field;

typedef struct Ticket {
    int id;
    int * values;
} Ticket;


//--------------------------------------------

int main (int argc, char * argv[]) {

if(argc != 2) {
    fprintf(stderr,"Wrong arguments given!\n");
}

char * fileName = argv[1];

int fd = open(fileName,O_RDONLY);

if(fd == -1)  {
    fprintf(stderr,"Could not open file!\n");
    return EXIT_FAILURE;
}


char buf;
ssize_t readSize;

int maxFieldsSize = 40;
Field * fields = malloc(sizeof(Field) * maxFieldsSize);

if(fields == NULL) {
    fprintf(stderr,"Malloc failed.\n");
    return EXIT_FAILURE;
}

for(int i = 0; i < maxFieldsSize; i++) {
    fields[i] = (Field) {0,0,0,0,0};
}

long curNum = 0;

Field curField = (Field) {-1,-1,-1,-1,-1};
Ticket * myTicket = NULL;
int mode = 0;

Ticket * tickets = NULL;
int maxTicketCount = 300;


char prev[3];
for(int i = 0; i < 3; i++) {
    prev[i] = 0;
}

int fieldCount = 0;
int id = 0;
int ticketSize = 0;
int ticketPos = 0;

while((readSize = read(fd,&buf,1)) > 0) {
    if(buf >= '0' && buf <= '9') {
        curNum = (10*curNum) + (buf-'0');
    } else if(buf == '\n' && prev[0] == '\n') {
        if(mode == 0) {
            fieldCount = id;

            myTicket = malloc(sizeof(int) * (fieldCount+1));
            myTicket->id = 0;
            myTicket->values = malloc(sizeof(int) * fieldCount);

            mode = 2;
        } else {
            tickets = malloc(sizeof(int) * (1+fieldCount) *  maxTicketCount);
            for(int i = 0; i < maxTicketCount; i++) {
                tickets[i].values = malloc(sizeof(int) * fieldCount);
                tickets[i].id = -1;
            }
            id = 0;
            mode = 3;
        }
    } else if(mode == 0) {
        if(buf == '-') {
            curField.lowerA = curNum;
            curNum = 0;    
        } else if(buf == ' ' && prev[0] == 'r' && prev[1] == 'o' && prev[2] == ' ') {
            curField.higherA = curNum;
            curNum = 0;
            mode = 1;
        }
    } else if(mode == 1) {
        if(buf == '-') {
            curField.lowerB = curNum;
            curNum = 0;
        } else if('\n') {
            curField.higherB = curNum;
            curField.id = id;
            fields[id] = curField;
            id++;

            curNum = 0;
            mode = 0;
        }
    } else if(mode == 2) {
        if(buf == ',') {
            myTicket->values[ticketPos] = curNum;
            curNum = 0;
            ticketPos++;                    
        } else if(buf == '\n') {
            myTicket->values[ticketPos] = curNum;
            curNum = 0;
            ticketPos = 0;
        }
    } else if(mode == 3) {
        if(buf == ',') {
            tickets[id].values[ticketPos] = curNum;
            curNum = 0;
            ticketPos++;
        } else if(buf == '\n' && prev[0] >= '0' && prev[0] <= '9') {
            tickets[id].values[ticketPos] = curNum;
            tickets[id].id = id;
            curNum = 0;
            ticketPos = 0;
            id++;
        }
    }

    for(int i = 1; i >= 0; i--) {
        prev[i+1] = prev[i];
    }
    prev[0] = buf;
}
ticketSize = id;

fprintf(stdout,"fieldCount: %d\n",fieldCount);
for(int i = 0; i < fieldCount; i++) {
    fprintf(stdout,"%d, %d-%d, %d-%d \n",fields[i].id, fields[i].lowerA,fields[i].higherA,fields[i].lowerB,fields[i].higherB);
} 

fprintf(stdout,"\n%d:",myTicket->id);
for(int i = 0; i < fieldCount; i++) {
    fprintf(stdout,"%d,",myTicket->values[i]);
}
fprintf(stdout,"\n");

fprintf(stdout,"\n Tickets: %d \n",ticketSize);
for(int i = 0; i < ticketSize; i++) {
    fprintf(stdout,"id %d: ",tickets[i].id);
    for(int j = 0; j < fieldCount; j++) {
        fprintf(stdout,"%d,",tickets[i].values[j]);
    }
    fprintf(stdout,"\n");
}
fprintf(stdout,"\n");

long errorRate = 0;
for(int i = 0; i < ticketSize; i++) {
    for(int j = 0; j < fieldCount; j++) {
        int valid = 0;
        int ticketVal = tickets[i].values[j];
        for(int k = 0; k < fieldCount; k++) {
            if((ticketVal >= fields[k].lowerA && ticketVal <= fields[k].higherA) || (ticketVal >= fields[k].lowerB && ticketVal <= fields[k].higherB)) {
                valid = 1;
                break;
            }
        }
        if(!valid) {
            errorRate += ticketVal;
        }
    }
}

fprintf(stdout,"Error rate: %ld \n",errorRate);

return EXIT_SUCCESS;

}


