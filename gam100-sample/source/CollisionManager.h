#pragma once

//PREFIX: CLM
#include "../CProcessing/inc/cprocessing.h"
#include "LinkedList.h"
#include "GameObject.h"

//#define CLM_RESPONSE_REMOVENONE 0
//#define CLM_RESPONSE_REMOVELEFT -1
//#define CLM_RESPONSE_REMOVERIGHT -2
//#define CLM_RESPONSE_REMOVEBOTH -3

/*
* Not all Gameobjects has collision. To create a collider for gameobject, call function CLM_AddCollider(...);
Currently only supports AABB for box

All existing colliders are checked when CLM_CheckCollisions() is called in update. TRY TO MAKE SURE there is no duplicate response. for example:
	ColliderA : player_collide_wall() { //do something }
	ColliderB : wall_collide_player() { //do something }
If player collide wall, "do something" will happen twice. To avoid this, pass in NULL for ColliderB
*/

/*!
@brief Type of collider
*/
typedef enum COLLIDER_TYPE
{
	COL_CIRCLE,
	COL_BOX,
	COL_POINT

} COLLIDER_TYPE;

/*!
@brief Colliders of the same space will check against each other
*/
typedef enum COLLIDER_SPACE
{
	COLSPC_WORLD,
	COLSPC_SCREEN
	//Can add more "space"

} COLLIDER_SPACE;

/*!
@brief Colliders of the same layer will NOT check against each other,
	Except NULL
*/
typedef enum COLLIDER_LAYER
{
	COLLAY_NULL,
	COLLAY_WALL
} COLLIDER_LAYER;

/*
GameObject* obj: reference to the gameobject. needs its positional information
OnCollision OnCollision: function pointer to the OnCollide() response. To be passed in during init if there is a collision 	response expected. Function pointer has to match int(Collider*, Collider*)
*/
typedef struct Collider Collider;
typedef void (*OnCollision)(Collider*, Collider*);
typedef struct Collider
{
	GameObject* obj;
	COLLIDER_TYPE type;
	COLLIDER_SPACE space;
	COLLIDER_LAYER layer;
	//CP_Vector velocity;
	int isKinematic;//aka isStatic, does not move due to force
	int isLockedPos;//aka unity's locked x,y
	int isEnabled;
	int isTrigger;
	int useScaleValue;
	float radius;
	float width, height;
	//Response. //-1 to remove left, 1 to remove right
	OnCollision OnCollision;

} Collider;


void CLM_Init();
void CLM_Add(Collider* );
void CLM_Set(Collider*, COLLIDER_TYPE, OnCollision);
Collider* CLM_AddComponent(GameObject*);
Collider* CLM_GetComponent(GameObject*);
int CLM_Remove(Collider*);
void CLM_RemoveGO(GameObject*);
void CLM_Clear();
int CLM_GetIndex(Collider*);
int IsCollide(Collider* left, Collider* right);
void CLM_Update();
int IsCircleCollideCircle(Collider* left, Collider* right);
int IsBoxCollideBox(Collider* left, Collider* right);
int IsPointCollidePoint(Collider* left, Collider* right);
int IsCircleCollideBox(Collider* left, Collider* right);
int IsCircleCollidePoint(Collider* left, Collider* right);
int IsBoxCollidePoint(Collider* left, Collider* right);