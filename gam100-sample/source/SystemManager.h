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

//Prefix: SM

typedef enum COMPONENT
{
	COM_RENDERER,
	COM_COLLISION,
	COM_ANIMATION,
	COM_COUNT
}COMPONENT;


void SM_SystemsInit();
void SM_SystemsPreUpdate();
void SM_SystemsUpdate();
void SM_SystemsLateUpdate();
void SM_SystemsExit();

void* SM_GetComponent(GameObject*, COMPONENT);
//void* SM_AddComponent(GameObject*, COMPONENT);
void SM_DeleteFromAllSystems(GameObject*);