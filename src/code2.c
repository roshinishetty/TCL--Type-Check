#include<stdio.h>   //library for input output
#include<stdlib.h>  //library for memory allocation, process control, conversions etc
#include<stdbool.h> //library for boolean functions 
#include<string.h>  //library for string functions
#include<ctype.h>   //library for testing and mapping characters
#define MAX 1000     //declares MAX as 100

char *var[MAX];         //an array to store variables present in the script
char type[MAX][MAX];    //an array to store variables respective data types
    // can be one of "num", "char", "bool", "string", "list", "array", "dict"
int count=0;            //count gives the number of variables in the script i.e, the size of var and type arrays

char* readFile(char* filename) {
    FILE *file = NULL;
    char *input =NULL;
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
    if(input==NULL) {
        perror("Error allocating memory");
        return NULL;
    }
    fread(input, 1, size, file);
    return input;
} 

char* getVarType(char in[], int len) {  /*Function that returns variable type from it's post expression or word*/
                //in[] is the next word after variable and len is the length
    char *x=in; //x character array stores in
    if( (strcmp(x,"yes")==0) || (strcmp(x,"on")==0) || (strcmp(x,"true")==0) ) { //if x is one of "yes","on" or "true"
        return "bool"; //returns bool
    }
    else if( (strcmp(x,"no")==0) || (strcmp(x,"off")==0) ||(strcmp(x,"false")==0) ){ //if x is one of the "no", "off" or "false"
        return "bool";  //return bool
    } 
    else if(len==1 && ( (x[0]>='a' && x[0]<='z') || (x[0]>='A' && x[0]<='Z') ) ){ //if x is a single alphabet
        return "char";  //returns char
    }   
    if(x[0]=='$') { //if x starts with $
        char* v= x+1;   //the string after $ is saved in v char array
        for(int i=0;i<count;i++)    //for loop with i increasing from 0 to count
            if(strcmp(v, var[i])==0)    //if v and var[i] are equal
                return type[i];     //return type[i]
        return "string";    //else return string
    }
    if(x[0]=='{' && x[len-1]=='}') {    //if x is enclosed in {} brackets
        for(int i=0;i<len;i++)  // for loop from 0 to len
            if(x[i]==' ')     // if x[i] is ' '
                return "list"; //return it as list
        return "string";   //else return it as string
    } 
    if(x[0]=='"' && x[len-1]=='"')  //if x is enclosed in "" brackets 
        return "string";    //return string
    if(x[0]=='[' && x[len-1]==']') {    //if x is enclosed in [] brackets
        int i=1;    //initialize i to 1
        while(x[i]==' ')    //while x[i] is ' ' 
            i++;    //increase i by 1 i.e, ignoring the spaces
        if(x[i]=='l' && x[i+1]=='i' && x[i+2]=='s' && x[i+3]== 't') // if "list" is next word 
            return "list";  //return "list"
        if(x[i]=='s' && x[i+1]=='p' && x[i+2]=='l' && x[i+3]=='i' && x[i+4]=='t') //if "split" is next word
            return "list";  //return "list"
        if(x[i]=='e' && x[i+1]=='x' && x[i+2]=='p' && x[i+3]=='r')  //if "expr" is next word
            return "num";   //return "num"
        if(x[i]=='d' && x[i+1]=='i'&& x[i+2]=='c'&& x[i+3]=='t')    //if "dict" is next word
            return "dict";  //return "dict"
    }
   
    int count1=0;//initialize count1 to 0
    int count2=0;//initialize count2 to 0
  
    for(int i=0;i<len;i++){ //iterating all the values of i from 0 to len
        if(isdigit(x[i]) || x[i]=='.'){ //if x[i] is digit or '.'
            count1++;   //count for number increases
        }
        if(isalpha(x[i])){  //if x[i] is alphabet
            count2++;   //count for alphabet increases
        }
    }

    if(count1==len){    //if count1 is equal to len
        return "num";   //return num
    }
    if(count2==len){    //if count2 is equal to len
        return "string"; //return string
    } 
    return "string";    //return string
}

char* my_strcpy(char delim,char *destination, char *source, char* rest) {
    if(delim=='"') delim='"';
    else if(delim=='{') delim='}';
    else if(delim=='[') delim=']';

    char* start = destination;

    *destination = *source;
    destination++;
    source++;

    while(*source!=delim) {
        *destination = *source;
        destination++;
        source++; 
    }
    *destination = delim;
    destination++;
    source++;
    return start;
}

void skip(char *line){
    char* rest =line;
    int count=0,count1=0;
    bool flag=0;
    while(count1<2 && rest[0]!='\0') {
        if(rest[0]=='{')
            count++;
        else if(rest[0]=='}')
        {
            count--;
            if(count==0)
                count1++;
        }   
        rest++;
    }
}    

void createLookUp(char* input) {
    char *rest = input;
    char *line=NULL;
    char *assigned;

    while((line=strtok_r(rest,"\n;",&rest))) {
        if(line[0]=='#')
            continue;
        char *words;
        char *rem=line;
        while((words=strtok_r(rem," ",&rem)))
        {
            if(strcmp(words,"proc")==0) {
                strcpy(type[count],"proc");
                var[count] = strtok(rem," {");
                count++;
                break;
            } else if(strcmp(words,"list")==0 || strcmp(words,"array")==0 || strcmp(words,"dict")==0) {
               // printf("%s\n",words);
                strcpy(type[count],words);
                char* w=strtok_r(rem," ",&rem);
                if(strcmp(w,"set")==0) 
                    var[count] = strtok(rem," ");
                count++;
                break;                              
            } else if(strcmp(words,"set")==0) {
                int i;
                var[count] = strtok_r(rem," ",&rem);
                if(strchr(var[count],'(')!=NULL) {
                    var[count]=strtok(var[count],"(");
                    strcpy(type[count],"array");
                    count++;
                    break;
                }
                else if(rem[0]!='"' && rem[0]!='{' && rem[0]!='[') {
                    assigned = strtok_r(rem," }",&rem);
                    strcpy(type[count],getVarType(assigned,strlen(assigned)));
                }
                else {
                    char delim=rem[0];
                    assigned = my_strcpy(delim,rem,rem,rest);
                    strcpy(type[count],getVarType(assigned,strlen(assigned)));
                }
                count++;    
                break;                  
            }
        }
    }
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
                char* line;
        
                while(line=strtok_r(rest, "\n",&rest)) {

                   while(line[0]!='.')
                        line++;
                    line++;
                    while(line[0]==' ')
                        line++;
                    
                    char *words;
                    char *rem=line;
                    bool flag=0;    
                    
                    while(words=strtok_r(rem," ,",&rem)) {

                        if(strcmp(words,"are")==0 && flag==1)
                            return false;
                        else if(strcmp(words, "are")==0)
                            break;
                        else if(flag==1 && strcmp(words,inp_2)==0) 
                            return true;
                        else if(flag==0 && strcmp(words,inp_1)==0)
                            flag=1;
                    }
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
                for(int i=0;i<num_2-num_1+1;i++) {
                    while(out[0]==' ')
                        out++;
                    out++;
                }

                while(out[0]==' ') 
                    out++;

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
}

int main() {
    char inp_1[MAX], inp_2[MAX];
    char filename[MAX];
    scanf("%s", inp_1);
    scanf("%s", inp_2);
    scanf("%s", filename);

    createLookUp(filename);
    
    int num_1 = GetVarNum(inp_1);
    int num_2 = GetVarNum(inp_2);
    
    char* type_1 = type[num_1];
    char* type_2 = type[num_2];

    int equivalence = getEquivalenceRule(type_1, type_2);
    bool result = getResult(inp_1, inp_2, equivalence, num_1, num_2 );
    displayOutput(inp_1,inp_2,result,equivalence);
    return 0;

}