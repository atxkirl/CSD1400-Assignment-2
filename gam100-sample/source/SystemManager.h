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

//Prefix: SM

void SM_SystemsInit();
void SM_SystemsPreUpdate();
void SM_SystemsUpdate();
void SM_SystemsLateUpdate();
void SM_SystemsExit();

