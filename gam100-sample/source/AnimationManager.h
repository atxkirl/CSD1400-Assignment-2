#pragma once

//prefix: AM
#include "../CProcessing/inc/cprocessing.h"
#include "GameObject.h"

//Cam also use for particles generation. dw create so many manager class

typedef enum ANIM_TYPE
{
	ANIM_SPRITE,
	ANIM_SHAKE,
	ANIM_ZOOM,
	ANIM_WALKSAND,
	ANIM_COLORBLINK
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
	int loopDir;

	//for shake shake. should union heh
	float rotateAngle;
	float defaultRotate;
	float loopTime;
	int loopCount;
	int loopCounter;
	int isContinuous;

	CP_Vector defaultScale;
	CP_Vector targetScale;
	float scaleToTime;
	int isFlipped;

	//for walk animatino
	CP_Vector oldPos;

	//for blink
	CP_Color targetColor;
	CP_Color defaultColor;
	//use looptime to go from default to target to default
	//use is continuous
	//use elapsedtime for et
	//use index for mode

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

/*!
@brief Set zoom animation for component
@param a - animation to be set
@param defaultScale - defaultscale of obj
@param targetScale - target scale value
@param scaleToTime - time from default to scale to target
@param isFlipped - is scale from default to target or flipped = target to default
@return void
*/
void AM_SetZoom(Animation*, CP_Vector, CP_Vector, float, int);

/*!
@brief Set a particle generator to generate particles when player walks/moves
@param a - animation to be set
@return void
*/
void AM_SetWalk(Animation*);

/*!
@brief Set animation to blinking color
@param a - animation to be set
@param defaultColor - default color
@param targetColor - target color
@param loopTime - time to loop from default to target to default
@param isContinuous - will it continue non stop
@param mode - 0 for fill, 1 for stroke, 2 for both
@return void
*/
void AM_SetBlink(Animation* a, CP_Color defaultColor, CP_Color targetColor, float loopTime, int isContinuous, int mode);