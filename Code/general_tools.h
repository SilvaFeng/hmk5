// Student names:  FENG KAIHANG

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo



#ifndef _GENERAL_TOOLS_H_
#define _GENERAL_TOOLS_H_

typedef unsigned int Uint;

/* Type to describe data size. In case unigned int is too small to describe datasize, just change it, like
typedef unsigned long long Dsize; 
*/
typedef unsigned int Dsize ; 

// Representing data of any general type
struct data {
    void * addr; //address of the data, space should be allocated on Heap.
    Dsize size; // number of bytes of the data
};
typedef struct data Data; 

/* A String object is also a Data object, only different is the addr field has type char *. Note that the size field also include the space of the terminating null character
*/
typedef struct str{
    char * addr;
    Dsize size; 
} String; 


typedef enum bool{FALSE, TRUE} Bool; 

void * safe_malloc(Uint size);

Uint clear_input_queue(void);

Data make_data(void * addr, Dsize size);

// naming convention: string appear in the funciton names if a String object is involved. Otherwise, if dealing with c-string, use str. 
String clone_char_sequence_to_string(const char * start, const char * end);

Data string_to_data(String str);

String data_to_string(Data d);

String free_string(String  string);

String load_file_to_string(const char * fileName);

int save_string_to_file(const char * fileName, String str);

char * str_first_non_white_space(const char * addr); 

char * str_skip_substr(const char * str, const char *substr); 

// jump to the starting position of a substring can be implemented using library function strstr.

char get_char_choice(const char choiceStr[]);

const char * get_int_from_str(const char * addr, int * num);

void pause_for_enter(void);

Uint print_n_char(const char * addr, Uint n);

void error_at_str(const char* functionName, const char* errorMsg, const char* position);

char * str_fit_clone(const char * addr);

char * f_input_to_fit_str(FILE* inputFile, char  endingChar, Uint sizeLimit);

#endif

