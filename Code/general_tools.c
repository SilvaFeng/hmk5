// Student names:  FENG KAIHANG

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "general_tools.h"  

/* Like malloc, allocate space of size bytes on the heap. The difference is that, if anything wrong (parameter size is 0 or malloc fails), print some error message and exit the whole program. 
*/
void *safe_malloc(Uint size)
{
  if(size == 0 )
  {
    printf("size is wrong\n");
    exit(EXIT_FAILURE);
  }
  return malloc(size);
}

/* Clear (remove) the characters in the input queue of stdin. Return the number of characters removed from the input queue.  
*/
Uint clear_input_queue(void)
{
    int ch;
    int num = 0;
    while(( ch = getchar()) != '\n' && ch != EOF)
    {
        num++;
    }
    return num;
}

/* Create a Data object that contains an address addr and an unsigned integer size.
*/
Data make_data(void *addr, Dsize size)
{
    Data d;
    d.addr = addr;
    d.size = size;
    return d;
}


/* Given a sequence of charcters (may not contain a 0 character, thus not a string) whose starting and ending character addresses are the parameters start and end, respectively.  Create clone of the char sequence on the heap. The clone is longer than the character sequence by one character, which is added trailing null character to make it a string. Return a String object which contains the starting address of the clone and its size.                     

*/
String clone_char_sequence_to_string(const char *start, const char *end)
{
    Dsize size = end - start + 2; // reserve the space of the trailing '\0'
    char *b = (char *)safe_malloc(size);
    const char *a = start;
    String str;
    int j;
    str.addr = b;
    str.size = size;
    for (j = 0; j < size - 1; j++)
        b[j] = a[j];
    b[j] = '\0'; // make the clone a string
    return str;
}

/* Return a Data object, which is the same as the the String object str, which is the parameter. Only difference is that the type of addr in the returned object changes to void *.
*/
Data string_to_data(String str)
{
    Data d;
    d.addr = (void *)(str.addr);
    d.size = str.size;
    return d;
}

/* Return a String object, which is the same as the the Data object d, which is the parameter. Only difference is that the type of addr in the returned object changes to char *.
*/
String data_to_string(Data d)
{
    String s;
    s.addr = (char *)d.addr;
    s.size = d.size;
    return s;
}

/* Given a String object, release its content, set its size to be 0, return the changed String parameter. 
   To reset a String variable x to represent an empty string, use:
   x = free_string(x); 
   Design idea: the parameter is not String *, no need to use a pointer to free the object it points to. Sometimes a String structure does not appear on the heap and cannot be freed using its address. 
*/
String free_string(String  str){
    free(str.addr);
    str.addr=NULL;
    str.size=0;
    return str;
}

/* Given f as the name of a file, opened a file stream  of it (using the "r" mode). Allocate a character array on the Heap whose size is n+1, for  n as the number of bytes in the file.  Read all the characters of the file into the character array. A trailing null character is put in the array as after all the loaded characters in, to make the content of the array a C-string. Return a String whose addr field is the the address of the first character in the array, and the the size field is n+1.  
If a '\0' character is found in the file before EOF is reached ( '\0' in file will cause trouble), some error message is printed out and the program exits. 
The opened file stream should be closed before 
returning the created String object. 
*/
String load_file_to_string(const char *fileName)
{
    FILE *f = fopen(fileName, "r");
    if (f == NULL)
    {   // __FUNCTION__  is the name of the calling function.
        printf("When calling %s, cannot open the file!!! \n", __FUNCTION__); 
        exit(EXIT_FAILURE);
    }
    /*  since ftell returns long, maybe Dsize is too small.
    +2 to make the allocated space large enough to save the string. 
    */
    { // a new block to declare some new names here
        fseek(f, 0, SEEK_END); // goto the end of file
        Dsize size = ftell(f) + 2;
        char *addr = (char *)safe_malloc(size);
        int j = 0;
        int c;
        char ch;
        rewind(f); // don't forget it, read from the start of the file
        while ((c = getc(f)) != EOF)
        {
            ch = (char)c;
            if (ch == '\0')
            {
                puts("The file contains a null character, wrong !");
                exit(3);
            }
            addr[j++] = ch;
        }
        addr[j] = '\0'; // make it a real c-string
        fclose(f); // remember to close it. 
        return data_to_string(make_data(addr, size));
    }
}

/* Given a file name, open a stream of the file, write the string of the String str into the file stream f. The previous content of f is totally cleared.  
*/
int save_string_to_file(const char * fileName, String str)
{    
  FILE *f = fopen(fileName, "a+");
  char *sdr = str.addr;
  fseek(f, 0, SEEK_END);
  fputc('\n', f);
  if (f == NULL)
  {  
        printf("When calling %s, cannot open the file!!! \n", __FUNCTION__); 
        exit(EXIT_FAILURE);
  }

  int n = 1;
  while(n < str.size)
  {
      if(fputc(*sdr,f) == EOF)
      {
        printf("could not input the file into the string.");
        exit(EXIT_FAILURE);
      }
      else
      {
        sdr = sdr + 1;
      }
      n++;
  }

  fclose(f);
  return 1;
}

/* Given an address addr in some string, find the address of the first non-white-space character on or after str
   A design choice: the return type is char*, better than const char*. refer to strstr()
*/
char * str_first_non_white_space(const char * addr){
    const char * p = addr;
    while(isspace(*p) && (*p != '\0')) // the ending '\0' is should not a be a space, check it incase isspace allow it. 
        p++;
    return (char *) p;
}

/* Given a string str (an address), and another string substr, find the address of the first character after the first occurrence of substr in str. If substr does not occur on or after str, return NULL. Otherwise, return the address of the first character in str after the found occurrence of substr. 

For example   str_skip_substr("apple", "app") will return the address of 'l' 
*/
char * str_skip_substr(const char * str, const char *substr)
{
    if(str == NULL || substr == NULL){return NULL;}
    return strstr(str,substr);

   
} 

/*Given choiceStr as a string, ask the user to provide character that should appear in choiceStr. Get the first character of user's input line, if the character does not appear in choiceStr, ask the user to input again. The input queue is always cleared after each input. 
For example, get_char_choice("apple") will return a user-provided character among a p l and e, 
*/
char get_char_choice(const char choiceStr[])
{
   printf(":) Please make a choice (type a character followed by Enter) according to the menu.\n");
   printf("%s",choiceStr);
   int i;
   char ch;
   while(5 == 5)
   {
       i = 0;
       ch = getchar();
       while(choiceStr[i] != '\0')
       {
           if(choiceStr[i] == ch){break;}
           i++;
       }
       printf(":)your input is not include , please input it again\n");
       clear_input_queue();
   }
   
   return  ch;

}

/* Given an address of a character, addr,  in some character array, and an address num of some int address num, read several consequtive characters starting from addr, skipping any proceeding white space,  that can form an int ( an optional sign of + or - is allowed). The integer is saved at the address num. The return value is the address of the first character after the intger in the array. If no integer appearing after addr, NULL is returned.  

For example:   
int x; 
char * ca = get_int_from_str("-5abc", &x);
// ca is the address of a, and x become -5. 
char * cb = get_int_from_str("abc", &x);
// cb is NULL, and x does not change
const char * str = "good  996haha";
char * cb = get_int_from_str(&str[4], &x);
// cb is the address of h, and x becomes 996
*/
const char * get_int_from_str(const char * addr, int * num)
{
 int num1 = 1;
 while((* addr) > 10)
 {
    if(addr == NULL){return NULL;}
    addr++;
 }
 while(* addr < 10)
 {
    if(addr == NULL){return NULL;}
    * num = ((* num)*10) + (* addr); 
    addr++;
    if(* addr >> 10 && * addr == ' '){break;}
 }

 return addr;
 
}

/* Wait until the user hit enter. Print some hint message before user's input. The input queue is cleared.  */
void pause_for_enter(void){
    puts(":) To continue, hit the ENTER key");
    clear_input_queue();
}

/* print at most n (a parameter) characters in a string at address addr. If n characters are printed or the '\0' is reached, stop the printing. return the number of characters returned 

For example: 
print_n_char("hello world", 5) will print: hello
print_n_char("hello world", 8) will print:  hello wo
*/
Uint print_n_char(const char * addr, Uint n)
{
 while(n>0)
 {
    if(* addr == '\0'){break;}
    printf("%c",* addr);
    addr++;
    n--;
 }
 return 1;

}

/* Parameters: 
   - functionName: the name of a funtion   
   - errorMsg: an error message errorMsg
   - position: and an address position
   computation:
   - print the function name and the error message. print (at most) 10 characters starting at the position. Exit the program. 
   How to call the function, example: 
   -  error_at_str(__FUNCTION_, "It is wrong", someCharAddr);
*/
void error_at_str(const char* functionName, const char* errorMsg, const char* currentPos) {
    printf("Calling %s found that %s errorMsg!!!\n", functionName, errorMsg);
    printf("Detection at the place: ");
    print_n_char(currentPos, 10); // show the errors
    puts("");
    exit(EXIT_FAILURE);
}

/* Given addr as the address of the first character of a string, create a clone of the string on Heap, whose size is very fit, i.e. it is an array of with its size equal to the length of the string plus 1 (1 for the trailing null character). Return the address of the first character of the clone. 

For example: 
char arr[100] = "hello"; // too much wasted space. 
char * p = str_fit_clone(arr); 
// Some space of 6 characters recording the string "hello" is allocated on the Heap, and the address of the first character is saved in p

*/
char * str_fit_clone(const char * addr)
{
 if(addr == NULL){return NULL;}
 char * addrhead;
 char * addrclone;
 addrhead = addrclone;
 while(* addr != '\0')
 {
    * addrclone = * addr;
    addrclone++;
    addr++;
 } 

 return addrhead;
}

/*  Given an opened file stream ("inputFile, a parameter) for inputting, read some characters from the stream and record these characters in some space with exactly fitting (no waste) space. 
The recording ends when 
- the first endingChar (a parameter) is read. Note the endingChar is not recorded.
- or EOF is received
- or sizeLimit (a parameter) characters are recorded. 
 A trailing null character is appended to the recorded characters to make it a c-string. If inputFile is the standard input stream (stdio), then the input queue is cleared if it is not empty (when the last read character is not '\n') after the recording. 

Design idea: instead of considering only stdin as the input source, use a file stream can make the function more useful; it can possibly read characters from a file on hard drive. 

Return the address of the first character of the recorded c-string (on Heap). 

Hint:  str_fit_clone() is helpful
*/
char * f_input_to_fit_str(FILE* inputFile, char endingChar, Uint sizeLimit)
{
    int c; 
    char string[100];
    if(inputFile == NULL){exit(EXIT_FAILURE);}
    fgets(string,100,inputFile);
    char * str_file_1 = str_fit_clone(string);
    char * str_file_2;
    char * str_file_2head;
    str_file_2head = str_file_2;
    while(sizeLimit>0)
    {
        * str_file_2 = * str_file_1;
        str_file_1++;
        str_file_2++;
        sizeLimit--;
    }
    char * str_file_3;
    char * str_file_3head;
    str_file_3head = str_file_3;
    while(* str_file_2head != endingChar)
    {
        * str_file_3 = * str_file_2head; 
        str_file_3++;
        str_file_2head++;
    }

    return str_file_3head;

}


