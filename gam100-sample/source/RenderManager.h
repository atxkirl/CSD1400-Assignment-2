#pragma once

#include "GameObject.h"
#include "LinkedList.h"

//PREFIX : RM
#define WORLD_HEIGHT 500.0f


void RM_Init();
void RM_AddRenderObject(GameObject*);
void RM_RemoveRenderObject(GameObject*);
void RM_ClearRenderObjects();
void RM_Render();
void RM_SetCameraPosition(CP_Vector);
void RM_SetCameraScale(CP_Vector);
CP_Vector RM_GetCameraScale();