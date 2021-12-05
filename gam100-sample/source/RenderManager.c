/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file		RendereManager.c
@author		Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team		BananaBoi
@brief		Render Manager that handles all entities that renders on screen. Also defines the 
			world space and screen space system which the entities renders on. Also sorts 
			render orders based on priority.
*//*________________________________________________________________________________________*/

#include "RenderManager.h"
#include "GameObjectManager.h"
#include "MatrixStack.h"

#include <stdio.h>
#include <stdlib.h>

LinkedList* renderObjects = NULL;
LinkedList* matrixStack = NULL;
LinkedList* debugLinesList = NULL;

CP_Vector cameraPos;
CP_Vector cameraScale;
float zoom = 1.0f;

//private functions

/// <summary>
/// Renders all objects in this list
/// </summary>
/// <param name="list">- list of objects to be rendered</param>
void RenderAllInList(LinkedList* list);

/// <summary>
/// Inits the render manager
/// </summary>
void RM_Init() 
{
	matrixStack = MS_InitIdentity();
	//MS_Print(MS_Top(matrixStack));
	//MS_Print(matrixStack->curr);
	cameraPos = CP_Vector_Set(0.0f, 0.0f);
	cameraScale = CP_Vector_Set(1.0f, 1.0f);
	zoom = 1.0f;

	CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_NEAREST);

	CP_Font_Set(CP_Font_Load("Assets/pixel-font.ttf"));
}
/// <summary>
/// Adds render component to the game object
/// </summary>
/// <param name="g">- parent gameobject to have renderer added to</param>
/// <returns>Renderer* - pointer of newly created renderer</returns>
Renderer* RM_AddComponent(GameObject* g)
{
	Renderer* r = (Renderer*)malloc(sizeof(Renderer));
	if (r)
	{
		r->go = g;
		r->sprite = NULL;
		r->color = CP_Color_Create(200, 200, 200, 255);
		r->renderPriority = PRI_GAME_OBJECT;
		r->width = 32;
		r->height = 32;

		r->startUV = CP_Vector_Set(0, 0);
		r->endUV = CP_Vector_Set(1, 1);
		r->isXFlipped = 0;
		r->isYFlipped = 0;

		r->text = NULL;
		r->textColor = CP_Color_Create(0, 0, 0, 255);
		r->textLocalPosition = CP_Vector_Set(0.0f, 0.0f);
		r->textScale = CP_Vector_Set(1.0f, 1.0f);
		r->textRotation = 0.0f;
		r->isEnabled = 1;

		r->strokeWeight = 2.0f;
		r->strokeColor = CP_Color_Create(0, 0, 0, 255);
	}

	LL_Add(&renderObjects, r);
	return r;
}
/// <summary>
/// Finds Renderer that contains game object arg
/// </summary>
/// <param name="curr">- current node's data</param>
/// <param name="arg">- gameobject to be compared</param>
/// <returns>- renderer of the game object</returns>
void* FindRenderer(void* curr, void* arg)
{
	Renderer* c = (Renderer*)curr;
	GameObject* go = (GameObject*)arg;
	if (c->go == go)
		return curr;
	return NULL;
}
/// <summary>
/// returns renderer if it exists for the gameobject
/// </summary>
/// <param name="g">- game object to find the renderer on</param>
/// <returns>Renderer* - pointer of renderer found</returns>
Renderer* RM_GetComponent(GameObject* g)
{
	return LL_Find(renderObjects, FindRenderer, g);
}
/// <summary>
/// removes renderer r from manager's list
/// </summary>
/// <param name="r">- renderer to be removed</param>
void RM_RemoveRenderObject(Renderer* r)
{
	LL_RemovePtr(&renderObjects, r);
	free(r);
}
/// <summary>
/// clears and de allocates memory for stored entities
/// </summary>
void RM_Clear()
{
	LinkedList* node = renderObjects;
	for (; node; node = node->next)
	{
		if (((Renderer*)node->curr)->sprite)
			RM_DeleteImage((Renderer*)node->curr);
		//If break around here, make sure the text you set is using RM_SetText and not r->text = ""
		if (((Renderer*)node->curr)->text)
		{
			free(((Renderer*)node->curr)->text);
			((Renderer*)node->curr)->text = NULL;
		}
		free(node->curr);
	}
	node = debugLinesList;
	for (; node; node = node->next)
	{
		free(node->curr);
	}
	
	LL_Clear(&renderObjects);
	LL_Clear(&debugLinesList);
	matrixStack = MS_Clear(matrixStack);
}
/// <summary>
/// Calculates and returns the view matrix according to world space
/// </summary>
/// <returns>CP_Matrix - matrix of view</returns>
CP_Matrix RM_GetViewMatrix()
{
	float w = (float)CP_System_GetWindowWidth();
	float h = (float)CP_System_GetWindowHeight();
	float hworld = WORLD_HEIGHT * zoom; //fit 500 grid in window height
	float hratio = h / hworld;
	RM_SetCameraScale(CP_Vector_Set(hratio, hratio));
	//coz 0,0 is top left.
	CP_Vector camFinalPos = CP_Vector_Subtract(cameraPos, CP_Vector_Set(w * 0.5f / cameraScale.x, h * 0.5f / cameraScale.y));
	CP_Matrix view = CP_Matrix_Multiply(CP_Matrix_Scale(cameraScale), CP_Matrix_Translate(CP_Vector_Negate(camFinalPos)));
	return view;
}
/// <summary>
/// renders all entities
/// </summary>
void RM_Render()
{
	CP_Graphics_ClearBackground(CP_Color_Create(112, 228, 204, 255));

	LinkedList* renderLayers[PRI_TOTAL] = { NULL };
	LinkedList* node = renderObjects;
	for (; node; node = node->next)
	{
		Renderer* r = (Renderer*)node->curr;
		LL_Add(&renderLayers[r->renderPriority] ,r);
	}

	for (int i = 0; i < PRI_TOTAL; ++i)
	{
		if (i != PRI_UI)
		{
			CP_Settings_ApplyMatrix(RM_GetViewMatrix());
			//MS_Translate(matrixStack, CP_Vector_Negate(cameraPos));

			RenderAllInList(renderLayers[i]);
			CP_Settings_ResetMatrix();
		}
		else
		{
			RenderAllInList(renderLayers[i]);
		}

//#if _DEBUG
		//rendered last
		if (i == PRI_GAME_OBJECT)
		{
			node = debugLinesList;
			for (; node; node = node->next)
			{
				DebugLine* line = (DebugLine*)node->curr;
				if (line->space == PRI_GAME_OBJECT)
				{
					CP_Settings_ApplyMatrix(RM_GetViewMatrix());
				}

				if (line->type == 1)
				{
					CP_Settings_Stroke(line->color);
					CP_Settings_Fill(line->color);
					CP_Graphics_DrawCircle(line->from.x, line->from.y, line->to.x);
					//CP_Graphics_DrawLine(line->from.x, line->from.y, line->to.x, line->to.y);
				}
				else
				{
					CP_Settings_Stroke(line->color);
					CP_Graphics_DrawLine(line->from.x, line->from.y, line->to.x, line->to.y);
					CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
				}


				CP_Settings_ResetMatrix();
				free(node->curr);
			}
			LL_Clear(&debugLinesList);
		}
		
//#endif

	}


	for (int i = 0; i < PRI_TOTAL; ++i)
	{
		LL_Clear(&renderLayers[i]);
	}

	
}

/*!
@brief Set camera center point to the position param
@param position - Position for the camera to focus/look at
@return void
*/
void RM_SetCameraPosition(CP_Vector pos)
{
	cameraPos = pos;
}
/*!
@brief Gets camera position
@return CP_Vector - position of camera in world space
*/
CP_Vector RM_GetCameraPosition()
{
	return cameraPos;
}
/*!
@brief Sets camera scale. higher scale means render objects larger
@param s - scale multiplied to the objects rendered
*/
void RM_SetCameraScale(CP_Vector s)
{
	cameraScale = s;
}
/*!
@brief Gets camera scale value
@return CP_Vector - scale of image in camera in world space
*/
CP_Vector RM_GetCameraScale()
{
	return cameraScale;
}
/*!
@brief Loads and attach image to renderer from file path
@param r - Renderer for image to be added to
@param filepath - filepath of the image
@return ptr of the renderer attached
*/
Renderer* RM_LoadImage(Renderer* r , const char* filepath)
{
	r->sprite = CP_Image_Load(filepath);
	r->width = CP_Image_GetWidth(r->sprite);
	r->height = CP_Image_GetHeight(r->sprite);
	return r;
}
/*!
@brief attach image to renderer
@param r - Renderer for image to be added to
@param img - image to be attached to renderer
@return ptr of the renderer attached
*/
Renderer* RM_AddImage(Renderer* r, CP_Image img)
{
	r->sprite = img;
	r->width = CP_Image_GetWidth(img);
	r->height = CP_Image_GetHeight(img);
	return r;
}
/*!
@brief deletes image from renderer
@param r - Renderer for image to be deleted
@return ptr of the renderer
*/
Renderer* RM_DeleteImage(Renderer* r)
{
	if (r->sprite)
		CP_Image_Free(&r->sprite);
	return r;
}
/*!
@brief assigns the width and height of screen/world size
@param width - ptr for width value to be assigned
@param height - ptr for height value to be assigned
@param space - which space of the width and height
@return void
*/
void RM_GetRenderSize(float* width, float* height, RENDER_PRIORITY space)
{
	float w = (float)CP_System_GetWindowWidth();
	float h = (float)CP_System_GetWindowHeight();

	switch (space)
	{
	case PRI_GAME_OBJECT:
		*height = WORLD_HEIGHT * zoom;
		*width = w / h * *height;
		break;
	default:
		*height = h;
		*width = w;
		break;
	}
}
/*!
@brief Returns mouse position on world space
@param x - x pos of the mouse in screen pixel space
@param y - y pos of the mouse in screen pixel space
@return CP_Vector position of the mouse in worldspace
*/
CP_Vector RM_MousePositionToWorldSpace(float x, float y)
{
	float mposx = CP_Input_GetMouseX();
	float mposy = CP_Input_GetMouseY();
	float w, h;
	RM_GetRenderSize(&w, &h, PRI_UI);
	float ww, wh;
	RM_GetRenderSize(&ww, &wh, PRI_GAME_OBJECT);
	CP_Vector campos = RM_GetCameraPosition();
	CP_Vector ret = CP_Vector_Set(ww / w * mposx - ww * 0.5f + campos.x,
		wh / h * mposy - wh * 0.5f + campos.y);
	return ret;
}
/*!
@brief Zooms in the camera. Renders objects larger
@param z - zoom by value. 2.0 zoom means can see half of original screen
@return void
*/
void RM_SetCameraZoom(float z)
{
	zoom = z;
}
/*!
@brief Set text to r->text
@param r - renderer for text to be set
@param text - null terminated text to be set
@return void
*/
void RM_SetText(Renderer* r, const char* text)
{
	if (r == NULL)
		return;

	if (r->text)
	{
		free(r->text);
		r->text = NULL;
	}

	size_t count = 0;
	for (char* c = text; (*c) != '\0'; c++, count++);


	r->text = calloc(count + 1, sizeof(char));
	if (r->text)
		strcpy_s(r->text, count + 1, text);
}
/*!
@brief draw line from position to position in space. it will delete itself after drawn in frame
@param from - start position of line
@param to - end position of line
@param space - space of line world or screen
@param color - color of line
@return void
*/
void RM_DebugDrawLine(CP_Vector from, CP_Vector to, RENDER_PRIORITY space, CP_Color color, int type)
{
	DebugLine* line = malloc(sizeof(DebugLine));
	if (line)
	{
		line->from = from;
		line->to = to;
		line->space = space;
		line->color = color;
		line->type = type;
	}
	LL_Add(&debugLinesList, line);
}
/// <summary>
/// Renders all objects in this list
/// </summary>
/// <param name="list">- list of objects to be rendered</param>
void RenderAllInList(LinkedList* list)
{
	LinkedList* currNode = list;
	for (; currNode != NULL; currNode = currNode->next)
	{
		Renderer* r = (Renderer*)currNode->curr;
		GameObject* go = (GameObject*)r->go;
		if (!go->isEnabled || !r->isEnabled)
			continue;
		//if (r->renderPriority != type)
		//	continue;

		CP_Settings_Fill(r->color);

		MS_PushMatrix(matrixStack); 

		MS_Translate(matrixStack, go->position);
		MS_Rotate(matrixStack, -go->rotation);//coz if camera tilt left, image will tilt right. but i want image to tilt left
		//MS_Scale(matrixStack, go->scale);
		CP_Settings_ApplyMatrix(*MS_Top(matrixStack));

		CP_Settings_Stroke(r->strokeColor);
		CP_Settings_StrokeWeight(r->strokeWeight);
		
		if (r->sprite)
		{
			float s0 = r->startUV.x * (float)r->width;
			float t0 = r->startUV.y * (float)r->height;
			float s1 = r->endUV.x * (float)r->width;
			float t1 = r->endUV.y * (float)r->height;
			if (r->isXFlipped)
			{
				float temp = s0;
				s0 = s1;
				s1 = temp;
			}
			if (r->isYFlipped)
			{
				float temp = t0;
				t0 = t1;
				t1 = temp;
			}


			CP_Image_DrawSubImage(r->sprite, 0, 0, go->scale.x, go->scale.y,
				s0, t0,
				s1, t1,
				r->color.a);
		}
		else
		{
			//draw primitive shapes
			switch (go->type)
			{
			case CIRCLE:
				//MS_Scale(matrixStack, CP_Vector_Set(10, 1));
				CP_Graphics_DrawCircle(0, 0, go->scale.x);
				break;
			case RECTANGLE:
				//rectangle is drawn w ref to topleft. i wan it centered
				//somehow cannot scale drawrect
				CP_Graphics_DrawRect(-go->scale.x * 0.5f, -go->scale.y * 0.5f,
					go->scale.x, go->scale.y);
				break;
			case FLOOR:
			case WATER:
			case WALL:
				CP_Graphics_DrawRect(-go->scale.x * 0.5f, -go->scale.y * 0.5f,
					go->scale.x, go->scale.y);
				break;
			case LINE:
				CP_Settings_Stroke(r->color);
				CP_Graphics_DrawLine(0, 0, go->scale.x, 0);
				CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
				break;
			default:
				break;
			}
		}
		

		CP_Settings_ApplyMatrix(CP_Matrix_Inverse(*MS_Top(matrixStack)));
		MS_PopMatrix(matrixStack);

		if (r->text)
		{
			CP_Settings_Fill(r->textColor);

			MS_PushMatrix(matrixStack);
			MS_Translate(matrixStack, go->position);
			MS_Translate(matrixStack, r->textLocalPosition);
			MS_Rotate(matrixStack, -r->textRotation);
			MS_Scale(matrixStack, r->textScale);
			CP_Settings_ApplyMatrix(*MS_Top(matrixStack));
			
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

			CP_Font_DrawText(r->text, 0.0f, 0.0f);

			CP_Settings_ApplyMatrix(CP_Matrix_Inverse(*MS_Top(matrixStack)));
			MS_PopMatrix(matrixStack);
		}

	}
}

