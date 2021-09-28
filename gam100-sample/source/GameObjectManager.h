#pragma once

//Controls lifetime of gameobjects
//PREFIX: GOM

#include "GameObject.h"

GameObject* objects[1];
int objectCount;

void GOM_Init();
GameObject* GOM_CreateGameObject();
int GOM_Delete(GameObject*);
void GOM_Clear();
int GOM_GetIndex(GameObject*);