#include "RenderManager.h"

void RM_Init() 
{
	renderObjectCount = 0;
	memset(renderObjects, 0, 0);
}
void RM_AddRenderObject(GameObject* g)
{
	renderObjectCount++;
	memset(renderObjects, sizeof(GameObject*), renderObjectCount);
	renderObjects[renderObjectCount - 1] = g;
}
void RM_RemoveRenderObject(GameObject* g)
{
	renderObjectCount--;
	renderObjects[renderObjectCount - 1] = NULL;
	memset(renderObjects, sizeof(GameObject*), renderObjectCount);
}
void RM_ClearRenderObjects()
{
	renderObjectCount = 0;
	memset(renderObjects, 0, 0);
}
void RM_Render()
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 255, 255));
	for (int i = 0; i < renderObjectCount; ++i)
	{
		GameObject* go = renderObjects[i];
		switch (go->type)
		{
		case CIRCLE:
			CP_Graphics_DrawCircle(go->position.x, go->position.y, go->scale.x);
			break;
		default:
			break;
		}
	}
}