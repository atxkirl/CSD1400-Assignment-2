#include "RenderManager.h"
#include "GameObjectManager.h"
#include "MatrixStack.h"

#include <stdio.h>
#include <stdlib.h>

LinkedList* renderObjects = NULL;
LinkedList* matrixStack = NULL;

CP_Vector cameraPos;
CP_Vector cameraScale;

//private functions
void RenderAllOfType(RENDER_PRIORITY type);

void RM_Init() 
{
	matrixStack = MS_InitIdentity();
	//MS_Print(MS_Top(matrixStack));
	//MS_Print(matrixStack->curr);
	cameraPos = CP_Vector_Set(0.0f, 0.0f);
	cameraScale = CP_Vector_Set(1.0f, 1.0f);
}
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

		r->text = NULL;
		r->textColor = CP_Color_Create(0, 0, 0, 255);
		r->textLocalPosition = CP_Vector_Set(0.0f, 0.0f);
		r->textScale = CP_Vector_Set(1.0f, 1.0f);
		r->textRotation = 0.0f;
	}

	LL_Add(&renderObjects, r);
	return r;
}
void* FindRenderer(void* curr, void* arg)
{
	Renderer* c = (Renderer*)curr;
	GameObject* go = (GameObject*)arg;
	if (c->go == go)
		return curr;
	return NULL;
}
Renderer* RM_GetComponent(GameObject* g)
{
	return LL_Find(renderObjects, FindRenderer, g);
}
void RM_RemoveRenderObject(Renderer* r)
{
	LL_RemovePtr(&renderObjects, r);
	free(r);
}
void RM_Clear()
{
	for (; renderObjects; renderObjects = renderObjects->next)
	{
		if (((Renderer*)renderObjects->curr)->sprite)
			RM_DeleteImage((Renderer*)renderObjects->curr);
		free(renderObjects->curr);
	}
	LL_Clear(&renderObjects);
	matrixStack = MS_Clear(matrixStack);
}
void RM_Render()
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 255, 255));
	
	float w = (float)CP_System_GetWindowWidth();
	float h = (float)CP_System_GetWindowHeight();
	//printf("%.0f %.0f\n", w, h);
	float hworld = WORLD_HEIGHT; //fit 500 grid in window height
	float hratio = h / hworld;
	RM_SetCameraScale(CP_Vector_Set(hratio, hratio));
	//CP_Vector sc = RM_GetCameraScale();
	//printf("%f %f\n", sc.x, sc.y);
	
	//coz 0,0 is top left.
	CP_Vector camFinalPos = CP_Vector_Subtract(cameraPos, CP_Vector_Set(w * 0.5f / cameraScale.x, h * 0.5f / cameraScale.y));
	CP_Matrix view = CP_Matrix_Multiply(CP_Matrix_Scale(cameraScale) , CP_Matrix_Translate(CP_Vector_Negate(camFinalPos)));
	CP_Settings_ApplyMatrix(view);
	//MS_Translate(matrixStack, CP_Vector_Negate(cameraPos));

	RenderAllOfType(PRI_GAME_OBJECT);
	CP_Settings_ResetMatrix();

	//CP_Vector screenScale = CP_Vector_Set(1,1);
	//hworld = SCREEN_HEIGHT;
	//hratio = h / hworld;
	//whratio = w / (float)h;
	//wscale = whratio * hworld;
	//screenScale = CP_Vector_Set(w / wscale, hratio);
	//CP_Settings_ApplyMatrix(CP_Matrix_Scale(screenScale));

	RenderAllOfType(PRI_UI);
	//CP_Settings_ResetMatrix();
}

void RM_SetCameraPosition(CP_Vector pos)
{
	cameraPos = pos;
}

CP_Vector RM_GetCameraPosition()
{
	return cameraPos;
}

void RM_SetCameraScale(CP_Vector s)
{
	cameraScale = s;
}

CP_Vector RM_GetCameraScale()
{
	return cameraScale;
}

Renderer* RM_LoadImage(Renderer* r , const char* filepath)
{
	r->sprite = CP_Image_Load(filepath);
	r->width = CP_Image_GetWidth(r->sprite);
	r->height = CP_Image_GetHeight(r->sprite);
	return r;
}

Renderer* RM_AddImage(Renderer* r, CP_Image img)
{
	r->sprite = img;
	r->width = CP_Image_GetWidth(img);
	r->height = CP_Image_GetHeight(img);
	return r;
}

Renderer* RM_DeleteImage(Renderer* r)
{
	if (r->sprite)
		CP_Image_Free(&r->sprite);
	return r;
}

void RM_GetRenderSize(float* width, float* height, RENDER_PRIORITY space)
{
	float w = (float)CP_System_GetWindowWidth();
	float h = (float)CP_System_GetWindowHeight();

	switch (space)
	{
	case PRI_GAME_OBJECT:
		*height = WORLD_HEIGHT;
		*width = w / h * *height;
		break;
	default:
		*height = h;
		*width = w;
		break;
	}
}

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

void RenderAllOfType(RENDER_PRIORITY type)
{
	LinkedList* currNode = renderObjects;
	for (; currNode != NULL; currNode = currNode->next)
	{
		Renderer* r = (Renderer*)currNode->curr;
		GameObject* go = (GameObject*)r->go;
		if (!go->isEnabled)
			continue;
		if (r->renderPriority != type)
			continue;

		CP_Settings_Fill(r->color);

		MS_PushMatrix(matrixStack); 

		MS_Translate(matrixStack, go->position);
		MS_Rotate(matrixStack, -go->rotation);//coz if camera tilt left, image will tilt right. but i want image to tilt left
		//MS_Scale(matrixStack, go->scale);
		CP_Settings_ApplyMatrix(*MS_Top(matrixStack));
		
		if (r->sprite)
		{
			CP_Image_DrawSubImage(r->sprite, 0, 0, go->scale.x, go->scale.y,
				r->startUV.x * (float)r->width, r->startUV.y * (float)r->height,
				r->endUV.x * (float)r->width, r->endUV.y * (float)r->height,
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

