/*!
@file		CollisionManager.h
@author		Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief		Collision manager that handles all the colliders entity by checking for collisions among
			the entities
*/

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


typedef struct Collider Collider;
typedef void (*OnCollision)(Collider*, Collider*);
/*
GameObject* obj: reference to the gameobject. needs its positional information
OnCollision OnCollision: function pointer to the OnCollide() response. To be passed in during init if there is a collision 	response expected. Function pointer has to match int(Collider*, Collider*)
*/
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

/// <summary>
/// Inits the collision manager
/// </summary>
void CLM_Init();
/// <summary>
/// Adds collider to collision manager
/// </summary>
/// <param name="c">- Collider entity to be added to manager's list</param>
void CLM_Add(Collider* );
/// <summary>
/// Sets the collider entity to type with response
/// </summary>
/// <param name="c">- Collider entity to be set</param>
/// <param name="t">- type of collider</param>
/// <param name="func">- response func pointer to be stored</param>
void CLM_Set(Collider*, COLLIDER_TYPE, OnCollision);
/// <summary>
/// Adds collider component to game object
/// </summary>
/// <param name="g">- parent game object to have collider added</param>
/// <returns>- pointer to the newly created collider</returns>
Collider* CLM_AddComponent(GameObject*);
/// <summary>
/// Returns collider component of game object if exists
/// </summary>
/// <param name="go">- game object to find collider</param>
/// <returns>- pointer of collider if exists</returns>
Collider* CLM_GetComponent(GameObject*);
/// <summary>
/// Removes collider from manager
/// </summary>
/// <param name="c">- collier to be removed</param>
/// <returns>int - 0</returns>
int CLM_Remove(Collider*);
/// <summary>
/// Remove collider containing parent go
/// </summary>
/// <param name="go">- parent of the collider to be removed</param>
void CLM_RemoveGO(GameObject*);
/// <summary>
/// Clears and deletes memory allocated for collision manager
/// </summary>
void CLM_Clear();
/// <summary>
/// returns index of collider in collision manager
/// </summary>
/// <param name="c">- collider to find index of</param>
/// <returns>int - index of the collider in manager</returns>
int CLM_GetIndex(Collider*);
/// <summary>
/// checks if collider left and collide right collides with each other
/// </summary>
/// <param name="left">- one of the collider to be checked</param>
/// <param name="right">- other one of the collider to be checked</param>
/// <returns>int - 1 if is colliding, 0 if is not</returns>
int IsCollide(Collider* left, Collider* right);
/// <summary>
/// updates the collision manager every frame
/// </summary>
void CLM_Update();

/// <summary>
/// check if circle collide w circle
/// </summary>
/// <param name="left">- circle a</param>
/// <param name="right">- circle b</param>
/// <returns>int - true or false if collide</returns>
int IsCircleCollideCircle(Collider* left, Collider* right);
/// <summary>
/// check if box collide w box
/// </summary>
/// <param name="left">- box a</param>
/// <param name="right">- box b</param>
/// <returns>int - true or false if collide</returns>
int IsBoxCollideBox(Collider* left, Collider* right);
/// <summary>
/// check if point collide w point
/// </summary>
/// <param name="left">- point a</param>
/// <param name="right">- point b</param>
/// <returns>int - true or false if collide</returns>
int IsPointCollidePoint(Collider* left, Collider* right);
/// <summary>
/// check if circle collide w box
/// </summary>
/// <param name="left">- circle a</param>
/// <param name="right">- box b</param>
/// <returns>int - true or false if collide</returns>
int IsCircleCollideBox(Collider* left, Collider* right);
/// <summary>
/// check if circle collide w point
/// </summary>
/// <param name="left">- circle a</param>
/// <param name="right">- point b</param>
/// <returns>int - true or false if collide</returns>
int IsCircleCollidePoint(Collider* left, Collider* right);
/// <summary>
/// check if box collide w point
/// </summary>
/// <param name="left">- box a</param>
/// <param name="right">- point b</param>
/// <returns>int - true or false if collide</returns>
int IsBoxCollidePoint(Collider* left, Collider* right);