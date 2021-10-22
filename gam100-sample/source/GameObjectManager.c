#include "GameObjectManager.h"
#include "RenderManager.h"
#include <stdlib.h>

LinkedList* GOM_objects;

void GOM_Init()
{

}

int GOM_Delete(GameObject* g)
{
	GOM_objects = LL_RemovePtr(GOM_objects, g);
	free(g);

	return 1;
}
void GOM_Clear()
{
	LinkedList* l = GOM_objects;
	while (l)
	{
		free(l->curr);
		l = l->next;
	}
	GOM_objects = LL_Clear(GOM_objects);
}
int GOM_GetIndex(GameObject* go)
{
	return LL_GetIndexPtr(GOM_objects, go);
}

GameObject* GOM_CreateGameObject(OBJECT_TYPE type, RENDER_PRIORITY priority)
{
	GameObject* go = malloc(sizeof(GameObject));

	if (go)
	{
		go->position = CP_Vector_Set(0.0f, 0.0f);
		go->rotation = 0.0f;
		go->scale = CP_Vector_Set(1.0f, 1.0f);
		go->type = type;
		go->tag = "";
		go->renderPriority = priority;
		go->isEnabled = 1;
		go->color = CP_Color_Create(0, 0, 0, 255);

		go->text = NULL;
		go->textColor = CP_Color_Create(0, 0, 0, 255);
		go->textLocalPosition = CP_Vector_Set(0.0f, 0.0f);
		go->textRotation = 0.0f;
		go->textScale = CP_Vector_Set(1.0f, 1.0f);
	}

	GOM_objects = LL_Add(GOM_objects, go);
	RM_AddRenderObject(go);
	return go;
}

GameObject* GOM_FactoryCreateGO(int type)
{
	enum OBJECT_TYPE objType = (enum OBJECT_TYPE)type;

	GameObject* go = GOM_CreateGameObject(objType, PRI_GAME_OBJECT);
	go->type = objType;
	//switch (objType)
	//{
	//case CIRCLE:
	//{
	//}
	//	break;
	//case RECTANGLE:

	//	break;

	//}
	return go;
}
