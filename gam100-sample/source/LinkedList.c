#include "LinkedList.h"
#include <stdio.h> //for NULL
#include <stdlib.h>//for malloc


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
//Returns node that took the slot
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
int LL_IsEmpty(LinkedList* ll)
{
	return ll == NULL;
}
LinkedList* LL_Get(LinkedList* ll, int index)
{
	if (LL_IsEmpty(ll))
		return NULL;
	int count = 0;
	LinkedList* node = ll;
	while (node != NULL)
	{
		if (count == index)
			return node;
		node = node->next;
		count++;
	}
	return NULL;
}
LinkedList* LL_GetLast(LinkedList* ll)
{
	if (LL_IsEmpty(ll))
		return NULL;
	LinkedList* ret = ll;

	while (ret->next != NULL)
		ret = ret->next;
	return ret;
}
void LL_Add(LinkedList** ll, void* ptr)
{
	LinkedList* lptr = LL_GetLast(*ll);
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
void LL_SetAdd(LinkedList** ll, void* ptr)//Add unique
{
	if (!LL_ContainsPtr(*ll, ptr))
		LL_Add(ll, ptr);
	//return ll;
}

void LL_RemoveLL(LinkedList** ll, LinkedList* node)
{
	if (LL_Contains(*ll, node))
	{
		//LinkedList* ret = RemoveThis(node);
		//if (LL_IsEmpty(ret))
		//	return NULL;
		*ll = RemoveThis(node);
	}

	//return ll;
}
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
			return;
		}
		node = node->next;
	}
	//return head
	//return ll;
}
void LL_RemoveIndex(LinkedList** ll, int index)
{
	LinkedList* rem = LL_Get(*ll, index);
	if (!rem)
		return;
	*ll = RemoveThis(rem);
	//if (LL_IsEmpty(*ll))
	//	return NULL;
	
	//return ll;
}

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

int LL_Contains(LinkedList* ll, LinkedList* ptr)
{
	return (LL_GetIndexPtr(ll, ptr->curr) < 0 ? 0 : 1);
}
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
void** LL_ToArray(LinkedList* ll)
{
	if (LL_IsEmpty(ll))
		return NULL;
	int count = LL_GetCount(ll);
	void** ret = (void**)calloc(count, sizeof(void*));
	LinkedList* node = ll;
	for (int i = 0; i < count; ++i, node = node->next)
	{
		if (ret)
			ret[i] = node->curr;
	}
	return ret;
}