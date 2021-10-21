#include "RenderManager.h"
#include "GameObjectManager.h"
#include "MatrixStack.h"

#include <stdio.h>

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
void RM_AddRenderObject(GameObject* g)
{
	renderObjects = LL_Add(renderObjects, g);
}
void RM_RemoveRenderObject(GameObject* g)
{
	renderObjects = LL_RemovePtr(renderObjects, g);
}
void RM_ClearRenderObjects()
{
	renderObjects = LL_Clear(renderObjects);
	matrixStack = LL_Clear(matrixStack);
}
void RM_Render()
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 255, 255));
	
	RenderAllOfType(PRI_GAME_OBJECT);
	CP_Settings_ResetMatrix();
	RenderAllOfType(PRI_UI);
	CP_Settings_ResetMatrix();
}

void RM_SetCameraPosition(CP_Vector pos)
{
	cameraPos = pos;
}

void RM_SetCameraScale(CP_Vector s)
{
	cameraScale = s;
}

CP_Vector RM_GetCameraScale()
{
	return cameraScale;
}

void RenderAllOfType(RENDER_PRIORITY type)
{
	LinkedList* currNode = renderObjects;
	for (; currNode != NULL; currNode = currNode->next)
	{
		GameObject* go = (GameObject*)currNode->curr;
		if (!go->isEnabled)
			continue;
		if (go->renderPriority != type)
			continue;

		CP_Settings_Fill(go->color);

		MS_PushMatrix(matrixStack); 
		if (type == PRI_GAME_OBJECT)
		{
			CP_Settings_ApplyMatrix(CP_Matrix_Scale(cameraScale));
			MS_Translate(matrixStack, CP_Vector_Negate(cameraPos));
		}
		MS_Translate(matrixStack, go->position);
		MS_Rotate(matrixStack, -go->rotation);//coz if camera tilt left, image will tilt right. but i want image to tilt left
		//MS_Scale(matrixStack, go->scale);
		CP_Settings_ApplyMatrix(*MS_Top(matrixStack));
		//RM_ApplyMatrix(MS_Top(matrixStack));
		
		switch (go->type)
		{
		case CIRCLE:
			//MS_Scale(matrixStack, CP_Vector_Set(10, 1));
			CP_Graphics_DrawCircle(0,0, go->scale.x);
			break;
		case RECTANGLE:
			//rectangle is drawn w ref to topleft. i wan it centered
			//somehow cannot scale drawrect

			CP_Graphics_DrawRect(-go->scale.x * 0.5f, -go->scale.y * 0.5f, go->scale.x, go->scale.y);
			break;
		case WALL:
				CP_Graphics_DrawRect(-go->scale.x * 0.5f, -go->scale.y * 0.5f, go->scale.x, go->scale.y);
				break;
		default:
			break;
		}
		MS_PopMatrix(matrixStack);
		CP_Settings_ResetMatrix();

		if (go->text)
		{
			CP_Settings_Fill(go->textColor);

			MS_PushMatrix(matrixStack);
			MS_Translate(matrixStack, go->position);
			MS_Translate(matrixStack, go->textLocalPosition);
			MS_Rotate(matrixStack, -go->textRotation);
			MS_Scale(matrixStack, go->textScale);
			CP_Settings_ApplyMatrix(*MS_Top(matrixStack));
			
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

			CP_Font_DrawText(go->text, 0.0f, 0.0f);
			MS_PopMatrix(matrixStack);
			CP_Settings_ResetMatrix();
		}

	}
}

