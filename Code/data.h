// Student names:  

// MUST CS110 EIE110 hmk5 2021
// Hmk designed by zyliang@must.edu.mo



#ifndef _DATA_H_
#define _DATA_H_

// typedef unsigned int  Uint; 

// Representing data of any general type
struct data {
    void * addr; //address of the data
    Uint size; // number of bytes of the data
};
typedef struct data Data; 


#endif