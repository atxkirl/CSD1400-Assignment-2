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
	LinkedList* currNode = renderObjects;
	for (;currNode != NULL; currNode = currNode->next)
	{
		GameObject* go = (GameObject*)currNode->curr;
		if (!go->isEnabled)
			continue;

		CP_Settings_Fill(go->color);
;		switch (go->type)
		{
		case CIRCLE:
			CP_Graphics_DrawCircle(go->position.x, go->position.y, go->scale.x);
			break;
		case RECTANGLE:
			//rectangle is drawn w ref to topleft
			CP_Graphics_DrawRect(go->position.x - go->scale.x * 0.5f, go->position.y - go->scale.y * 0.5f, go->scale.x, go->scale.y);
			break;
		default:
			break;
		}
	}
}