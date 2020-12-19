#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#include<math.h>
#include <limits.h>

typedef struct Rule {
    int id;
    int firstCount;
    int secondCount;
    
    char special;
    int first[4];
    int second[4];

} Rule;

typedef struct Tuple {
    int start;
    int end;
    Rule rule;
};

Rule * rules;
Rule constants[2];
int rulesLength;

int maxAllPossibilities = 1000;
char ** allPossibilities;
int allPosIndex;

static void generateAllPossibilities(char * curString, Rule * curRules, int curRulesCount) { 
    /**
    fprintf(stderr,"Cur String: %s - Cur Rules count: %d - Cur Rules: ",curString,curRulesCount);
    for(int i = 0; i < curRulesCount; i++) {
        fprintf(stderr,"%d,",curRules[i].id);
    }
    fprintf(stderr,"\n");
    */


    if(curRulesCount > 0 && curRules[curRulesCount-1].id == constants[0].id) {
        strcat(curString,"a");
        curRulesCount--;
    }

    if(curRulesCount > 0 && curRules[curRulesCount-1].id == constants[1].id) {
        strcat(curString,"b");
        curRulesCount--;
    }   
    
    if(curRulesCount == 0) {
        char * posString = malloc(sizeof(char) * (strlen(curString) + 20));
        memset(posString,0,(sizeof(char) * (strlen(curString) + 20)));
        strcat(posString,curString);
        strcat(posString,"\n");

        allPossibilities[allPosIndex] = posString;
        allPosIndex++;

        if(maxAllPossibilities <= allPosIndex) {
            int tmp = 2 * maxAllPossibilities;
            fprintf(stderr,"Increase max possiblities: from %d to %d\n",maxAllPossibilities,tmp);
            char ** allPoss = malloc(sizeof(char) * tmp * 20);
            for(int i = 0; i < allPosIndex; i++) {
                allPoss[i] = allPossibilities[i];
            }
            free(allPossibilities);
            allPossibilities = allPoss;
            maxAllPossibilities = tmp;
        }

        return;
    }

    if(curRules[curRulesCount-1].firstCount > 0) {
        char * firstString = malloc(sizeof(char) * (strlen(curString)+20));
        memset(firstString,0,(sizeof(char) * strlen(curString)));
        strcat(firstString,curString);

        int firstRulesCount = curRulesCount - 1 + curRules[curRulesCount-1].firstCount;
        Rule * firstRules = malloc(sizeof(Rule) * firstRulesCount);
        int i;
        for(i = 0; i < curRulesCount-1; i++) {
            firstRules[i] = curRules[i];
        }
        for(int j = (curRules[curRulesCount-1].firstCount-1); j >= 0;j--) {
            for(int w = 0; w < rulesLength; w++) {
                if(rules[w].id == curRules[curRulesCount-1].first[j]) {
                    firstRules[i] = rules[w];
                    i++;
                }
            }
        }

        generateAllPossibilities(firstString,firstRules,firstRulesCount);
        free(firstString);
        free(firstRules);
    }

    if(curRules[curRulesCount-1].secondCount > 0) {
        char * secondString = malloc(sizeof(char) * (strlen(curString)+20));
        memset(secondString,0,(sizeof(char) * strlen(curString)));
        strcat(secondString,curString);

        int secondRulesCount = curRulesCount - 1 + curRules[curRulesCount-1].secondCount;
        Rule * secondRules = malloc(sizeof(Rule) * secondRulesCount);
        
        int i;
        for(i = 0; i < curRulesCount-1; i++) {
            secondRules[i] = curRules[i];
        }
        for(int j = (curRules[curRulesCount-1].secondCount-1); j >=0; j--) {
            for(int w = 0; w < rulesLength; w++) {
                if(rules[w].id == curRules[curRulesCount-1].second[j]) {
                    secondRules[i] = rules[w];
                    i++;
                }
            }
        }

        generateAllPossibilities(secondString,secondRules,secondRulesCount);
        free(secondString);
        free(secondRules);
    }

    if(curRules[curRulesCount-1].secondCount == 0 && curRules[curRulesCount-1].firstCount == 0) {
        char * thirdString = malloc(sizeof(char) * (strlen(curString)+20));
        memset(thirdString,0,(sizeof(char) * strlen(curString)));
        strcat(thirdString,curString);

        int thirdRulesCount = curRulesCount;
        Rule * thirdRules = malloc(sizeof(Rule) * thirdRulesCount);
        
        int i;
        for(i = 0; i < curRulesCount; i++) {
            thirdRules[i] = curRules[i];
        }
        generateAllPossibilities(thirdString,thirdRules,thirdRulesCount);
        free(thirdString);
        free(thirdRules);
    }

}


//--------------------------------------------

int main (int argc, char * argv[]) {

    allPossibilities = malloc(sizeof(char) * maxAllPossibilities * 20);
    memset(allPossibilities,0,(sizeof(char) * maxAllPossibilities * 20));
    
    if(argc != 2) {
        fprintf(stderr,"Wrong arguments given!\n");
    }

    char * fileName = argv[1];

    FILE* file = fopen(fileName,"r");

    if(file == NULL)  {
        fprintf(stderr,"Could not open file!\n");
        return EXIT_FAILURE;
    }

    int maxRules = 1000;
    rulesLength = 0;
    rules = malloc(sizeof(Rule) * maxRules);

    char * line = NULL;
    size_t len = 0;
    ssize_t nread;

    int succCount = 0;

    Rule ruleZero = (Rule) {-1,-1,-1,-1};

    while((nread = getline(&line,&len,file) > 0)) {
        if(strchr(line,':') != NULL) {
            //Rule section.
            Rule curRule = (Rule) {-1,0,0,0};
            curRule.id = strtol(line,NULL,10);
            
            line = (strchr(line,':') + 2);
            
            char * endPtr = NULL;
            
            curRule.first[curRule.firstCount] = strtol(line,&endPtr,10);
            curRule.firstCount++;

            if(strncmp(endPtr,"\"a\"",3) == 0) {
                curRule.special = 'a';
                curRule.firstCount = 0;
                curRule.secondCount = 0;
                constants[0] = curRule;
                fprintf(stderr,"Cur Rule: %d -> %c\n",curRule.id, 'a');
            } else if(strncmp(endPtr,"\"b\"",3) == 0) {
                curRule.special = 'b';
                curRule.firstCount = 0;
                curRule.secondCount = 0;
                constants[1] = curRule;
                fprintf(stderr,"Cur Rule: %d -> %c\n",curRule.id, 'b');
            } else {
                if(curRule.first[curRule.firstCount-1] >= 100) {
                    line = line + 4;
                } else if (curRule.first[curRule.firstCount-1] >= 10) {
                    line = line + 3;
                } else {
                    line = line + 2;
                }
                
                curRule.first[curRule.firstCount] = strtol(line,&endPtr,10);
                curRule.firstCount++;

                if(strchr(line,'|') == NULL) {
                    if((*(line+2)) != '\n') {
                        if(curRule.first[curRule.firstCount-1] >= 100) {
                            line = line + 4;
                        } else if (curRule.first[curRule.firstCount-1] >= 10) {
                            line = line + 3;
                        } else {
                            line = line + 2;
                        }
                        curRule.first[curRule.firstCount] = strtol(line,&endPtr,10);
                        curRule.firstCount++;
                    }
                } else {
                    line = (strchr(line,'|') + 2);
                    curRule.second[curRule.secondCount] = strtol(line,&endPtr,10);
                    curRule.secondCount++;

                    if((*(line+2)) != '\n') {
                        if(curRule.second[curRule.secondCount-1] >= 100) {
                            line = line + 4;
                        } else if (curRule.second[curRule.secondCount-1] >= 10) {
                            line = line + 3;
                        } else {
                            line = line + 2;
                        }
                        
                        curRule.second[curRule.secondCount] = strtol(line,&endPtr,10);
                        curRule.secondCount++;
                    }
                    
                }
                
                if(curRule.id == 0) {
                    ruleZero = curRule;
                }
                fprintf(stderr,"Cur Rule: %d -> [",curRule.id);
                for(int i = 0; i < curRule.firstCount; i++) {
                    fprintf(stderr,"%d,",curRule.first[i]);
                }
                fprintf(stderr,"]");
                if(curRule.secondCount > 0) {
                    fprintf(stderr," | [");
                    for(int i = 0;i < curRule.secondCount; i++) {
                        fprintf(stderr,"%d,",curRule.second[i]);
                    }
                    fprintf(stderr,"]");
                }
                fprintf(stderr,"\n");


            }
            rules[rulesLength] = curRule;
            rulesLength++;
        } //end strchr(line,':') if
        else if(strncmp(line,"\n",1) == 0) 
        {
            char * initialString = malloc(sizeof(char) * 5);
            memset(initialString,0,(sizeof(char) * 5));
            generateAllPossibilities(initialString,&ruleZero,1);

            fprintf(stderr,"\n %d \n",allPosIndex);
            for(int i = 0; i < allPosIndex; i++) {
                fprintf(stderr,"Possibility %d: %s \n",i,allPossibilities[i]);
            }
        } 
        else 
        {
           for(int i = 0; i < allPosIndex; i++) {
                if((strlen(allPossibilities[i]) == strlen(line)) && (strncmp(line,allPossibilities[i],strlen(allPossibilities[i])) == 0)) {
                    fprintf(stderr,"Line is true: %s \n", line);
                    succCount++;
                }
           }
        }
        
    }

    fprintf(stderr,"Successful count: %d \n", succCount);

    free(rules);
    fclose(file);
}
