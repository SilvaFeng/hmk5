// Student names:  FENG KAIHANG
 
// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo



// task_tools.c
// the code only applicable to specific business tasks are here

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general_tools.h"
#include "gd_list.h"
#include "task_ui.h"
#include "task_tools.h"

//#define FILE_CONTENT_LEN 20000
//static char fileBuffer[FILE_CONTENT_LEN] ;  // load the content of the file into this buffer.

static String theFileContent = {NULL, 0}; // The content of the file will be loaded into this String.
static int maxId = 0;                  // The largest id used for the records. Each new record will have the id of (++maxId);

extern char studentNames[100]; // refer to the array defined in driver.c

#define FILE_NAME_LEN 80
/* The array used to record the file name. 
   the null characters are helpful to make sure the array records some C-string. 
*/
char theFileName[FILE_NAME_LEN] = {'\0'};

// FILE * recordFileStream = NULL;

static G_dl_node * theRecordList = NULL; // the list

enum status theListState = UNLOADED;

/* the following function all assume that fileName  is  properly initialized. */

/* diferent parts of a record. These parts can guide the computation of inputing or outputing a record*/
enum recordParts
{
    BEGIN,
    ID,
    //INTV, // the int value after <id>
    TITLE,
    TEXT,
    END
};

static void print_record(Record r);
static void free_record_data(Data d);

/* Given a String fContent, read its content starting at the index pointed by the pointer readPos, generate a Record. 
   Return the address of the generated record. Also change the readPos to the first character that is not read. NULL is returned when no record can be found. When some error is found, exit the program
*/
static Record * fetch_record_in_string(String fContent, Uint *readPos)
{   static Uint count = 0;  //count how many record fectched. 
    Record *rp = NULL;
    const char *currentPos = fContent.addr + *readPos;
    enum recordParts step = BEGIN; //  the next step of reading which part of a record. First part is the tag <begin>
    const char *findPos = NULL;
    int id; // for the id of a record
    String titleString;
    String textString; 
    while (5 == 5)
    {   // fix a bug, checking condition to return NULL should be done after jumping to the first non-white-space 
        currentPos = str_first_non_white_space(currentPos); 
        if (*currentPos == '\0'){
            //break;
            if(DEBUG)
                printf("null character reached in %s\n", __FUNCTION__);
            return NULL;
        }
        if (currentPos == fContent.addr + (fContent.size - 1)) //the last character,  the null character.
        {
            //break;
            if(DEBUG)
                printf("ending position reached in %s\n", __FUNCTION__);
            return NULL;
        }
        if(DEBUG){
            printf("in %s, current character value is %u, currentPos index is at %ld, the ending position is at %d\n", 
            __FUNCTION__, *currentPos,   currentPos-fContent.addr, fContent.size);
            //printf("*currentPos==\'\\0\' is %d; *currentPos==0 is %d\n", *currentPos=='\0', *currentPos==0);
        }
        // skip comment
        if (*currentPos == '/' && *(currentPos + 1) == '/')
        {   
            if(VERBOSE)
                printf("record %u, found comment\n", count);
            findPos = strchr(currentPos, '\n'); // each line should end with a newline.
            if (findPos == NULL)               // a comment line should end with a newline,
                error_at_str(__FUNCTION__, "a comment has no ending newline", currentPos);
            currentPos = findPos+1;
            continue;
        }
        switch(step){
            case BEGIN:
                if (strncmp(currentPos, "<begin>", 7) != 0)
                    error_at_str(__FUNCTION__, " <begin> is missing ", currentPos);
                else{
                    if(DEBUG)
                        puts("<begin> is found");
                    currentPos=currentPos + strlen("<begin>");
                    step = ID;
                }
                break;
            case ID: 
                if(strncmp(currentPos, "<id>", 4) !=0)
                    error_at_str(__FUNCTION__, "<id> is missing ", currentPos);
                else{
                    if(DEBUG)
                        puts("<id> is found");
                    currentPos = currentPos + strlen("<id>");
                    // now fetch the integer after <id>
                    findPos = get_int_from_str(currentPos, &id);
                    if(maxId < id)
                        maxId=id; 
                    if(findPos == NULL)
                        error_at_str(__FUNCTION__, " <id> is not followed by an integer ", currentPos);
                    currentPos = findPos;
                    step = TITLE;
                }
                break;
            case TITLE:
                if(strncmp(currentPos, "<title>", 7) !=0)
                    error_at_str(__FUNCTION__, "<title> is missing ", currentPos);
                else{
                    currentPos = currentPos + strlen("<title>");
                    findPos = strstr(currentPos, "<text>"); // find the trailing '\n'. 
                    if(findPos == NULL)
                        error_at_str(__FUNCTION__, "<text> does not appear after <title>", currentPos);                    
                    // does not record the ending '\n' as part of the title. 
                    titleString = clone_char_sequence_to_string(currentPos, ( (*(findPos-1)=='\n')? findPos-2 : findPos-1 ));
                    currentPos = findPos;
                    step = TEXT;                    
                }
                break;
            case TEXT:
                if (strncmp(currentPos, "<text>", 6) !=0)
                    error_at_str(__FUNCTION__, "<text> is missing", currentPos);
                else
                {
                    currentPos = currentPos + strlen("<text>");
                    findPos = strchr(currentPos, '\n'); // find the trailing '\n'.
                    if (findPos == NULL)
                         error_at_str(__FUNCTION__, "newline does not appear after <text>", currentPos);
                    currentPos = currentPos + 1 ; // skip over the newline
                    findPos = strstr(currentPos, "<end>");
                    if(findPos==NULL)
                        error_at_str(__FUNCTION__, "<end> does not appear after <text>", currentPos);
                   
                    // does not record the ending '\n' as part of the title.
                    textString = clone_char_sequence_to_string(currentPos, findPos-1);
                    currentPos = findPos;
                    step = END;
                }
                break;
            case END:
                 if (strncmp(currentPos, "<end>", 5) !=0)
                    error_at_str(__FUNCTION__, "<end> is missing", currentPos);
                else{
                    currentPos = currentPos+5;
                    goto FINISH;
                }
                break;
        }
    }
    FINISH:
    rp = (Record * ) malloc(sizeof(Record)); 
    rp->content = textString;
    rp->id = id;
    rp->title = titleString;
    // change the reading position
    *readPos = currentPos - fContent.addr;
    if(VERBOSE){
        count++;
        printf("%s %d records fetched\n", __FUNCTION__, count);
        printf("the record is\n");
        print_record(*rp);
        puts("~~~~~");
    }
    return rp;
}

static int datacompare(const Data x, const Data y)
{
    Record * addrx = (Record*)x.addr;
    Record * addry = (Record*)y.addr;
    if(addrx -> id != addry -> id)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* choice 'a':
Read the file content into the list. If there is anything wrong, print some error message and quit the program. 
*/
void read_file_to_list(void)
{
    Record *rp = NULL;
    G_dl_node *tail;  // tail of the list in memory
    Uint readPos = 0; // the current index in fileConent to fetch a record
    if (theListState == SYNCED || theListState == LOADED)
        return; // nothing need to be done
    // release the space of the String fileContent before loading the file
    if (theFileContent.addr != NULL)
        theFileContent = free_string(theFileContent);
    // release the space of the list before loading the file
    delete_all_records_in_the_list();
    theFileContent = load_file_to_string(theFileName);
    if(VERBOSE){
        puts("The load file content is:");
        puts(theFileContent.addr);
    }
    while ((rp =  fetch_record_in_string(theFileContent, &readPos)) != NULL)
    {
        G_dl_node * ndp = g_dl_append_data(theRecordList, make_data(rp, sizeof rp));
        if(theRecordList == NULL)
            theRecordList = ndp; //When the list turn from empty to non-empty, record the address of its first node. 
    }
    theListState = LOADED; //just loaded the file into the list, before the list makes any change.
}

//choice 'b':
void write_list_to_file(void)
{
    G_dl_node * head = g_dl_find_head(theRecordList);
    G_dl_node * current;
    current = head;
    FILE * fp;
    fp = fopen(theFileName,"w");
    if(fp == NULL)
    {
        printf("It is fail\n");
        exit(1);
    }

    printf("what is the id?\n");
    scanf("%s",current);
    current = current -> next;

    printf("what is the title?\n");
    scanf("%s",current);
    current = current -> next;

    printf("what is the content?\n");
    scanf("%s",current);

    while (head != NULL)
    {
        fgets(head,TEXT_LEN_LIMIT,fp);
        head = head -> next;
    }

    fclose(fp);

}

//choice 'c':
void record_shape1_to_list(void)
{
    char * node;
    printf("welcome to ues the fuction 1 to print your shape");
    printf("please input the colummn and line you want, it is a square\n");
    int line,col;
    scanf("%d %d",&col,&line);
    for(;line>0;)
    {
        while(col>0)
        {
            sprintf(node,"*");
            col--;
        }
        line--;
    }

}

//choice 'd':
void record_shape2_to_list(void)
{
    char * node;
    printf("welcome to ues the fuction 2 to print your shape");
    printf("please input the colummn and line you want, it is a special parallelogram\n");
    int line,col;
    scanf("%d %d",&col,&line);
    for(;line>0;)
    {
        while(col>0)
        {
            sprintf(node," ");
            sprintf(node,"*");
            col--;
        }
        line--;
    }

}

//choice 'e':
void record_shape3_to_list(void)
{
    char * node;
    printf("welcome to ues the fuction 3 to print your shape");
    printf("please input the line you want, it is a special triangle\n");
    int line;
    scanf("%d",&line);
    for(int i = 0;i<line;i++)
    {
        int j;
        for(j = 0;j<i;j++)
        {
            sprintf(node," ");
        }
        for(j = 0;j<2*line-2*i-1;j++)
        {
            sprintf(node,"*");
        }
        sprintf(node,"\n");
    }
}

//choice 'f':
void record_shape4_to_list(void)
{
    char * node;
    printf("welcome to ues the fuction 4 to print your shape");
    printf("please input the line you want, it is a normal triangle\n");
    int line;
    scanf("%d",&line);
    for(int i = 0;i<line;i++)
    {
        int j;
        for(j = 0;j<line-i-1;j++)
        {
            sprintf(node," ");
        }
        for(j = line-i-1;j<line+1;j++)
        {
            sprintf(node,"*");
        }
        sprintf(node,"\n");
    }
}

//choice 'g':
static Record* input_to_record(void){
    Record * rp = (Record *) malloc(sizeof(Record)); // allocate a record on Heap. 
    String titleString, textString;
    printf(":) What is the title of the text record? No more than %d characters \n", TITLE_LEN_LIMIT);
    titleString.addr = f_input_to_fit_str(stdin, '\n', TITLE_LEN_LIMIT);
    titleString.size = strlen(titleString.addr)+1; 
    printf(":) What is content of the text record? No more than %d characters \n", TEXT_LEN_LIMIT);
    printf(":) End the input by %s \n", TEXT_END_EXPLAIN);
    textString.addr = f_input_to_fit_str(stdin, TEXT_END_CHAR, TEXT_LEN_LIMIT);
    textString.size = strlen(titleString.addr)+1; 
    rp->id = ++maxId;
    rp->title = titleString;
    rp->content = textString;
    return rp;
}

void input_to_record_in_list(void){
    Record* rp = input_to_record();
    Data d = {rp, sizeof(Record)}; 
    G_dl_node *nd = g_dl_append_data(theRecordList, d);
    if(theRecordList == NULL)
        theRecordList = nd;  // in case a record is added to an empty list, record the head address
    if(VERBOSE){
        puts(":) A record is added into the list. The record is:");
        print_record (*rp);
    }
}

//choice 'h':
void find_record_by_id(void)
{
    G_dl_node * current = g_dl_find_head(theRecordList);
    G_dl_node * node = NULL;
    int ID = 0;
    printf("What is the id you wanted ?\n");
    scanf("%d",&ID);
    clear_input_queue();
    Data want;
    want.addr = safe_malloc(sizeof(Record));
    ((Record*)want.addr) -> id = ID;
    node = g_dl_find_data(current,want,datacompare);
    if(node == NULL)
    {
        printf("Not found the id\n");
    }
    else
    {
        print_record(*((Record *) (node -> data.addr)));
    }
}

static void print_record(Record r){
    // if(r==NULL)
    //    return ;
    printf("................  <id> %u ...........\n", r.id);
    printf("<title> %s\n", r.title.addr);
    printf("<text>\n");
    puts(r.content.addr);
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"); 
}

static void print_record_node(G_dl_node * nd)
{
    if (nd==NULL)
        return;
    Record * rp = (Record *) nd->data.addr;
    print_record(*rp);
}

//choice 'i':
void print_all_records_in_list(void){
    Uint count = g_dl_count(g_dl_find_head(theRecordList));
    printf("The list has %u nodes\n", g_dl_count(theRecordList));
    g_dl_print_list(theRecordList, print_record_node);
}

//choice 'j':
void find_records_by_title_substring(void)
{
    G_dl_node * current = g_dl_find_head(theRecordList);
    G_dl_node * node = NULL;
    char into[7000] = {'\0'};
    printf("Please input the substring of the record :\n");
    fgets(into,7000,stdin);
    char *str = into;
    while((*str) != '\n')
    {
        str++;
    }
    Data sub;
    sub.addr = safe_malloc(sizeof(Record));
    ((Record*)(sub.addr)) -> title.addr = into;
    node = g_dl_find_data(current,sub,datacompare);
    if(node == NULL)
    {
        printf("Not found in the record\n");
    }
    else
    {
        print_record( * ((Record *) node -> data.addr));
    }

     
}

//choice 'k':
void delete_record_by_id(void)
{
    G_dl_node * current = g_dl_find_head(theRecordList);
    G_dl_node * node = NULL;
    int ID = 0;
    printf("What is the id of the record you want to delete?\n");
    scanf("%d",&ID);
    clear_input_queue();
    Data want;
    want.addr = safe_malloc(sizeof(Record));
    ((Record*)want.addr) -> id = ID;
    node = g_dl_find_data(current,want,datacompare);
    if(node == NULL)
    {
        printf("Not found the record !\n");
    }
    else
    {
        g_dl_delete_node(node,free_record_data);
        printf("It is done!\n");
    }

}

static void free_record_data(Data d)
{
    Record *r = (Record *)d.addr;
    free(r->content.addr); // release the space pointed by the String
    free(r->title.addr);   // release the space pointed by the String
    free(r);               // free the record after its contents are freed
}

//choice 'l':
void delete_all_records_in_the_list(void)
{
    g_dl_free_list(theRecordList, free_record_data);
    theRecordList = NULL; // the list is empty.
}

//choice 'x':
void do_something_interesting(void)
{
    puts(":) I am thinking about it.  ");
}
