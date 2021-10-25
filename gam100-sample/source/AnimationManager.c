#include "AnimationManager.h"
#include "LinkedList.h"
#include <stdlib.h>
#include "RenderManager.h"

LinkedList* animationList;

void Update_SpriteAnimation(Animation* a, float dt)
{
	a->elapsedTime += CP_System_GetDt();
	float delay = 1.0f / a->fps;
	if (a->elapsedTime >= delay)
	{
		a->index = (a->index + 1) % a->frameCount;
		a->elapsedTime = 0.0f;

		float w = 1.0f / a->splitX;
		float h = 1.0f / a->splitY;
		float l = (a->index % a->splitX) * (w);
		float t = (a->index / a->splitX) * h;

		Renderer* r = RM_GetComponent(a->go);
		r->startUV = CP_Vector_Set(l,t);
		r->endUV = CP_Vector_Set(l + w, t + h);
	}
}

void AM_Init()
{
}

void AM_Update()
{
	int count = LL_GetCount(animationList);
	void** arr = LL_ToArray(animationList);
	for (int i = 0; i < count; ++i)
	{
		Animation* a = arr[i];
		Update_SpriteAnimation(a, CP_System_GetDt());
	}
}

void AM_Clear()
{
	LinkedList* node = animationList;
	for (; node; node = node->next)
	{
		free(node->curr);
	}
	//animationList = LL_Clear(animationList);
	LL_Clear(&animationList);
}

Animation* AM_AddComponent(GameObject* go)
{
	Animation* ani = malloc(sizeof(Animation));
	if (ani)
	{
		ani->go = go;
		ani->splitX = 1;
		ani->splitY = 1;
		ani->elapsedTime = 0.0f;
		ani->index = 0;
		ani->frameCount = 0;
		ani->fps = 0.0f;
	}
	LL_Add(&animationList, ani);
	return ani;
}

/*!
@brief Gets Animation from go object if go has added animation
@param curr - curr node data
@param arg - gameobject to find
@return animation component that exists
*/
void* FindAnimation(void* curr, void* arg)
{
	GameObject* g = (GameObject*)arg;
	Animation* a = (Animation*)curr;
	if (a->go == g)
		return a;
	return NULL;
}

Animation* AM_GetComponent(GameObject* go)
{
	return LL_Find(animationList, FindAnimation, go);
}

void AM_Remove(Animation* a)
{
	if (!LL_ContainsPtr(animationList, a))
		return;
	LL_RemovePtr(&animationList, (void*)a);
	free(a);
}

void AM_SetSprite(Animation* a, int x, int y, int f, float fps)
{
	a->splitX = x;
	a->splitY = y;
	a->frameCount = f;
	a->fps = fps;

	float w = 1.0f / a->splitX;
	float h = 1.0f / a->splitY;
	float l = (a->index % a->splitX) * (w);
	float t = (a->index / a->splitX) * h;
	Renderer* r = RM_GetComponent(a->go);
	r->startUV = CP_Vector_Set(l, t);
	r->endUV = CP_Vector_Set(l + w, t + h);
}
