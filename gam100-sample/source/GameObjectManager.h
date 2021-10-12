#pragma once

//Controls lifetime of gameobjects
//PREFIX: GOM

#include "GameObject.h"
#include "LinkedList.h"


void GOM_Init();
GameObject* GOM_CreateGameObject();
int GOM_Delete(GameObject*);
void GOM_Clear();
int GOM_GetIndex(GameObject*);


GameObject* GOM_FactoryCreateGO(int type);//int according to OBJECT_TYPE