// Student names:  

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo



#ifndef _TASK_TOOLS_H_
#define _TASK_TOOLS_H_

#include "general_tools.h"

#define VERBOSE TRUE
#define DEBUG TRUE

#define TITLE_LEN_LIMIT 200
#define TEXT_LEN_LIMIT 5000

// https://www.windmill.co.uk/ascii-control-codes.html
// By default, use ctrl+e to represent the end of input, ascii value 005
#define TEXT_END_CHAR '\005'
#define TEXT_END_EXPLAIN "holding [control] and press [e] :  ctrl+e"

//A piece of text record
typedef struct record{
    Uint id;
    String title;
    String content;
} Record;

/* Status on the relationship between the list in memory and the file.
 * UNLOADED: The file is not loaded into the list in memory for at least once. No operation has been done to change the list yet. 
 * LOADED: The file has been loaded into the list in memory. Since then, no change to the list yet. 
 * SYNCED : The list has been written to the file by some operation. Since then, the list has not been changed. 
 * UNSYNCED: Some operation has been done to change the list. 
 *           Since then, the list has not been written to the file yet. 
*/
enum status
{
    UNLOADED,
    LOADED,
    UNSYNCED,
    SYNCED
};


// choice 'a':
void read_file_to_list(void);

//choice 'b':
void write_list_to_file(void);

//choice 'c':
void record_shape1_to_list(void);
 
//choice 'd':
void record_shape2_to_list (void);
 
//choice 'e':
void record_shape3_to_list (void);
 
//choice 'f':
void record_shape4_to_list (void);
 
//choice 'g':
void input_to_record_in_list(void);
 
//choice 'h':
void find_record_by_id (void);
 
//choice 'i':
void print_all_records_in_list (void);
 
//choice 'j':
void find_records_by_title_substring (void);
 
//choice 'k':
void delete_record_by_id (void);
 
//choice 'l':
void delete_all_records_in_the_list (void);
 
//choice 'x':
void do_something_interesting (void);

#endif