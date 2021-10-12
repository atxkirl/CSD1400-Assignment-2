#pragma once

#include "GameObject.h"
#include "LinkedList.h"

//PREFIX : RM

LinkedList* renderObjects;

void RM_Init();
void RM_AddRenderObject(GameObject*);
void RM_RemoveRenderObject(GameObject*);
void RM_ClearRenderObjects();
void RM_Render();