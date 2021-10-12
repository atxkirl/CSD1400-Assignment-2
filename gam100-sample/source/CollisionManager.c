#include "CollisionManager.h"
//#include "GameObject.h"
#include <math.h> //for fabs()
#include "Helpers.h" //for FLT_EPS
#include <stdlib.h> //for malloc/calloc
#include <stdio.h>
#include <stdarg.h>

LinkedList* CLM_objects = NULL;

void CLM_Init()
{

}
void CLM_Add(Collider* c)
{
	CLM_objects = LL_Add(CLM_objects, (void*)c);
}
void CLM_AddCollider(GameObject* g, OnCollision func, COLLIDER_TYPE type, ...)
{
	Collider* c = malloc(sizeof(Collider));
	if (c)
	{
		c->obj = g;
		c->type = type;
		c->isEnabled = 1;
		c->OnCollision = func;
		switch (type)
		{
		case COL_CIRCLE:
		{
			va_list arg;
			va_start(arg, type);
			float r = (float)va_arg(arg, double); //float variables are changed to double after being passed thr ...
			c->radius = r;
			va_end(arg);
		}
		break;
		case COL_BOX:
		{
			va_list arg;
			va_start(arg, type);
			float w = (float)va_arg(arg, double);
			float h = (float)va_arg(arg, double);
			c->width = w;
			c->height = h;
			va_end(arg);
		}
			break;
		default: //point
			break;
		}
	}
	CLM_objects = LL_Add(CLM_objects, c);

}
int CLM_Remove(Collider* c)
{
	CLM_objects = LL_RemovePtr(CLM_objects, (void*)(c));
	free(c);
	return 0;
}

//Need "lambda" func for LL_find
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
		CLM_objects = LL_RemovePtr(CLM_objects, (void*)c);
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
	CLM_objects = LL_Clear(CLM_objects);
}
int CLM_GetIndex(Collider* c)
{
	return LL_GetIndexPtr(CLM_objects, c);
}
void CLM_CheckCollisions()
{
	void** objArray = LL_ToArray(CLM_objects);
	int size = LL_GetCount(CLM_objects);
	LinkedList* delList = NULL;
	for (int i = 0; i < size; ++i)
	{
		Collider* left = (Collider*)objArray[i];
		if (!left->isEnabled)
			continue;
		for (int j = i + 1; j < size; ++j)
		{
			Collider* right = (Collider*)objArray[j];
			if (!right->isEnabled)
				continue;
			
			if (IsCollide(left, right))
			{
				//response
				int ret = CLM_RESPONSE_REMOVENONE;
				int ret2 = CLM_RESPONSE_REMOVENONE;
				if (left->OnCollision != NULL)
					ret = left->OnCollision(left, right);
				if (right->OnCollision != NULL)
					ret2 = right->OnCollision(right, left);

				switch (ret)
				{
				case CLM_RESPONSE_REMOVELEFT: delList = LL_SetAdd(delList, (void*)left);
					break;
				case CLM_RESPONSE_REMOVERIGHT: delList = LL_SetAdd(delList, (void*)right);
					break;
				case CLM_RESPONSE_REMOVEBOTH:
					delList = LL_SetAdd(delList, (void*)left);
					delList = LL_SetAdd(delList, (void*)right);
					break;
				}
					
				switch (ret2)
				{
				case CLM_RESPONSE_REMOVELEFT: delList = LL_SetAdd(delList, (void*)right);
					break;
				case CLM_RESPONSE_REMOVERIGHT: delList = LL_SetAdd(delList, (void*)left);
					break;
				case CLM_RESPONSE_REMOVEBOTH:
					delList = LL_SetAdd(delList, (void*)left);
					delList = LL_SetAdd(delList, (void*)right);
					break;
				}
				

			}
		}
	}

	//printf("SIZE del %d", LL_GetCount(CLM_objects));
	for (; delList; delList = delList->next)
	{
		//printf("Removing from CollisionManager %s", ((Collider*)delList->curr)->obj->tag);
		CLM_objects = LL_RemovePtr(CLM_objects, delList->curr);
	}
	delList = LL_Clear(delList);
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
	if (fabs(relative.x) < left->width * 0.5f + right->width * 0.5f &&
		fabs(relative.y) < left->height * 0.5f + right->height * 0.5f)
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
	if (fabs(relative.x) < left->radius + right->width * 0.5f &&
		fabs(relative.y) < left->radius + right->height * 0.5f)
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
	if (fabs(relative.x) < left->width * 0.5f &&
		fabs(relative.y) < left->height * 0.5f)
		return 1;
	return 0;
}
