// Student names:  

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo



#include <stdio.h>
#include <stdlib.h>
#include "general_tools.h"
#include "gd_list.h"


/* Given someNode as the address of a node in a list,  
 * find the address of the tail (the last node) of the list.
*/
G_dl_node * g_dl_find_tail(G_dl_node * someNode)
{
  G_dl_node * current = someNode;
    while(current != NULL && current -> next != NULL)
    {
        current = current -> next;
    } 
    return current;
}

/* Given someNode as the address of a node in a list,  
 * find the address of the head (the first node) of the list.
*/
G_dl_node * g_dl_find_head(G_dl_node * someNode)
{
  G_dl_node * current = someNode;
    while(current != NULL && current -> prev != NULL)
    {
        current = current -> prev;
    } 
    return current;
}

/* Given a Data d, create a G_dl_node that contains d as its data (not a clone of d), 
 * The prev and next fields of the new node are all NULL. 
 * Return the address of the created node. 
 * When the node is not created sucessfully, exit the whole program.
*/
G_dl_node * g_dl_new_node_with_data(Data d){
    G_dl_node * newNode = (G_dl_node * ) safe_malloc( sizeof(G_dl_node)); 
    /* No need to worry if malloc() is unsuccesful, it is handled by safe_malloc  */
    newNode->data = d;
    newNode->next = NULL;
    newNode->prev = NULL; 
    return newNode; 
}

/* Given someNode as the address of a node in a list, d as a Data, 
 * create a new G_dl_node containing d as its data, and insert it in the list after the 
 * node whose address is someNode. Return the address of the new node. 
 * If someNode is NULL, it means the list is empty, then the new node becomes
 * the only node in the list. 
*/
G_dl_node * g_dl_insert_data_after_node(G_dl_node * someNode, Data d)
{
  G_dl_node * newnode = g_dl_new_node_with_data(d);
  if(newnode == NULL){return newnode;} 
  G_dl_node * nextnode = someNode -> next;
  if(nextnode == NULL)
  {
    someNode -> next = newnode;
    newnode -> prev = someNode;
    return newnode;
  }
  else
  {
    someNode -> next = newnode;
    newnode -> prev = someNode;
    newnode -> next = newnode;
    nextnode -> prev = newnode;

    return newnode;
  }

}

/* Given someNode as the address of a node in a list, d as a Data, 
 * create a new G_dl_node containing d as its data, and insert it in the list before the 
 * node whose address is someNode. Return the address of the new node. 
 * If someNode is NULL, it means the list is empty, then the new node becomes
 * the only node in the list. 
*/
G_dl_node * g_dl_insert_data_before_node(G_dl_node * someNode, Data d)
{
  G_dl_node * newnode = g_dl_new_node_with_data(d);
  if(newnode == NULL){return newnode;}
  G_dl_node * pnode = someNode -> prev;
  if(pnode == NULL)
  {
    someNode -> prev = newnode;
    newnode -> next = someNode;
    return newnode;
  }
  else
  {
    pnode -> next = newnode;
    newnode -> prev = pnode;
    newnode -> next = someNode;
    someNode -> prev = newnode;

    return newnode;
  }

}


/* Given someNode as the address of a node in a list, d as a Data, 
 * create a new G_dl_node containing d as its data, and attach it after the 
 * last node in the list. Return the address of the new node. 
 * If someNode is NULL, it means the list is empty, then the new node becomes
 * the only node in the list. 
*/
G_dl_node * g_dl_append_data(G_dl_node * someNode, Data d)
{
  G_dl_node * newnode = g_dl_new_node_with_data(d);
  G_dl_node * tail = g_dl_find_tail(someNode);
  if(newnode == NULL){return newnode;}
  else
  {
  tail -> next = newnode;
  newnode -> prev = tail;
  }
  return newnode;

}

/* Given someNode as the address of a node in a list, d as a Data, 
 * create a new G_dl_node containing d as its data, and attach it before the 
 * first node in the list. Return the address of the new node. 
 * If someNode is NULL, it means the list is empty, then the new node becomes
 * the only node in the list. 
*/
G_dl_node * g_dl_prepend_data(G_dl_node * someNode, Data d)
{
  G_dl_node * head = g_dl_find_head(someNode);
  G_dl_node * newnode = g_dl_insert_data_before_node(head,d);
  if(newnode == NULL){return NULL;}
  return newnode;

}


/* <parameters>
* - head: the address of the first node of a list
* - d: a object of Data
* - data_compare, a function (corresponding argument can be a function name), which accepts two Data objects x and y , returns an integer according some logic of the function. The return value of data_compare is: 
* .....  0, if two objects are considered equal
* ..... -1, if the x is less than y
* .....  1, if x is larger than y
*/  
//int (* data_compare)(const Data x, const Data y)
G_dl_node * g_dl_find_data(G_dl_node * head, Data d, 
							int (* data_compare)(const Data x, const Data y))
{
    G_dl_node * current = head;
    if(current == NULL){return current;}
    while(current != NULL)
    {
        if(data_compare (current -> data , d) == 0 )
        {return current;}
        else
        current = current -> next;
    }
    return NULL;
}

/* Given someNode as the address of some node in a list, delete the node from the list, and free the space of node. The other parameter, freeData, is  a function is used to free the data of the node
* The space of the data (the space its address points to), should be freed before the node is removed. 
* The left neighbor and right neighbor of the node should link with each other after removing the node. 
* Return the address of its neighbor that is closest to the tail. More exactly
* .... the address of its next neighbor if it is not NULL,  
* .... otherwise, the address of its previous neighbor if it is not NULL, 
* .... otherwise, NULL if the list is empty after deleting the node. 
*/
G_dl_node * g_dl_delete_node(G_dl_node * someNode, void (*freeData)(Data))
{
  G_dl_node * nd = someNode;
  if(someNode == NULL){return NULL;}
  if(nd != NULL)
  {
    nd = nd -> next;
    freeData(nd -> data);
    free(nd);
  }
  return someNode;

}

/* Given head as the address of the first node in a list, return the number of nodes in the list
*/
Uint g_dl_count(G_dl_node * head)
{
  G_dl_node * current = g_dl_find_head(head);
  Uint num = 1;
  while(current != NULL)
  {
    current = current -> next;
    num++;
  }
  return num;

}


/* Given head as the address of the first node in a list, and a function freeData which can free the data of a node in  list, apply freeData to each node in the list to release the space of all nodes in the list, return the number of nodes whose space are released. 
*/
Uint g_dl_free_list(G_dl_node * head, void (*freeData)(Data ))
{
  Uint count = 0;
  G_dl_node * current = g_dl_find_head(head);
  G_dl_node * nexNd = NULL;
  while(current != NULL)
  {
    nexNd = current -> next;
    freeData(current -> data);
    free(current);
    current = nexNd;
    count++;
  }
  return count;

}

/* Given head as the address of the first node in a list, create a clone of the list. Each node e of the list has a separate clone (containing som identical data object) in the clone list. Return the address of the first node in the clone list.  
*/
G_dl_node * g_dl_clone_list(G_dl_node * head, Data (*cloneData)(Data))
{
  G_dl_node * current = head;
  G_dl_node * newlisthead = NULL;
  G_dl_node * newlisttail = NULL;
  G_dl_node * newNd;
  if(head == NULL){return NULL;}
  if((newNd = g_dl_new_node_with_data(head -> data)) == NULL)
  {
    printf("Creating new node fails\n");
    return NULL;
  }
  newlisthead = newlisttail = newNd;
  current = head -> next;
  while (current != NULL)
  {
    if((newNd = g_dl_new_node_with_data(head -> data)) == NULL )
    {
      printf("creating new node fails\n");
    }

  newlisttail->next = newNd;
  cloneData(current -> data);
  current = current->next;
  newlisttail = newlisttail->next;
  
  }
  return newlisthead;
}

/* Apply the function f (a parameter) on each node of the list, whose head (the address of the first node) is given as another parameter.  
*/
void g_dl_map_on_list(G_dl_node * head,  void(*f)(G_dl_node *))
{
  G_dl_node * current = head;
  if(current == NULL)
  {
    printf("It is NULL.");
  }
  while(current != NULL)
  {
    f(current);
    current = current -> next;
  }

}

void g_dl_print_list(G_dl_node  * head, void (*print_node)(G_dl_node *))
{
  G_dl_node * current = g_dl_find_head(head);
  if(current == NULL)
  {
    printf("Nothing in the list.");
  }
  while(current != NULL)
  {
    print_node(current);
    current = current -> next;
  }
}
