#include "LinkedList.h"
#include <stdio.h> //for NULL
#include <stdlib.h>//for malloc

LinkedList* RemoveThis(LinkedList* currPtr)
{
	if (currPtr->prev == NULL)
	{
		LinkedList* old = currPtr;
		currPtr = currPtr->next;
		free(old);
		return currPtr;
	}
	
	LinkedList* prev = currPtr->prev;
	LinkedList* next = currPtr->next;
	prev->next = currPtr->next;
	next->prev = prev;
	currPtr->prev = NULL;
	currPtr->next = NULL;
	currPtr->curr = NULL;
	free(currPtr);
	return prev;
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

	ll->next = newPtr;
	return ll;
}

LinkedList* LL_RemoveLL(LinkedList* ll, LinkedList* ptr)
{
	if (LL_IsEmpty(ll))
		return NULL;

	if (LL_Contains(ll, ptr))
		ll = RemoveThis(ptr);

	return ll;
}
LinkedList* LL_RemovePtr(LinkedList* ll, void* ptr)
{
	if (LL_IsEmpty(ll))
		return NULL;

	while (ll)
	{
		if (ll->curr == ptr)
			return RemoveThis(ll);
		ll = ll->next;
	}

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

	int index = 0;
	while (ll != NULL)
	{
		if (ll == ptr)
			return index;
		ll = ll->next;
		index++;
	}

	return -1;
}
int LL_GetIndexPtr(LinkedList* ll, void* ptr)
{
	if (LL_IsEmpty(ll))
		return -1;

	int index = 0;
	while (ll != NULL)
	{
		if (ll->curr == ptr)
			return index;
		ll = ll->next;
		index++;
	}

	return -1;
}

int LL_Contains(LinkedList* ll, LinkedList* ptr)
{
	return (LL_GetIndexPtr(ll, ptr) < 0 ? 0 : 1);
}
void LL_Clear(LinkedList* ll)
{
	if (LL_IsEmpty(ll))
		return;
	while (ll)
	{
		LinkedList* prev = ll;
		ll = ll->next;
		free(prev);
	}
	return;
}