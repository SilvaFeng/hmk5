// Student names:  

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo


#include  <stdio.h>
#include "task_ui.h"
#include "task_tools.h"

extern char studentNames[100]; 

void welcome(void){
    puts(":) ~~~~~ Welcom to the program of <Text-Record Manager> ~~~~~ ");
    puts(":) Each record contains:"
         ":) its ID (an integer);\n" 
        ":) its title (a line of string);\n"
        ":)  and its text (a string of possibly multiple lines. ");
    puts(":) Among the operations, records can be saved to, or loaded from a file. ");
    puts(":) Hope you find this program useful. ");
    puts(":) ~~~~~ Enjoy ~~~~~");
}


void show_menu(void){
    puts("=================== Menu ==================");
    puts("a : Read a file into a list of string records in memory");
    puts("    (warning) the content of the list in memory will be lost");
    puts("b : Write the list of record in memory into the file");
    puts("   (warning) the content of the file will be replaced");
    puts("c : Record a string (as a node in the list) of some shape s_1 by calling some function f_1  ");
    puts("d : Record a string (as a node in the list) of some shape s_2 by calling some function f_2  ");
    puts("e : Record a string (as a node in the list) of some shape s_3 by calling some function f_3  ");
    puts("f : Record a string (as a node in the list) of some shape s_4 by calling some function f_4  ");   
    puts("g : Record a string (as a node in the list) of provided by user's input at the command line.  ");
    puts("    The input can have multiple lines, ending by a line of control+e followed by Enter");
    puts("h : Find a record in the list with some given id, and print the record ");
    puts("i : Print all records of the list ") ;
    puts("j : Given a string provided by the user,\n"
         "    find and print all the records in the list whose titles contain the string as a substring");
    puts("k : Delete a record whose id is provided by the user");
    puts("l : Delete all of the records in the list");
    puts("x : Something interesting");
    puts("y : Print the names of students who submitted this homework"); 
    puts("q : Quit the program");
    puts("===========================================");
    //puts(":) Please make a choice (type a character followed by Enter) according to the menu.");
}

static const char * choiceStr = "abcdefghijklxyq";

char get_choice(void){
    return get_char_choice(choiceStr);
}

void process_choice(char x){
    switch(x){
        case 'a': read_file_to_list(); break;
        case 'b': write_list_to_file(); break;
        case 'c': record_shape1_to_list(); break;
        case 'd': record_shape2_to_list(); break;
        case 'e': record_shape3_to_list(); break;
        case 'f': record_shape4_to_list(); break;
        case 'g': input_to_record_in_list();  break;
        case 'h': find_record_by_id(); break;
        case 'i': print_all_records_in_list(); break;
        case 'j': find_records_by_title_substring(); break;
        case 'k': delete_record_by_id(); break;
        case 'l': delete_all_records_in_the_list(); break;
        case 'x': do_something_interesting(); break;
        case 'y': puts("The names of the students:");
                  puts(studentNames);
                  break;
        case 'q': printf(":) Bye bye\n"); break;
        default : puts("Wrong choice, bug!"); break;
    }
}