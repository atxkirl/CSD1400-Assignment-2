#include "CollisionManager.h"
#include <math.h> //for fabs()
#include "Helpers.h" //for FLT_EPS
#include <stdlib.h> //for malloc/calloc
#include <stdio.h>
#include <stdarg.h>

LinkedList* CLM_objects = NULL;


/*!
@brief Snap Circle object out of another Circle object.
	Snaps both position to nearest free space
@param l - one of circle collider
@param r - another circle collider
@return void
*/
void SnapCircleOutOfCircle(Collider* l, Collider* r)
{
	//See which one is locked pos, if locked means the one no lock take full movement, else share half
	if (l->isLockedPos && r->isLockedPos) //both locked pos and somehow they colliding.
		return; //since both locked means both cannot move 
	float lWeight = 0.5f;
	float rWeight = 0.5f;
	l->isLockedPos ? (lWeight = 0.0f, rWeight = 1.0f) :
		r->isLockedPos ? (lWeight = 1.0f, rWeight = 0.0f) :
		(lWeight = rWeight = 0.5f);

	//imagine left is stationary
	GameObject* lGo = l->obj;
	GameObject* rGo = r->obj;
	CP_Vector vLtoR = CP_Vector_Subtract(rGo->position, lGo->position);
	//CP_Vector midPoint = CP_Vector_Add(lGo->position, CP_Vector_Scale(vLtoR, 0.5f));
	float snapLen = l->radius + r->radius;
	float intersectLen = snapLen - CP_Vector_Length(vLtoR);
	CP_Vector vhatLtoR = CP_Vector_Normalize(vLtoR);
	//case where normalize causes problem when lpos == rpos, just assume go down
	if (fabs(vhatLtoR.x) < FLT_EPS && fabs(vhatLtoR.y) < FLT_EPS)
		vhatLtoR = CP_Vector_Set(0, 1);
	CP_Vector vhatRtoL = CP_Vector_Negate(vhatLtoR);
	rGo->position = CP_Vector_Add(rGo->position, CP_Vector_Scale(vhatLtoR, intersectLen * rWeight));
	lGo->position = CP_Vector_Add(lGo->position, CP_Vector_Scale(vhatRtoL, intersectLen * lWeight));
}
/*!
@brief Snap Circle object out of another Box object.
	Snaps both position to nearest free space
@param l - circle collider
@param r - box collider
@return void
*/
void SnapCircleOutOfBox(Collider* l, Collider* r)
{
	if (l->isLockedPos && r->isLockedPos) //both locked pos and somehow they colliding.
		return; //since both locked means both cannot move 
	float lWeight = 0.5f;
	float rWeight = 0.5f;
	l->isLockedPos ? (lWeight = 0.0f, rWeight = 1.0f) :
		r->isLockedPos ? (lWeight = 1.0f, rWeight = 0.0f) :
		(lWeight = rWeight = 0.5f);

	//imagine left is stationary
	GameObject* lGo = l->obj;
	GameObject* rGo = r->obj;

	CP_Vector BoxToCircle = CP_Vector_Subtract(lGo->position, rGo->position);
	float a = CP_Vector_Angle(CP_Vector_Set(r->width * 0.5f, r->height * 0.5f), CP_Vector_Set(1.f, 0.f));
	float b = CP_Vector_Angle(BoxToCircle, CP_Vector_Set(1.f, 0.f));
	if (b <= a)
	{
		//on rightside
		float intersectLen = (r->width * 0.5f + l->radius) - (float)fabs(BoxToCircle.x);
		lGo->position.x += intersectLen * lWeight;
		rGo->position.x -= intersectLen * rWeight;
		return;
	}
	a = CP_Vector_Angle(CP_Vector_Set(-r->width * 0.5f, -r->height * 0.5f), CP_Vector_Set(-1.f, 0.f));
	b = CP_Vector_Angle(BoxToCircle, CP_Vector_Set(-1.f, 0.f));
	if (b <= a)
	{
		float intersectLen = (r->width * 0.5f + l->radius) - (float)fabs(BoxToCircle.x);
		lGo->position.x -= intersectLen * lWeight;
		rGo->position.x += intersectLen * rWeight;
		return;
	}
	a = CP_Vector_Angle(CP_Vector_Set(r->width * 0.5f, -r->height * 0.5f), CP_Vector_Set(0.f, -1.f));
	b = CP_Vector_Angle(BoxToCircle, CP_Vector_Set(0.f, -1.f));
	if (b <= a)
	{
		float intersectLen = (r->height * 0.5f + l->radius) - (float)fabs(BoxToCircle.y);
		lGo->position.y -= intersectLen * lWeight;
		rGo->position.y += intersectLen * rWeight;
		return;
	}
	a = CP_Vector_Angle(CP_Vector_Set(-r->width * 0.5f, r->height * 0.5f), CP_Vector_Set(0.f, 1.f));
	b = CP_Vector_Angle(BoxToCircle, CP_Vector_Set(0.f, 1.f));
	if (b <= a)
	{
		float intersectLen = (r->height * 0.5f + l->radius) - (float)fabs(BoxToCircle.y);
		lGo->position.y += intersectLen * lWeight;
		rGo->position.y -= intersectLen * rWeight;
		return;
	}

}
/*!
@brief Snap Box object out of another Box object.
	Snaps both position to nearest free space
@param l - box collider
@param r - box collider
@return void
*/
void SnapBoxOutOfBox(Collider* l, Collider* r)
{
	if (l->isLockedPos && r->isLockedPos) //both locked pos and somehow they colliding.
		return; //since both locked means both cannot move 
	float lWeight = 0.5f;
	float rWeight = 0.5f;
	l->isLockedPos ? (lWeight = 0.0f, rWeight = 1.0f) :
		r->isLockedPos ? (lWeight = 1.0f, rWeight = 0.0f) :
		(lWeight = rWeight = 0.5f);

	//imagine left is stationary
	GameObject* lGo = l->obj;
	GameObject* rGo = r->obj;

	CP_Vector BoxToCircle = CP_Vector_Subtract(lGo->position, rGo->position);
	float a = CP_Vector_Angle(CP_Vector_Set(r->width * 0.5f, r->height * 0.5f), CP_Vector_Set(1.f, 0.f));
	float b = CP_Vector_Angle(BoxToCircle, CP_Vector_Set(1.f, 0.f));
	if (b <= a)
	{
		//on rightside
		float intersectLen = (r->width * 0.5f + l->width * 0.5f) - (float)fabs(BoxToCircle.x);
		lGo->position.x += intersectLen * lWeight;
		rGo->position.x -= intersectLen * rWeight;
		return;
	}
	a = CP_Vector_Angle(CP_Vector_Set(-r->width * 0.5f, -r->height * 0.5f), CP_Vector_Set(-1.f, 0.f));
	b = CP_Vector_Angle(BoxToCircle, CP_Vector_Set(-1.f, 0.f));
	if (b <= a)
	{
		float intersectLen = (r->width * 0.5f + l->width * 0.5f) - (float)fabs(BoxToCircle.x);
		lGo->position.x -= intersectLen * lWeight;
		rGo->position.x += intersectLen * rWeight;
		return;
	}
	a = CP_Vector_Angle(CP_Vector_Set(r->width * 0.5f, -r->height * 0.5f), CP_Vector_Set(0.f, -1.f));
	b = CP_Vector_Angle(BoxToCircle, CP_Vector_Set(0.f, -1.f));
	if (b <= a)
	{
		float intersectLen = (r->height * 0.5f + l->height * 0.5f) - (float)fabs(BoxToCircle.y);
		lGo->position.y -= intersectLen * lWeight;
		rGo->position.y += intersectLen * rWeight;
		return;
	}
	a = CP_Vector_Angle(CP_Vector_Set(-r->width * 0.5f, r->height * 0.5f), CP_Vector_Set(0.f, 1.f));
	b = CP_Vector_Angle(BoxToCircle, CP_Vector_Set(0.f, 1.f));
	if (b <= a)
	{
		float intersectLen = (r->height * 0.5f + l->height * 0.5f) - (float)fabs(BoxToCircle.y);
		lGo->position.y += intersectLen * lWeight;
		rGo->position.y -= intersectLen * rWeight;
		return;
	}

}

void CLM_Init()
{

}
void CLM_Add(Collider* c)
{
	LL_Add(&CLM_objects, (void*)c);
}
void CLM_Set(Collider* c, COLLIDER_TYPE t, OnCollision func)
{
	c->type = t;
	if (c->useScaleValue)
	{
		c->width = c->obj->scale.x;
		c->height = c->obj->scale.y;
		c->radius = c->obj->scale.x * 0.5f;
	}
	c->OnCollision = func;
}
Collider* CLM_AddComponent(GameObject* g)
{
	Collider* c = malloc(sizeof(Collider));
	if (c)
	{
		c->obj = g;
		c->type = COL_POINT;
		c->space = COLSPC_WORLD;
		c->layer = COLLAY_NULL;
		//c->velocity = CP_Vector_Set(0, 0);
		c->isKinematic = 0;
		c->isLockedPos = 0;
		c->isEnabled = 1;
		c->isTrigger = 0;
		c->OnCollision = NULL;
		c->useScaleValue = 1;
		c->radius = g->scale.x * 0.5f;
		c->width = g->scale.x;
		c->height = g->scale.y;
		LL_Add(&CLM_objects, c);
	}
	return c;
}
int CLM_Remove(Collider* c)
{
	LL_RemovePtr(&CLM_objects, (void*)(c));
	free(c);
	return 0;
}

/*!
@brief "Lambda" function to find collider with matching go
@param curr - ptr of curr data to be checked
@param arg - ptr of argument
@return ptr of found obj
*/
void* findGO(void* curr, void* arg)
{
	GameObject* obj = (GameObject*)arg;
	Collider* s = (Collider*)curr;

	if (s->obj == obj)//match
		return curr;
	return NULL;
}
void CLM_RemoveGO(GameObject* go)
{
	Collider* c = LL_Find(CLM_objects, findGO, go);
	//printf("REMOVE!");
	if (c)
	{
		LL_RemovePtr(&CLM_objects, (void*)c);
		free(c);
	}
}
void CLM_Clear()
{
	LinkedList* l = CLM_objects;
	while (l)
	{
		free(l->curr);
		l = l->next;
	}
	LL_Clear(&CLM_objects);
}
int CLM_GetIndex(Collider* c)
{
	return LL_GetIndexPtr(CLM_objects, c);
}
void CLM_Update()
{
	void** objArray = LL_ToArray(CLM_objects);
	int size = LL_GetCount(CLM_objects);
	for (int i = 0; i < size; ++i)
	{
		Collider* left = (Collider*)objArray[i];
		if (left->useScaleValue)
		{
			left->radius = left->obj->scale.x * 0.5f;
			left->width = left->obj->scale.x;
			left->height = left->obj->scale.y;
		}
		if (!left->isEnabled || !left->obj->isEnabled)
			continue;
		for (int j = i + 1; j < size; ++j)
		{
			Collider* right = (Collider*)objArray[j];
			if (right->useScaleValue)
			{
				right->radius = right->obj->scale.x * 0.5f;
				right->width = right->obj->scale.x;
				right->height = right->obj->scale.y;
			}
			if (!right->isEnabled || !right->obj->isEnabled)
				continue;
			if (right->space != left->space) //only same space collider w each other
				continue;
			if (left->layer != COLLAY_NULL && right->layer != COLLAY_NULL)
				if (left->layer == right->layer)
					continue;
			
			if (IsCollide(left, right))
			{
				if (!left->isTrigger && !right->isTrigger)
				{
					if (left->type == COL_CIRCLE && right->type == COL_CIRCLE)
						SnapCircleOutOfCircle(left, right);
					else if (left->type == COL_CIRCLE && right->type == COL_BOX)
						SnapCircleOutOfBox(left, right);
					else if (left->type == COL_BOX && right->type == COL_CIRCLE)
						SnapCircleOutOfBox(right, left);
					else if (left->type == COL_BOX && right->type == COL_BOX)
						SnapBoxOutOfBox(left, right);
				}

				//response
				if (left->OnCollision != NULL)
					left->OnCollision(left, right);
				if (right->OnCollision != NULL)
					right->OnCollision(right, left);
			}
		}
	}
}

int IsCollide(Collider* left, Collider* right)
{
	switch (left->type)
	{
	case COL_CIRCLE:
		switch (right->type)
		{
		case COL_CIRCLE:
			return IsCircleCollideCircle(left, right);
		case COL_BOX:
			return IsCircleCollideBox(left, right);
		case COL_POINT:
			return IsCircleCollidePoint(left, right);
		}
	case COL_BOX:
		switch (right->type)
		{
		case COL_CIRCLE:
			return IsCircleCollideBox(right, left);
		case COL_BOX:
			return IsBoxCollideBox(left, right);
		case COL_POINT:
			return IsBoxCollidePoint(left, right);
		}
	case COL_POINT:
		switch (right->type)
		{
		case COL_CIRCLE:
			return IsCircleCollidePoint(right, left);
		case COL_BOX:
			return IsBoxCollidePoint(right, left);
		case COL_POINT:
			return IsPointCollidePoint(left, right);
		}
	}
	return 0;
}
int IsCircleCollideCircle(Collider* left, Collider* right)
{
	CP_Vector posLeft = ((GameObject*)left->obj)->position;
	CP_Vector posRight = ((GameObject*)right->obj)->position;

	CP_Vector relative = CP_Vector_Subtract(posRight, posLeft);
	if (CP_Vector_Length(relative) < left->radius + right->radius)
		return 1;
	return 0;
}
int IsBoxCollideBox(Collider* left, Collider* right)
{
	CP_Vector posLeft = ((GameObject*)left->obj)->position;
	CP_Vector posRight = ((GameObject*)right->obj)->position;

	CP_Vector relative = CP_Vector_Subtract(posRight, posLeft);
	if (fabsf(relative.x) < left->width * 0.5f + right->width * 0.5f &&
		fabsf(relative.y) < left->height * 0.5f + right->height * 0.5f)
		return 1;
	return 0;
}
int IsPointCollidePoint(Collider* left, Collider* right) //No such thing well...
{
	CP_Vector posLeft = ((GameObject*)left->obj)->position;
	CP_Vector posRight = ((GameObject*)right->obj)->position;

	CP_Vector relative = CP_Vector_Subtract(posRight, posLeft);
	if (CP_Vector_Length(relative) < FLT_EPS)
		return 1;
	return 0;
}

int IsCircleCollideBox(Collider* left, Collider* right)
{
	CP_Vector posLeft = ((GameObject*)left->obj)->position;
	CP_Vector posRight = ((GameObject*)right->obj)->position;

	CP_Vector relative = CP_Vector_Subtract(posRight, posLeft);
	if (fabsf(relative.x) < left->radius + right->width * 0.5f &&
		fabsf(relative.y) < left->radius + right->height * 0.5f)
		return 1;
	return 0;
}
int IsCircleCollidePoint(Collider* left, Collider* right)
{
	CP_Vector posLeft = ((GameObject*)left->obj)->position;
	CP_Vector posRight = ((GameObject*)right->obj)->position;

	CP_Vector relative = CP_Vector_Subtract(posRight, posLeft);
	if (CP_Vector_Length(relative) < left->radius)
		return 1;
	return 0;
}
int IsBoxCollidePoint(Collider* left, Collider* right)
{
	CP_Vector posLeft = ((GameObject*)left->obj)->position;
	CP_Vector posRight = ((GameObject*)right->obj)->position;

	CP_Vector relative = CP_Vector_Subtract(posRight, posLeft);
	//printf("%f,%f %f,%f\n", relative.x, relative.y, left->width * 0.5f, left->height * 0.5f);
	if (fabsf(relative.x) < left->width * 0.5f &&
		fabsf(relative.y) < left->height * 0.5f)
		return 1;
	return 0;
}


Collider* CLM_GetComponent(GameObject* go)
{
	return LL_Find(CLM_objects, findGO, go);
}
