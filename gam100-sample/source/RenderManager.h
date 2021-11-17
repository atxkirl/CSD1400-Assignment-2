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
//#define SCREEN_HEIGHT 500.0f;

typedef enum RENDER_PRIORITY //lowest to highest
{
	PRI_GAME_OBJECT,
	PRI_UI,
}RENDER_PRIORITY;


typedef struct Renderer
{
	GameObject* go;
	CP_Image sprite;
	CP_Color color;
	enum RENDER_PRIORITY renderPriority;
	int width;
	int height;

	CP_Vector startUV;
	CP_Vector endUV;

	char* text;//to be directly rendered on top of object
	CP_Color textColor;//defaulted black
	CP_Vector textLocalPosition;//w reference to position
	CP_Vector textScale;//defaulted 1,1
	float textRotation;

	int isEnabled;
} Renderer;


typedef struct DebugLine
{
	CP_Vector from, to;
	RENDER_PRIORITY space;
	CP_Color color;
} DebugLine;


void RM_Init();
Renderer* RM_AddComponent(GameObject*);
Renderer* RM_GetComponent(GameObject*);
void RM_RemoveRenderObject(Renderer*);
void RM_Clear();
void RM_Render();
/*!
@brief Set camera center point to the position param
@param position - Position for the camera to focus/look at
@return void
*/
void RM_SetCameraPosition(CP_Vector);
CP_Vector RM_GetCameraPosition();
void RM_SetCameraScale(CP_Vector);
CP_Vector RM_GetCameraScale();

/*!
@brief Loads and attach image to renderer from file path
@param r - Renderer for image to be added to
@param filepath - filepath of the image
@return ptr of the renderer attached
*/
Renderer* RM_LoadImage(Renderer*, const char*);
/*!
@brief attach image to renderer
@param r - Renderer for image to be added to
@param img - image to be attached to renderer
@return ptr of the renderer attached
*/
Renderer* RM_AddImage(Renderer*, CP_Image);
/*!
@brief deletes image from renderer
@param r - Renderer for image to be deleted
@return ptr of the renderer
*/
Renderer* RM_DeleteImage(Renderer*);

/*!
@brief assigns the width and height of screen/world size
@param width - ptr for width value to be assigned
@param height - ptr for height value to be assigned
@param space - which space of the width and height
@return void
*/
void RM_GetRenderSize(float* width, float* height, RENDER_PRIORITY space);

/*!
@brief Returns mouse position on world space
@param x - x pos of the mouse in screen pixel space
@param y - y pos of the mouse in screen pixel space
@return CP_Vector position of the mouse in worldspace
*/
CP_Vector RM_MousePositionToWorldSpace(float x, float y);

/*!
@brief Zooms in the camera. Renders objects larger
@param z - zoom by value. 2.0 zoom means can see half of original screen
@return void
*/
void RM_SetCameraZoom(float);

/*!
@brief Set text to r->text
@param r - renderer for text to be set
@param text - null terminated text to be set
@return void
*/
void RM_SetText(Renderer*, const char* );

/*!
@brief draw line from position to position in space. it will delete itself after drawn in frame
@param from - start position of line
@param to - end position of line
@param space - space of line world or screen
@param color - color of line
@return void
*/
void RM_DebugDrawLine(CP_Vector from, CP_Vector to, RENDER_PRIORITY space, CP_Color color);