#include "RenderManager.h"

void RM_Init() 
{

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
}
void RM_Render()
{
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 255, 255));
	int size = LL_GetCount(renderObjects);
	LinkedList* currNode = renderObjects;
	for (;currNode != NULL; currNode = currNode->next)
	{
		GameObject* go = (GameObject*)currNode->curr;
		if (!go->isEnabled)
			continue;
;		switch (go->type)
		{
		case CIRCLE:
			CP_Graphics_DrawCircle(go->position.x, go->position.y, go->scale.x);
			break;
		default:
			break;
		}
	}
}