#pragma once

/*!
@file SystemManager.h
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains function for all systems to initialize and exit cleanly
*//*______________________________________________________________________*/


#include "GameObjectManager.h"
#include "RenderManager.h"
#include "CollisionManager.h"
#include "AnimationManager.h"
#include "Dialogue.h"
#include "AIManager.h"

//Prefix: SM

typedef enum COMPONENT
{
	COM_RENDERER,
	COM_COLLISION,
	COM_ANIMATION,
	COM_COUNT
}COMPONENT;

/*!
@brief Inits all systems
@return void
*/
void SM_SystemsInit();
/*!
@brief Update systems at the start of the frame
@return void
*/
void SM_SystemsPreUpdate();
/*!
@brief Update systems
@return void
*/
void SM_SystemsUpdate();
/*!
@brief Update systems at the end of the frame
@return void
*/
void SM_SystemsLateUpdate();
/*!
@brief Exit and clean up systems
@return void
*/
void SM_SystemsExit();

/*!
@brief Find component that is attached to gameobject
@param g - gameobject to find
@param c - component id to find for
@return ptr of component
*/
void* SM_GetComponent(GameObject*, COMPONENT);
//void* SM_AddComponent(GameObject*, COMPONENT);

/*!
@brief Delete gameobject from all systems AFTER Update()
@param g - gameobject clean delete
@return void
*/
void SM_DeleteGameObject(GameObject*);