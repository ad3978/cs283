#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
int string_reverser(char *, int, int);
int print_words(char *, int, int);

int setup_buff(char *buff, char *user_str, int len){
     //TODO: #4:  Implement the setup buff as per the directions
int str_len=0;
char *write_ptr =buff;
char *read_ptr =user_str;
int last_space =1;

while (*read_ptr != '\0')
{
if (*read_ptr == ' ' || *read_ptr == '\t')
{
if (!last_space)
{
*write_ptr++ =' ';
str_len++;
last_space=1;
}
} 
else 
{
*write_ptr++ =*read_ptr;
str_len++;
last_space=0;
}
if (str_len > len)
{
return -1; 
}
read_ptr++;
}
if (str_len > 0 && *(write_ptr-1) ==' ')
{
write_ptr--;
str_len--;
}
while (write_ptr < buff + len)
{
*write_ptr++ ='.';
}
return str_len;
}
// function to print the buffer statement
void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    printf("]\n");
}
// function to print correct usage
void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}
// function ot print word count
int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
if (!buff || len <= 0 || str_len <= 0)
{
return -1;
}
int count=0;
int in_word=0;
char *ptr=buff;

while (ptr< buff + str_len)
{
if (*ptr != ' ' && !in_word)
{
count++;
in_word=1;
}
else if (*ptr ==' ')
{
in_word=0;
}
ptr++;
}
return count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS
int string_reverser(char *buff, int len, int str_len)
{
if (!buff || len <= 0 || str_len <= 0)
{
return -1;
}
char *start =buff;
char *end = buff + str_len -1;
char temp;

while (start<end)
{
temp=*start;
*start=*end;
*end=temp;
start++;
end--;
}
return 0;
}

int print_words(char *buff, int len, int str_len)
{

if (str_len>len)
{
return -1;  
}
if (str_len == 0)
{
return 0;
}    
int word_count=0;    
int char_ctr=0;      
int at_start=1;      
char *ptr=buff;

printf("Word Print\n");
printf("----------\n");
while (ptr< buff + str_len)
{
if (at_start && *ptr !=' ')
{
word_count++;
printf("%d. ",word_count);
at_start=0; 
}
if (*ptr==' ')
{  
if (!at_start)
{
printf("(%d)\n", char_ctr);
}
char_ctr = 0;     
at_start = 1;    
} 
else
{            
putchar(*ptr);   
char_ctr++;      
}
ptr++;
}

if (!at_start)
{
printf("(%d)\n", char_ctr);
}
printf("\nNumber of words returned: %d\n",word_count);
return word_count;
}
//function to replace the string
int string_replacer(char *buff, int len, int str_len, char *search, char *replace)
{
if (!buff || !search || !replace)
{
return -1;
}
int search_len=0;
int replace_len=0;
while (search[search_len]) search_len++;
while (replace[replace_len]) replace_len++;
int found=0;
int pos=0;
for (int i=0; i<= str_len - search_len;i++)
{
found=1;
for (int j=0; j<search_len;j++)
{
if (buff[i+j] != search[j])
{
found=0;
break;
}
}
if (found)
{
pos=i;
break;
}
}

if (!found)
{
return -2;
}

char temp[BUFFER_SZ];
for (int i = 0; i < len; i++)
{
temp[i] = '.';
}
for (int i = 0; i < pos; i++)
{
temp[i] = buff[i];
}
for (int i=0;i< replace_len && (pos + i)<len; i++) 
{
temp[pos+i]=replace[i];
}

int suffix_start= pos +search_len;
int new_suffix_start =pos+ replace_len;
for (int i=0; suffix_start+i < str_len && new_suffix_start+i <len;i++)
{
temp[new_suffix_start+i] = buff[suffix_start+i];
}

for (int i=0; i< len;i++)
{
buff[i]=temp[i];
}
return len;
}
//main func
int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    // The statement if ((argc < 2) || (*argv[1] != '-')) first checks if we
    // have fewer than 2 arguments (meaning the user just ran the program without
    // any options), and thanks to the OR operator and how C evaluates conditions
    // it won't even try to look at argv[1] if there aren't enough arguments. 
    // This is super important because in C if we try to access memory that 
    // isn't there the program will crash with a segmentation fault
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    // The if (argc<3) block checks that the user gave 
    // enough command-line arguments for the program to work. The program needs 
    // both a valid option and an input string to continue. 
    // If these aren't provided then the program can't do what it's supposed to. This 
    // check helps by showing the user how to use the program properly with the 
    // usage message and also avoids crashes, like segmentation faults, 
    // that could happen if it tries to access argv[2] when it's not there. 
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a
    //          return code of 99
    buff = (char *)malloc(BUFFER_SZ);
    if (buff == NULL) {
        printf("Failed to allocate buffer\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            rc=string_reverser(buff, BUFFER_SZ, user_str_len);
            if (rc<0){
                printf("Error reversing string, rc = %d",rc);
                exit(2);
            }
            break;

        case 'w':
            rc = print_words(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error printing words, rc = %d", rc);
                exit(2);
            }
            break;

        case 'x':
            if (argc != 5) {
                usage(argv[0]);
                exit(1);
            }
            rc = string_replacer(buff, BUFFER_SZ, user_str_len, argv[3], argv[4]);
            if (rc < 0) {
                if (rc == -2) {
                    printf("wrod not found in input\n");
                } else {
                    printf("Error replacing the string, rc = %d",rc);
                }
                exit(2);
            }
            user_str_len=rc;
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
// Providing both the buffer pointer and its length to the helper 
// functions can be a good practice because it makes the code safer 
// and more flexible. Even though we know that the buffer is exactly
// 50 bytes explicitly passing the length can ensure that the functions 
// don't accidentally go out of bounds if something changes, like 
// a future update to the buffer size. It also makes the functions 
// more reusable since they aren’t tied to a fixed size and could 
// handle buffers of different lengths. This approach helps avoid bugs
// like buffer overflows and makes the code easier to maintain in the long term.
