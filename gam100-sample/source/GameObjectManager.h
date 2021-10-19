#pragma once

//Controls lifetime of gameobjects
//PREFIX: GOM

#include "GameObject.h"
#include "LinkedList.h"


void GOM_Init();

/*!
@brief Creates a GameObject and return the pointer to it
This function takes as input a type and priority.
Gameobject is automatically stored in container for better control of lifespan.
Special scenarios: -
@param type - the type of the object is used on how it is rendered
@param priority - higher priority means render on top
@return pointer to gameobject created
*/
GameObject* GOM_CreateGameObject(OBJECT_TYPE type, RENDER_PRIORITY priority);

int GOM_Delete(GameObject*);
void GOM_Clear();
int GOM_GetIndex(GameObject*);


GameObject* GOM_FactoryCreateGO(int type);//int according to OBJECT_TYPE