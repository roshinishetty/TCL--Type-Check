#include<stdio.h>   //library for input output
#include<stdlib.h>  //library for memory allocation, process control, conversions etc
#include<stdbool.h> //library for boolean functions 
#include<string.h>  //library for string functions
#include<ctype.h>   //library for testing and mapping characters
#define MAX 1000     //declares MAX as 100

char *var[MAX];
char *type[MAX];
int count=0;   

char* readFile(char* filename) {
    FILE *file = NULL;
    char* input = NULL;
    int size=0;

    file = fopen(filename, "r");
    if(file==NULL) {
        perror("Error opening file\n");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    size=ftell(file);
    rewind(file);    

    input=(char*)malloc(sizeof(size));
   // while(getchar()!='\n'); //to clear the buffer NOT WORKING
    if(input==NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    fread(input, 1, size, file);
    //printf("%s\n", input);
    //fclose(file);
    return input;
} 

void createLookUp(char* filename) {
    char* input =readFile("variables.txt");
    char* line;
    while((line=strtok_r(input,"\n",&input))) {
        var[count]=strtok_r(line," ",&line);
        type[count]=line;
        count++;
    }

    for(int i=0;i<count;i++)
        printf("%s\n", type[i]);
    //getVar(input);
}

int GetVarNum(char inp[]) {
    for(int i=0;i<count;i++){
        if(strcmp(inp, var[i])==0)
            return i;
    }
    return -1;
    // locate the word in program script and find type
}

int getEquivalenceRule(char* Typ1, char* Typ2) {
    if(strcmp(Typ1, "num")==0 || strcmp(Typ1,"bool")==0 || strcmp(Typ1,"char")==0 || strcmp(Typ1,"string")==0)
        return 2;
    else if(strcmp(Typ1,"num")==0 || strcmp(Typ1,"bool")==0 || strcmp(Typ1,"char")==0 || strcmp(Typ1,"string")==0)
        return 2;
    return 3;
} // output 1-name;2-internal;3-structural

bool getResult(char* inp_1, char* inp_2, int equivalence, int num_1, int num_2) {
    char *output = readFile("result.txt");
   //char output[] = "Table to determine structural equivalence\n    a   b   c   d   e   f   g   \na   Y   Y   Y   N   N   N   N   \nb       Y   Y   N   N   N   N   \nc           Y   N   N   N   N   \nd               Y   Y   Y   N   \ne                   Y   Y   N   \nf                       Y   N   \ng                           Y   \n\nNameEquivalence:\na, b, c, d, e, f, g are name equivalent\n\nInternal Name Equivalence:\n1.  a, c are internal equivalent\n2.  b are internal equivalent\n3.  d, e, f are internal equivalent\n4.  g are internal equivalent";
   // printf("%s\n", output);

    char ch;
    char *rest=output;
    char *line;

    if(type[num_1]==type[num_2]) {
        return true;
    }

    if(num_1==-1 || num_2==-1) {    // if any of them is invalid
        printf("Invalid variables\n");
        return false;
    }

    if(num_1>num_2) { //ordering such that first is less than second
        int temp=num_2;
        num_2=num_1;
        num_1=temp;

        char* t = inp_1;
        inp_1=inp_2;
        inp_2=t;
    }

   switch(equivalence) {
        case 1:
            {
                return true;   
            }

        case 2:
           {
                int j=0;
                for(int i=0;i<2+count+5;i++) {
                    while(output[j]!='\n')
                        j++;
                    j++;
                }
               // printf("%s\n", (char*)output+j);
                char* line;
               // char* rest = (char*)output+j;
        
                while(line=strtok_r(rest, "\n",&rest)) {

                   while(line[0]!='.')
                        line++;
                    line++;
                   // printf("%s\n", line);
                    while(line[0]==' ')
                        line++;
                  //  printf("%s\n",line);
                    
                    char *words;
                    char *rem=line;
                    bool flag=0;    
                    
                    //words=strtok(rem," ");
                    while(words=strtok_r(rem," ,",&rem)) {
                    //printf("y");
                    
                   /* if(strcmp(words,"are")==0){
                        printf("%d\n", flag);
                    }*/
                    
                    if(strcmp(words,"are")==0 && flag==1)
                        return false;
                    else if(strcmp(words, "are")==0)
                        break;
                    else if(flag==1 && strcmp(words,inp_2)==0) {
                       // printf("%s\n",inp_2);
                        return true;
                    }
                    else if(flag==0 && strcmp(words,inp_1)==0){
                        flag=1;
                        //printf("%s\n",inp_1);
                    }
                    //printf("%d ",flag);
                }
                //printf("n");
                }
                
                return false;
            } 

        case 3:
            {
                char *out = output;
                for(int i=0;i<2+num_1;i++) {
                    while(out[0]!='\n')
                        out++;
                    out++;
                }
               // printf("%s\n", out);

                for(int i=0;i<num_2-num_1+1;i++) {
                    while(out[0]==' ')
                        out++;
                    out++;
                }

                while(out[0]==' ') 
                    out++;
                //printf("%s\n", out);
                if(out[0]=='Y')
                    return true;
                else if(out[0]=='N')
                    return false;
            }

        default:
            return false;
    }

 }

void displayOutput(char* inp_1, char* inp_2, bool res, int equivalence) {
    if(res) {
        printf("True as %s, %s are ", inp_1, inp_2);
    }
    else
        printf("False as %s, %s are not ", inp_1, inp_2);


    if(equivalence==1)
        printf("name ");
    else if(equivalence==2)
        printf("internally ");
    else if(equivalence==3)
        printf("structurally ");

    printf("equivalent.\n");

    //prints out if equivalent or not
}

int main() {
    char inp_1[MAX], inp_2[MAX];
    char filename[MAX];
    scanf("%s", inp_1);
    scanf("%s", inp_2);
    //scanf("%s", filename);

    createLookUp(filename);

    int num_1 = GetVarNum(inp_1);
    int num_2 = GetVarNum(inp_2);
    
    char* type_1 = type[num_1];
    char* type_2 = type[num_2];

    printf("%s\n%s\n", type_1,type_2);

    int equivalence = getEquivalenceRule(type_1, type_2);
    printf("%d\n",equivalence);
    bool result = getResult(inp_1, inp_2, equivalence, num_1, num_2 );
    displayOutput(inp_1,inp_2,result,equivalence);
    
   // for(int i=0;i<count;i++)
   //     printf("%s\t%s\n", var[i],type[i]);
    return 0;
}