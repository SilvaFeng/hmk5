// Student names: 

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo

//name : FENG KAIGANG 冯凯航
//NO : 1210022592
 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general_tools.h"
#include "task_ui.h" //better to inclnude the system head files before programmer's head file

// put the names of the students in the group in this array: 
// example   studentNames= "LI Bai cs110-d1, DU fu eie110-d2, LI qingzhao cs110-d3 "; 
// Change unknown to your name information
char studentNames[100] = "FENG KAIHANG" ;  // it is referred by task_tools.c

int main(int argc,char *argv[])
{
    strcpy(theFileName, argv[1]);
    welcome();
    char choice;
    while(TRUE)
    {
        show_menu();
        choice = get_choice();
        process_choice(choice);
        if(choice == 'q')
        {
            break;
        }
    }
    return 0;
}