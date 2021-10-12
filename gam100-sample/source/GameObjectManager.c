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

GameObject* GOM_CreateGameObject()
{
	GameObject* go = GameObject_new();
	GOM_objects = LL_Add(GOM_objects, go);
	RM_AddRenderObject(go);
	return go;
}

GameObject* GOM_FactoryCreateGO(int type)
{
	enum OBJECT_TYPE objType = (enum OBJECT_TYPE)type;

	GameObject* go = GOM_CreateGameObject();
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
