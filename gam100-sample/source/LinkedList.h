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
LinkedList* LL_Add(LinkedList*, void*);
LinkedList* LL_RemoveLL(LinkedList*, LinkedList*);
LinkedList* LL_RemovePtr(LinkedList*, void*);
//LinkedList* LL_Remove(LinkedList*, int);
int LL_GetIndexLL(LinkedList*, LinkedList*);
int LL_GetIndexPtr(LinkedList*, void*);
int LL_Contains(LinkedList*, LinkedList*);
LinkedList* LL_Clear(LinkedList*);
void* LL_Find(LinkedList*, void*(void*, void*), void*);