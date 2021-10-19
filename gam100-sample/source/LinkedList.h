#pragma once

#define LL_SUCCESS 1
#define LL_ERROR 0
//PREFIX: LL

typedef struct LinkedList LinkedList;
struct LinkedList
{
	LinkedList* prev;
	void* curr;
	LinkedList* next;
};

int LL_GetCount(LinkedList*);
int LL_IsEmpty(LinkedList*);
LinkedList* LL_Get(LinkedList* ,int);
LinkedList* LL_GetLast(LinkedList*);
/*!
@brief Adds a node to the linkedlist containing the void* as data.
	This function creates and add a node to the end of the list.
Special scenarios: The case where ll param is null, function will return the new reference
to the head of the list.
@param ll - pointer of the head of the list
@param ptr - void* of the object
@return pointer of the head of the list
*/
LinkedList* LL_Add(LinkedList*, void*);
LinkedList* LL_RemoveLL(LinkedList*, LinkedList*);
LinkedList* LL_RemovePtr(LinkedList*, void*);
LinkedList* LL_RemoveIndex(LinkedList*, int);
/*!
@brief Adds a UNIQUE node to the linkedlist containing the void* as data.
	This function check if the data exists. if it doesnt,
	it creates and add a node to the end of the list.
Special scenarios: The case where ll param is null, function will return the new reference
to the head of the list.
@param ll - pointer of the head of the list
@param ptr - void* of the object
@return pointer of the head of the list
*/
LinkedList* LL_SetAdd(LinkedList*, void*);

int LL_GetIndexLL(LinkedList*, LinkedList*);
int LL_GetIndexPtr(LinkedList*, void*);
int LL_Contains(LinkedList*, LinkedList*);
int LL_ContainsPtr(LinkedList*, void*);
LinkedList* LL_Clear(LinkedList*);
void* LL_Find(LinkedList*, void*(void*, void*), void*);
void** LL_ToArray(LinkedList*);
