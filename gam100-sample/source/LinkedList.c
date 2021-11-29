/*!
@file		LinkedList.c
@author		Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief		A storage container which contains functions to do what a linked list or vectors can
*/

#include "LinkedList.h"
#include <stdio.h> //for NULL
#include <stdlib.h>//for malloc

/// <summary>
/// Returns the head node of the list
/// </summary>
/// <param name="l">- list to get the head node</param>
/// <returns>LinkedList* - pointer of the head node</returns>
LinkedList* GetHead(LinkedList* l)
{
	if (LL_IsEmpty(l))
		return NULL;
	while (l->prev)
	{
		l = l->prev;
	}
	return l;
}
/// <summary>
/// Deletes the param-ed node and reattach the list to cover the empty node
/// </summary>
/// <param name="currPtr">- node to be deleted</param>
/// <returns>LinkedList* - head of the list</returns>
LinkedList* RemoveThis(LinkedList* currPtr)
{
	if (currPtr->prev == NULL)
	{
		LinkedList* old = currPtr;
		currPtr = currPtr->next;
		if (currPtr) 
			currPtr->prev = NULL; //release head
		old->curr = NULL;
		old->next = NULL;
		free(old);
		return GetHead(currPtr);
	}
	
	LinkedList* prev = currPtr->prev;
	LinkedList* next = currPtr->next;
	prev->next = currPtr->next;
	if (next)
		next->prev = prev;
	currPtr->prev = NULL;
	currPtr->next = NULL;
	currPtr->curr = NULL;
	free(currPtr);
	return GetHead(prev);
}

/*!
@brief Returns last linkedlist node
@param ll - pointer of the head of the list
@return LinkedList* node of the last
*/
LinkedList* GetLastNode(LinkedList* ll)
{
	if (LL_IsEmpty(ll))
		return NULL;
	LinkedList* ret = ll;

	while (ret->next != NULL)
		ret = ret->next;
	return ret;
}
/// <summary>
/// Returns length of the list
/// </summary>
/// <param name="ll">- pointer of list to be counted</param>
/// <returns>int - count of the list</returns>
int LL_GetCount(LinkedList* ll)
{
	if (LL_IsEmpty(ll))
		return 0;
	int count = 1;
	LinkedList* node = ll;
	while (node->next != NULL)
	{
		node = node->next;
		count++;
	}
	return count;
}
/// <summary>
/// Returns true of false if list is empty
/// </summary>
/// <param name="ll">- pointer of list to be checked</param>
/// <returns>int - 1 if is empty. 0 if is not empty</returns>
int LL_IsEmpty(LinkedList* ll)
{
	return ll == NULL;
}
/// <summary>
/// Get object pointer in the list at index
/// </summary>
/// <param name="ll">- list to find object</param>
/// <param name="index">- index of the object</param>
/// <returns>void* - pointer stored in the list</returns>
void* LL_Get(LinkedList* ll, int index)
{
	if (LL_IsEmpty(ll))
		return NULL;
	int count = 0;
	LinkedList* node = ll;
	while (node != NULL)
	{
		if (count == index)
			return node->curr;
		node = node->next;
		count++;
	}
	return NULL;
}
/// <summary>
/// Gets the last object in the list
/// </summary>
/// <param name="ll">- list to return the last object</param>
/// <returns>void* - pointer of the last node's object</returns>
void* LL_GetLast(LinkedList* ll)
{
	return GetLastNode(ll)->curr;
}
/*!
@brief Adds a node to the linkedlist containing the void* as data.
	This function creates and add a node to the end of the list.
Special scenarios: The case where ll param is null, function will return the new reference
to the head of the list.
@param ll - pointer of the head of the list
@param ptr - void* of the object
@return void
*/
void LL_Add(LinkedList** ll, void* ptr)
{
	LinkedList* lptr = GetLastNode(*ll);
	LinkedList* newPtr = malloc(sizeof(LinkedList));
	if (lptr == NULL)
	{
		*ll = newPtr;
		if (*ll) //to prevent warnings
		{
			(*ll)->prev = NULL;
			(*ll)->curr = ptr;
			(*ll)->next = NULL;
		}
		return;
		//return ll;
	}
	if (newPtr)
	{
		newPtr->prev = lptr;
		newPtr->curr = ptr;
		newPtr->next = NULL;
	}

	lptr->next = newPtr;
	//return ll;
}

//void LL_AddAt(LinkedList** ll, void* ptr, int index)
//{
//	if (LL_IsEmpty(*ll))
//		return;
//	LinkedList* node = *ll;
//	int counter = 0;
//	for (; counter < index && node; counter++, node = node->next)
//	{
//		
//	}
//	LinkedList* newPtr = malloc(sizeof(LinkedList));
//	LinkedList* prev = 
//}

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
void LL_SetAdd(LinkedList** ll, void* ptr)//Add unique
{
	if (!LL_ContainsPtr(*ll, ptr))
		LL_Add(ll, ptr);
	//return ll;
}
/// <summary>
/// Remove node from the list
/// </summary>
/// <param name="ll">- Reference of the list</param>
/// <param name="node">- node to be deleted</param>
void LL_RemoveLL(LinkedList** ll, LinkedList* node)
{
	if (LL_Contains(*ll, node))
	{
		//LinkedList* ret = RemoveThis(node);
		//if (LL_IsEmpty(ret))
		//	return NULL;
		*ll = RemoveThis(node);
		*ll = GetHead(*ll);
	}

	//return ll;
}
/// <summary>
/// Removes node that contains the data. DOES NOT deletes the data
/// </summary>
/// <param name="ll">- Reference of the list</param>
/// <param name="ptr">- object/data contained in the to be deleted node</param>
void LL_RemovePtr(LinkedList** ll, void* ptr)
{
	//if (LL_IsEmpty(*ll))
	//	return;

	LinkedList* node = *ll;
	while (node)
	{
		if (node->curr == ptr)
		{
			//LinkedList* ret = RemoveThis(node);
			//if (LL_IsEmpty(ret))
			//	return NULL;
			//return ll;
			*ll = RemoveThis(node);
			*ll = GetHead(*ll);
			return;
		}
		node = node->next;
	}
	//return head
	//return ll;
}
/// <summary>
/// Removes node at index
/// </summary>
/// <param name="ll">- Reference of the list</param>
/// <param name="index">- index of node to be deleted</param>
void LL_RemoveIndex(LinkedList** ll, int index)
{
	int count = 0;
	LinkedList* node = *ll;
	for (; node; node = node->next)
	{
		if (count == index)
		{
			*ll = RemoveThis(node);
			*ll = GetHead(*ll);
			return;
		}
		count++;
	}
	if (index >= count)
		return;
}
/// <summary>
/// Gets index of the node in the list
/// </summary>
/// <param name="ll">- list to find the node</param>
/// <param name="ptr">- node to be found in list</param>
/// <returns>int - index of the found node. if not found, returns -1</returns>
int LL_GetIndexLL(LinkedList* ll, LinkedList* ptr)
{
	if (LL_IsEmpty(ll))
		return -1;
	LinkedList* node = ll;
	int index = 0;
	while (node != NULL)
	{
		if (node == ptr)
			return index;
		node = node->next;
		index++;
	}

	return -1;
}
/// <summary>
/// Gets index of the object/data in the list
/// </summary>
/// <param name="ll">- list to find the object/data</param>
/// <param name="ptr">- object/data to be found in list</param>
/// <returns>int - index of the found object/data. if not found, returns -1</returns>
int LL_GetIndexPtr(LinkedList* ll, void* ptr)
{
	if (LL_IsEmpty(ll))
		return -1;
	LinkedList* node = ll;
	int index = 0;
	while (node != NULL)
	{
		if (node->curr == ptr)
			return index;
		node = node->next;
		index++;
	}

	return -1;
}
/// <summary>
/// Checks if list contains that node
/// </summary>
/// <param name="ll">- list to find the node</param>
/// <param name="ptr">- node to be check if contain in list</param>
/// <returns>int - true or false if node contains in list</returns>
int LL_Contains(LinkedList* ll, LinkedList* ptr)
{
	return (LL_GetIndexPtr(ll, ptr->curr) < 0 ? 0 : 1);
}
/// <summary>
/// Checks if list contains that object/data
/// </summary>
/// <param name="ll">- list to find the object/data</param>
/// <param name="ptr">- object/data to be check if contain in list</param>
/// <returns>int - true or false if object/data contains in list</returns>
int LL_ContainsPtr(LinkedList* ll, void* ptr)
{
	if (LL_IsEmpty(ll))
		return 0;
	LinkedList* node = ll;
	for (; node != NULL; node = node->next)
	{
		if (node->curr == ptr)
			return 1;
	}
	return 0;
}
/// <summary>
/// Clears and delete all nodes
/// </summary>
/// <param name="ll">- Reference of the list</param>
void LL_Clear(LinkedList** ll)
{
	//if (LL_IsEmpty(ll))
	//	return NULL;
	while (*ll)
	{
		LinkedList* prev = *ll;
		*ll = (*ll)->next;
		free(prev);
	}
	//return NULL;
}
/// <summary>
/// Finds using the function, and returns the object/data if function returns something
/// </summary>
/// <param name="ll">- list of the object to be checked</param>
/// <param name="func">- lambda func to check thru the list. 
///						first param - object/data of the current node
///						second param - arguement </param>
/// <param name="arg">- argument to be passed to lambda func</param>
/// <returns>void* - whatever the lambda func returns</returns>
void* LL_Find(LinkedList* ll, void* func(void*, void*), void* arg)
{
	if (LL_IsEmpty(ll))
		return NULL;
	LinkedList* node = ll;
	while (node)
	{
		void* ret = func(node->curr, arg);
		if (ret != NULL)
			return ret;
		node = node->next;
	}
	return NULL;
}
/// <summary>
/// Reverse the list
/// </summary>
/// <param name="ll">- Reference of the list</param>
void LL_Reverse(LinkedList** ll)
{
	if (LL_IsEmpty(*ll))
		return;
	LinkedList* last = GetLastNode(*ll);
	LinkedList* temp = GetHead(*ll);
	last->next = last->prev;
	last->prev = NULL;
	while (temp != last)
	{
		LinkedList* p = temp->prev;
		temp->prev = temp->next;
		temp->next = p;
		temp = temp->prev;
	}
	*ll = last;
}
//void** LL_ToArray(LinkedList* ll)
//{
//	if (LL_IsEmpty(ll))
//		return NULL;
//	int count = LL_GetCount(ll);
//	void** ret = (void**)calloc(count, sizeof(void*));
//	LinkedList* node = ll;
//	for (int i = 0; i < count; ++i, node = node->next)
//	{
//		if (ret)
//			ret[i] = node->curr;
//	}
//	return ret;
//}