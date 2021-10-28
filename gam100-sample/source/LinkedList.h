#pragma once

#define LL_SUCCESS 1
#define LL_ERROR 0

/*
* Some functions returns a linkedlist* because when linkedlist is empty, it is simple a null pointer. 
(im sorry im too lazy to make it user friendly)
Just assign the operation back to itself: objectList = LL_Add(objectList, obj);
*/

//PREFIX: LL

//Maybe can typedef a list* list to see less stars

typedef struct LinkedList LinkedList;
struct LinkedList
{
	LinkedList* prev;
	void* curr;
	LinkedList* next;
};

int LL_GetCount(LinkedList*);
int LL_IsEmpty(LinkedList*);
void* LL_Get(LinkedList* ,int);
void* LL_GetLast(LinkedList*);
LinkedList* GetHead(LinkedList* l);
/*!
@brief Adds a node to the linkedlist containing the void* as data.
	This function creates and add a node to the end of the list.
Special scenarios: The case where ll param is null, function will return the new reference
to the head of the list.
@param ll - pointer of the head of the list
@param ptr - void* of the object
@return void
*/
void LL_Add(LinkedList**, void*);
void LL_RemoveLL(LinkedList**, LinkedList*);
void LL_RemovePtr(LinkedList**, void*);
void LL_RemoveIndex(LinkedList**, int);
/*!
@brief Adds a UNIQUE node to the linkedlist containing the void* as data.
	This function check if the data exists. if it doesnt,
	it creates and add a node to the end of the list.
Special scenarios: The case where ll param is null, function will return the new reference
to the head of the list.
@param ll - pointer of the head of the list
@param ptr - void* of the object
@return void
*/
void LL_SetAdd(LinkedList**, void*);

int LL_GetIndexLL(LinkedList*, LinkedList*);
int LL_GetIndexPtr(LinkedList*, void*);
int LL_Contains(LinkedList*, LinkedList*);
int LL_ContainsPtr(LinkedList*, void*);
void LL_Clear(LinkedList**);
void* LL_Find(LinkedList*, void*(void*, void*), void*);
void** LL_ToArray(LinkedList*);
