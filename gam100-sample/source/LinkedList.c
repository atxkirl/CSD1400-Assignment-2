#include "LinkedList.h"
#include <stdio.h> //for NULL
#include <stdlib.h>//for malloc

void RemoveThis(LinkedList* currPtr)
{
	if (currPtr->prev == NULL)
	{
		LinkedList* old = currPtr;
		currPtr = currPtr->next;
		free(old);
		return;
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
	return;
}

int LL_GetCount(LinkedList* ll)
{
	if (LL_IsEmpty(ll))
		return 0;
	int count = 1;
	while (ll->next != NULL)
	{
		ll = ll->next;
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
	while (ll != NULL)
	{
		if (count == index)
			return ll;
		ll = ll->next;
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
LinkedList* LL_Add(LinkedList* ll, void* ptr)
{
	LinkedList* lptr = LL_GetLast(ll);
	LinkedList* newPtr = malloc(sizeof(LinkedList));
	if (lptr == NULL)
	{
		ll = newPtr;
		if (ll) //to prevent warnings
		{
			ll->prev = NULL;
			ll->curr = ptr;
			ll->next = NULL;
		}

		return ll;
	}
	if (newPtr)
	{
		newPtr->prev = lptr;
		newPtr->curr = ptr;
		newPtr->next = NULL;
	}

	lptr->next = newPtr;
	return ll;
}
LinkedList* LL_SetAdd(LinkedList* ll, void* ptr)//Add unique
{
	if (LL_IsEmpty(ll))
		return LL_Add(ll, ptr);

	if (!LL_ContainsPtr(ll, ptr))
		return LL_Add(ll, ptr);
	return ll;
}

LinkedList* LL_RemoveLL(LinkedList* ll, LinkedList* ptr)
{
	if (LL_IsEmpty(ll))
		return NULL;

	if (LL_Contains(ll, ptr))
		RemoveThis(ptr);

	return ll;
}
LinkedList* LL_RemovePtr(LinkedList* ll, void* ptr)
{
	if (LL_IsEmpty(ll))
		return NULL;

	LinkedList* node = ll;
	while (node)
	{
		if (node->curr == ptr)
		{
			RemoveThis(node);
			return ll;
		}
		node = node->next;
	}
	//return head
	return ll;
}
//LinkedList* LL_Remove(LinkedList* ll, int index)
//{
//	LinkedList* rem = LL_Get(ll, index);
//	if (rem != NULL) //contains
//		ll = RemoveThis(rem);
//	return ll;
//}

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
	return (LL_GetIndexPtr(ll, ptr) < 0 ? 0 : 1);
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

LinkedList* LL_Clear(LinkedList* ll)
{
	if (LL_IsEmpty(ll))
		return NULL;
	while (ll)
	{
		LinkedList* prev = ll;
		ll = ll->next;
		free(prev);
	}
	return NULL;
}
void* LL_Find(LinkedList* ll, void* func(void*, void*), void* arg)
{
	if (LL_IsEmpty(ll))
		return NULL;
	while (ll)
	{
		void* ret = func(ll->curr, arg);
		if (ret != NULL)
			return ret;
		ll = ll->next;
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