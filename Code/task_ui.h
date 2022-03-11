// Student names:  

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo




// ui.h, some code for user interface
// cannot use the name _UI_H_
#ifndef _TASK_UI_H__
#define _TASK_UI_H__

void welcome(); 

void show_menu(void);

char get_choice(void);

void process_choice(char);

#define FILE_NAME_LEN 80
extern char theFileName[FILE_NAME_LEN];

//extern const char * choiceStr;

#endif