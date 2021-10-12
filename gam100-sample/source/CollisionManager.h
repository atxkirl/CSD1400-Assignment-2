#pragma once

//PREFIX: CLM
#include "../CProcessing/inc/cprocessing.h"
#include "LinkedList.h"
#include "GameObject.h"

#define CLM_RESPONSE_REMOVENONE 0
#define CLM_RESPONSE_REMOVELEFT -1
#define CLM_RESPONSE_REMOVERIGHT -2
#define CLM_RESPONSE_REMOVEBOTH -3

//typedef struct GameObject GameObject;
typedef enum COLLIDER_TYPE
{
	COL_CIRCLE,
	COL_BOX,
	COL_POINT

} COLLIDER_TYPE;

typedef struct Collider Collider;
typedef int (*OnCollision)(Collider*, Collider*);
typedef struct Collider
{
	GameObject* obj;
	COLLIDER_TYPE type;
	CP_Vector velocity;
	int isEnabled;
	float radius;
	float width, height;
	//Response. //-1 to remove left, 1 to remove right
	OnCollision OnCollision;

} Collider;


void CLM_Init();
void CLM_Add(Collider* );
void CLM_AddCollider(GameObject*, OnCollision, COLLIDER_TYPE type, ...);
int CLM_Remove(Collider*);
void CLM_RemoveGO(GameObject*);
void CLM_Clear();
int CLM_GetIndex(Collider*);
int IsCollide(Collider* left, Collider* right);
void CLM_CheckCollisions();
int IsCircleCollideCircle(Collider* left, Collider* right);
int IsBoxCollideBox(Collider* left, Collider* right);
int IsPointCollidePoint(Collider* left, Collider* right);
int IsCircleCollideBox(Collider* left, Collider* right);
int IsCircleCollidePoint(Collider* left, Collider* right);
int IsBoxCollidePoint(Collider* left, Collider* right);
