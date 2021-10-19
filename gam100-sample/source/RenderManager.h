#pragma once

#include "GameObject.h"
#include "LinkedList.h"

//PREFIX : RM


void RM_Init();
void RM_AddRenderObject(GameObject*);
void RM_RemoveRenderObject(GameObject*);
void RM_ClearRenderObjects();
void RM_Render();