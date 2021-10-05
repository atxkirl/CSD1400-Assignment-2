#include "CollisionManager.h"


void CLM_Init()
{

}
void CLM_Add(Collider* c)
{
	objects = LL_Add(objects, (void*)c);
}
int CLM_Remove(Collider* c)
{
	objects = LL_RemovePtr(objects, (void*)(c));
	return 0;
}
void CLM_Clear()
{
	LL_Clear(objects);
}
int CLM_GetIndex(Collider* c)
{
	return LL_GetIndexPtr(objects, c);
}
