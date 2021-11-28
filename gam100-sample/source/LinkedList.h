/*!
@file		LinkedList.h
@author		Ow Hong Yu (ow.h)
@course		CSD 1400
@section	A
@brief		A storage container which contains functions to do what a linked list or vectors can
*/

#pragma once

#define LL_SUCCESS 1
#define LL_ERROR 0


//PREFIX: LL

//Maybe can typedef a list* list to see less stars

typedef struct LinkedList LinkedList;
/// <summary>
/// Node of the linked list
/// </summary>
struct LinkedList
{
	LinkedList* prev;
	void* curr;
	LinkedList* next;
};
/// <summary>
/// Returns length of the list
/// </summary>
/// <param name="ll">- pointer of list to be counted</param>
/// <returns>int - count of the list</returns>
int LL_GetCount(LinkedList*);
/// <summary>
/// Returns true of false if list is empty
/// </summary>
/// <param name="ll">- pointer of list to be checked</param>
/// <returns>int - 1 if is empty. 0 if is not empty</returns>
int LL_IsEmpty(LinkedList*);
/// <summary>
/// Get object pointer in the list at index
/// </summary>
/// <param name="ll">- list to find object</param>
/// <param name="index">- index of the object</param>
/// <returns>void* - pointer stored in the list</returns>
void* LL_Get(LinkedList* ,int);
/// <summary>
/// Gets the last object in the list
/// </summary>
/// <param name="ll">- list to return the last object</param>
/// <returns>void* - pointer of the last node's object</returns>
void* LL_GetLast(LinkedList*);
/// <summary>
/// Returns the head node of the list
/// </summary>
/// <param name="l">- list to get the head node</param>
/// <returns>LinkedList* - pointer of the head node</returns>
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

//void LL_AddAt(LinkedList**, void*, int);

/// <summary>
/// Remove node from the list
/// </summary>
/// <param name="ll">- Reference of the list</param>
/// <param name="node">- node to be deleted</param>
void LL_RemoveLL(LinkedList**, LinkedList*);
/// <summary>
/// Removes node that contains the data. DOES NOT deletes the data
/// </summary>
/// <param name="ll">- Reference of the list</param>
/// <param name="ptr">- object/data contained in the to be deleted node</param>
void LL_RemovePtr(LinkedList**, void*);
/// <summary>
/// Removes node at index
/// </summary>
/// <param name="ll">- Reference of the list</param>
/// <param name="index">- index of node to be deleted</param>
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
/// <summary>
/// Gets index of the node in the list
/// </summary>
/// <param name="ll">- list to find the node</param>
/// <param name="ptr">- node to be found in list</param>
/// <returns>int - index of the found node. if not found, returns -1</returns>
int LL_GetIndexLL(LinkedList*, LinkedList*);
/// <summary>
/// Gets index of the object/data in the list
/// </summary>
/// <param name="ll">- list to find the object/data</param>
/// <param name="ptr">- object/data to be found in list</param>
/// <returns>int - index of the found object/data. if not found, returns -1</returns>
int LL_GetIndexPtr(LinkedList*, void*);
/// <summary>
/// Checks if list contains that node
/// </summary>
/// <param name="ll">- list to find the node</param>
/// <param name="ptr">- node to be check if contain in list</param>
/// <returns>int - true or false if node contains in list</returns>
int LL_Contains(LinkedList*, LinkedList*);
/// <summary>
/// Checks if list contains that object/data
/// </summary>
/// <param name="ll">- list to find the object/data</param>
/// <param name="ptr">- object/data to be check if contain in list</param>
/// <returns>int - true or false if object/data contains in list</returns>
int LL_ContainsPtr(LinkedList*, void*);
/// <summary>
/// Clears and delete all nodes
/// </summary>
/// <param name="ll">- Reference of the list</param>
void LL_Clear(LinkedList**);
/// <summary>
/// Finds using the function, and returns the object/data if function returns something
/// </summary>
/// <param name="ll">- list of the object to be checked</param>
/// <param name="func">- lambda func to check thru the list. 
///						first param - object/data of the current node
///						second param - arguement </param>
/// <param name="arg">- argument to be passed to lambda func</param>
/// <returns>void* - whatever the lambda func returns</returns>
void* LL_Find(LinkedList*, void*(void*, void*), void*);
//void** LL_ToArray(LinkedList*);

/// <summary>
/// Reverse the list
/// </summary>
/// <param name="ll">- Reference of the list</param>
void LL_Reverse(LinkedList**);