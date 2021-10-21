#pragma once

#include "GameObject.h"
#include "LinkedList.h"

//PREFIX : RM

/*
this is the worldspace size vertically. rendering is based on this size.
		meaning: in a 800x600 window, worldspace rendered will be (800/600 * 500, 500)
		in a 1280x720 window, worldspace will be (1280/720 * 500, 500)
	This ensures the area vertically remains the same, so larger window doesnt offers more view
	At the same time, aspect ratio of the obj rendered is not affected. a square will still look like a square
*/
#define WORLD_HEIGHT 500.0f



void RM_Init();
void RM_AddRenderObject(GameObject*);
void RM_RemoveRenderObject(GameObject*);
void RM_ClearRenderObjects();
void RM_Render();
/*!
@brief Set camera center point to the position param
@param position - Position for the camera to focus/look at
@return void
*/
void RM_SetCameraPosition(CP_Vector);
void RM_SetCameraScale(CP_Vector);
CP_Vector RM_GetCameraScale();