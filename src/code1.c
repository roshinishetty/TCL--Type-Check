/*code1 is executed as 
gcc -o code1 code1.c
./code1 > result.txt 
variables stored in variables.txt*/

#include<stdio.h>   //library for input output
#include<stdlib.h>  //library for memory allocation, process control, conversions etc
#include<stdbool.h> //library for boolean functions 
#include<string.h>  //library for string functions
#include<ctype.h>   //library for testing and mapping characters
#define MAX 1000    //declares MAX as 100

char *var[MAX];         //an array to store variables present in the script
char type[MAX][MAX];    //an array to store variables respective data types
    // can be one of "num", "char", "bool", "string", "list", "array", "dict"
int count=0;            //count gives the number of variables in the script i.e, the size of var and type arrays

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

    if(count1==len){    //
        return "num";
    }
    if(count2==len){
        return "string";
    } 

    return "string";
}

char* my_strcpy(char delim,char *destination, char *source, char* rest) {	//function to read contents between quotes or braces 
    if(delim=='"') delim='"';
    else if(delim=='{') delim='}';
    else if(delim=='[') delim=']';	// delim is made as second part of delim;

    char* start = destination;		//storing the start address of destination

    *destination = *source;
    destination++;
    source++;

    while(*source!=delim) {		// contents of source allocated to destination till second part of delim
        *destination = *source;
        destination++;
        source++; 
    }
    *destination = delim;
    destination++;
    source++;
    return start;			// starting address of destination is returned.
}

void skip(char *line){
    char* rest =line;				//function to skip
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

void getVar(char* input) {	// function to read the buffer and store variables in var array and corresponding types in types array.
    char *rest = input;
    char *line=NULL;
    char *assigned;

    while((line=strtok_r(rest,"\n;",&rest))) {	//tokenising input buffer based on \n and ;
        if(line[0]=='#')
            continue;
        char *words;
        char *rem=line;
        while((words=strtok_r(rem," ",&rem)))	// tokenising line into words
        {
            if(strcmp(words,"proc")==0) {
               // printf("It's a proc\n");
                strcpy(type[count],"proc");	// assigning type to funct var as proc
                var[count] = strtok(rem," {");	// storing name of function
                count++;
                //skip(rest);
                //printf("%s\n",rest);
                break;
            } else if(strcmp(words,"list")==0 || strcmp(words,"array")==0 || strcmp(words,"dict")==0) {
               // printf("%s\n",words);
                strcpy(type[count],words);		// storing types as list,array,dict
                char* w=strtok_r(rem," ",&rem);
                if(strcmp(w,"set")==0) 
                    var[count] = strtok(rem," ");
                count++;
                break;                              
            } else if(strcmp(words,"set")==0) {
               // printf("It's a set\n"); 
                int i;
                var[count] = strtok_r(rem," ",&rem);	
                if(strchr(var[count],'(')!=NULL) {
                    var[count]=strtok(var[count],"("); 	// assigning array type if declaration is of second type.
                    strcpy(type[count],"array");	
                    count++;
                    break;
                }
                else if(rem[0]!='"' && rem[0]!='{' && rem[0]!='[') {	
                    assigned = strtok_r(rem," }",&rem);
                    strcpy(type[count],getVarType(assigned,strlen(assigned)));	// checking type of the expression
                }
                else {
                    char delim=rem[0];
                    assigned = my_strcpy(delim,rem,rem,rest);
                    strcpy(type[count],getVarType(assigned,strlen(assigned)));	// assigning type if it is quoted word/nested expression
                }
                count++;    
                break;                  
            }
        }
    }
}            

void storeVar() {
    FILE* fptr;
    fptr = fopen("variables.txt","w");	// creating variables.txt  file 
    if(fptr==NULL) {
        perror("Error opening file");
        return;
    }
    for(int i=0;i<count;i++)
        fprintf(fptr,"%s %s\n",var[i],type[i]);	// storing variable and its types on to it
    fclose(fptr);    
}

char get_structural_type(int x) {	// function to assign structural types to data
    if(strcmp(type[x],"string")==0 || strcmp(type[x],"num")==0 ||
        strcmp(type[x],"char")==0 || strcmp(type[x],"bool")==0 || strcmp(type[x],"proc")==0)
        return 'a';
    else if(strcmp(type[x],"list")==0 || strcmp(type[x],"dict")==0)	
        return 'b';
    else if(strcmp(type[x],"array")==0)		
        return 'c';
}

void checkStructuralEquivalence() {	// to print structural equivalance table
   // a lookUp table outputted
    printf("Table to determine structural equivalence:\n");

    printf("\t");

    for(int i=0;i<count;i++) 
        printf("%s\t", var[i]);  	// print the variables name
    printf("\n");

    for(int i=0;i<count;i++) {

        printf("%s ",var[i]);

        char type_1=get_structural_type(i);	// getting the type of variable

        for (int j=0;j<=i;j++)
            printf("\t");
        for(int j=i;j<count;j++) {
            char type_2=get_structural_type(j);	// getting the type of rem variables
            if(type_1==type_2)
                printf("Y\t");
            else
                printf("N\t");
        }
        printf("\n");
    } 
    return;
}

void checkNameEquivalence() {		//function to check which types are name equivalent.

    printf("\nNameEquivalence:\n");		
    for(int i=0;i<count-1;i++)
     printf("%s, ",var[i]);
    printf("%s", var[count-1]);
    printf(" are name equivalent.\n");
    return ;     
}

void checkInternalEquivalence() {	//function to check which types are name equivalent
    
    printf("\nInternal Name Equivalence:\n");

    bool flag[count];
    memset(flag, 0, sizeof(flag));	// array of flags initialised to '0'
    int rule_num=1;

    for(int i=0;i<count;i++) {
        if(flag[i]==0) {		// check if it is not printed before i.e internal equivalent

            printf("%d.\t%s", rule_num++, var[i]);	 

            for(int j=i+1;j<count;j++) {
                if(strcmp(type[j], type[i])==0){
                    printf(", %s", var[j]);
                    flag[j]=1;		// setting flag to one so it need not be checked again
                }
            }
            printf(" are internal equivalent.\n");
        }
    }
    return;
}

void checkEquivalence() {
    checkStructuralEquivalence(); //working
    
    checkNameEquivalence(); //working
            
    checkInternalEquivalence(); //should do now
} 

int main() {
    char *input;
    int size;
    FILE *file;

    char filename[MAX];
    scanf("%s", filename);
   // printf("%s\n", filename);

    file = fopen(filename, "r");
    if(file==NULL) {
        perror("Error opening file");		// opening file
        return 0;
    }
  //  printf("yes\n");

    fseek(file, 0, SEEK_END);
    size=ftell(file);
    rewind(file);
  //  printf("%d\n",size);

    input=(char*)malloc(sizeof(size));		// allocating memory to buffer
    if(input==NULL){
        perror("Error allocating memory");
    }
  //  printf("allocated\n");

    fread(input, 1, size, file);
    input[size]='\0';
  //  printf("%s\n",input);
    fclose(file);

    getVar(input);

    storeVar();			// storing variables and their types in a file

    checkEquivalence();

    free(input);
    return 0;
}
