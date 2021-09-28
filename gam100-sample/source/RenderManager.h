#pragma once

#include "GameObject.h"

//PREFIX : RM

GameObject* renderObjects[1];
int renderObjectCount;

void RM_Init();
void RM_AddRenderObject(GameObject*);
void RM_RemoveRenderObject(GameObject*);
void RM_ClearRenderObjects();
void RM_Render();