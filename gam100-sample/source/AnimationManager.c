#include "SystemManager.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <math.h>
#include "Helpers.h"

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
void Update_ShakeAnimation(Animation* a, float dt)
{
	a->elapsedTime += CP_System_GetDt();

	float r = a->elapsedTime / a->loopTime;
	if (r < 1.0f)
	{
		float rot = a->rotateAngle * sinf(r * 2.0f * 3.141592f);
		a->go->rotation = a->defaultRotate + rot;
	}
	else
	{
		a->go->rotation = a->defaultRotate;
		a->loopCounter++;
		a->elapsedTime = 0.0f;
	}

	if (a->isContinuous) return;

	if (a->loopCounter >= a->loopCount)
	{
		a->isEnabled = 0;
		a->loopCounter = 0;
	}
}
void Update_ZoomAnimation(Animation* a, float dt)
{
	a->elapsedTime += CP_System_GetDt();

	float r = a->elapsedTime / a->scaleToTime;
	if (r > 1.0f)
	{
		if (!a->isFlipped)
			a->go->scale = a->targetScale;
		else
			a->go->scale = a->defaultScale;

		a->elapsedTime = 0.0f;
		a->isEnabled = 0;
		return;
	}
	CP_Vector v = CP_Vector_Subtract(a->targetScale, a->defaultScale);
	v.x = v.x * r;
	v.y = v.y * r;

	if (!a->isFlipped) v = CP_Vector_Add(v, a->defaultScale);
	else v = CP_Vector_Add(CP_Vector_Negate(v), a->targetScale);

	a->go->scale = v;

	
}
void Update_WalkAnimation(Animation* a, float dt)
{
	if (CP_Vector_Distance(a->oldPos, a->go->position) > FLT_EPS)
	{
		a->elapsedTime += dt;
		if (a->elapsedTime > a->loopTime)
		{
			float t = 0.5f;
			CP_Vector pos = a->go->position;
			CP_Vector scale = CP_Vector_Set(20, 20);
			pos.y += a->go->scale.y * 0.5f - scale.y * 0.3f;
			float rot = CP_Random_RangeFloat(0.0f, 360.0f);
			GameObject* p = GOM_Create2(RECTANGLE, pos, rot, scale);
			Renderer* r = RM_AddComponent(p);
			RM_LoadImage(r, "Assets/walkparticle.png");
			Animation* ani = AM_AddComponent(p);
			AM_SetZoom(ani, scale, CP_Vector_Set(2.f, 2.f), t * 2.0f, 0);
			SM_DeleteGameObjectAfter(p, t);
			a->elapsedTime = 0.0f;
		}
		a->oldPos = a->go->position;
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
		if (!a->isEnabled || !a->go->isEnabled)
			continue;
		switch (a->type)
		{
		case ANIM_SHAKE:
			Update_ShakeAnimation(a, CP_System_GetDt());
			break;
		case ANIM_ZOOM:
			Update_ZoomAnimation(a, CP_System_GetDt());
			break;
		case ANIM_WALKSAND:
			Update_WalkAnimation(a, CP_System_GetDt());
			break;
		default:
			Update_SpriteAnimation(a, CP_System_GetDt());
			break;
		}
		
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
		ani->type = ANIM_SPRITE;

		ani->rotateAngle = 0.0f;
		ani->defaultRotate = 0.0f;
		ani->loopTime = 0.0f;
		ani->loopCount = 0;
		ani->loopCounter = 0;
		ani->isContinuous = 0;
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
	a->elapsedTime = 0.0f;
	a->splitX = x;
	a->splitY = y;
	a->frameCount = f;
	a->fps = fps;
	a->type = ANIM_SPRITE;

	float w = 1.0f / a->splitX;
	float h = 1.0f / a->splitY;
	float l = (a->index % a->splitX) * (w);
	float t = (a->index / a->splitX) * h;
	Renderer* r = RM_GetComponent(a->go);
	r->startUV = CP_Vector_Set(l, t);
	r->endUV = CP_Vector_Set(l + w, t + h);
}

void AM_SetShake(Animation* a, float rotateAngle, float loopTime, int loopCount, int isContinuous)
{
	a->elapsedTime = 0.0f;
	a->rotateAngle = rotateAngle;
	a->loopCount = loopCount;
	a->loopTime = loopTime;
	a->isContinuous = isContinuous;
	a->type = ANIM_SHAKE;

	a->defaultRotate = a->go->rotation;
}

void AM_SetZoom(Animation* a, CP_Vector defaultScale, CP_Vector targetScale, float scaleToTime, int isFlipped)
{
	a->elapsedTime = 0.0f;
	a->defaultScale = defaultScale;
	a->targetScale = targetScale;
	a->scaleToTime = scaleToTime;
	a->isFlipped = isFlipped;
	a->type = ANIM_ZOOM;
}

void AM_SetWalk(Animation* a)
{
	a->type = ANIM_WALKSAND;
	a->elapsedTime = 0.0f;
	a->loopTime = 0.5f;
	a->oldPos = a->go->position;
}
