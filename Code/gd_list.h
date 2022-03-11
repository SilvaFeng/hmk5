// Student names: 

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo
 


// gd_list.h
/* Naming convention: 
   A name of a type or a function has the underscore style, like hello_world.
   A yype name start with a capital letter. 
   A name of a variable or array name has the camel style, like helloWorld.  
*/

#ifndef _GD_LIST_H_
#define _GD_LIST_H_

#include "general_tools.h"

/* Node of general double-linked list . 
 * The data can be any general type, recorded using the void * address. 
*/
typedef struct g_dl_node{
    Data data;
    struct g_dl_node * prev; // address of the previous node in the list
    struct g_dl_node * next; // address of the next node in the list
}G_dl_node; 

/* these function names have a g_dl prefix to avoid name clash */
G_dl_node * g_dl_find_tail(G_dl_node * someNode);
G_dl_node * g_dl_find_head(G_dl_node * someNode);

G_dl_node  * g_dl_new_node_with_data(Data d);
G_dl_node  * g_dl_insert_data_after_node(G_dl_node * someNode, Data d);
G_dl_node  * g_dl_insert_data_before_node(G_dl_node * someNode, Data d);
G_dl_node  * g_dl_append_data(G_dl_node * someNode, Data d);
G_dl_node  * g_dl_prepend_data(G_dl_node * someNode, Data d);
G_dl_node  * g_dl_find_data(G_dl_node * head, Data d , int (* data_compare)(const Data, const Data));         
G_dl_node * g_dl_delete_node(G_dl_node * someNode, void (*freeData)(Data ));
G_dl_node * g_dl_clone_list(G_dl_node * head, Data (*cloneData)(Data));
Uint g_dl_free_list(G_dl_node * head, void (*freeData)(Data));
G_dl_node  * clone_list(G_dl_node * head);
void g_dl_map_on_list(G_dl_node * head,  void(*f)(G_dl_node *));
void g_dl_print_list(G_dl_node  * head, void (*print_node)(G_dl_node *));
Uint g_dl_count(G_dl_node * head);

#endif