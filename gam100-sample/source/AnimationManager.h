#pragma once

//prefix: AM
#include "../CProcessing/inc/cprocessing.h"
#include "GameObject.h"

typedef enum ANIM_TYPE
{
	ANIM_SPRITE,
	ANIM_SHAKE
} ANIM_TYPE;
//think as a big struct that holds all the values for any animation sad
typedef struct Animation
{
	GameObject* go;
	ANIM_TYPE type;
	int isEnabled;

	int splitX, splitY;
	float elapsedTime;
	int index;
	int frameCount;
	float fps;

	//for shake shake. should union heh
	float rotateAngle;
	float defaultRotate;
	float loopTime;
	int loopCount;
	int loopCounter;
	int isContinuous;

}Animation;

/*!
@brief Init animation manager
*/
void AM_Init();
/*!
@brief Updates all animation component
*/
void AM_Update();
/*!
@brief clear all animations
*/
void AM_Clear();
/*!
@brief Adds Animation to go object
@param go - gameobject to have animation added to
@return animation component that is created
*/
Animation* AM_AddComponent(GameObject* go);
/*!
@brief Gets Animation from go object if go has added animation
@param go - gameobject to have animation retrieved
@return animation component that exists
*/
Animation* AM_GetComponent(GameObject* go);
/*!
@brief Removes and delete animation from list
@param a - animation to be deleted
@return void
*/
void AM_Remove(Animation*);

/*!
@brief Set Sprite animation for component
@param a - animation to be set
@param x - size of x split
@param y - size of y split
@param f - number of frames
@param fps - fps of animation to be played
@return void
*/
void AM_SetSprite(Animation* ,int, int, int, float);


/*!
@brief Set shake animation for component
@param a - animation to be set
@param rotateAngle - max angle to rotate
@param loopTime - how long for one sin curve
@param loopCount - how many loops
@param isContinuous - if the shake continues nonstop
@return void
*/
void AM_SetShake(Animation*, float, float, int, int);