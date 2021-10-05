#include "GameObjectManager.h"
#include <stdlib.h>

void GOM_Init()
{

}

int GOM_Delete(GameObject* g)
{
	objects = LL_RemovePtr(objects, g);
	free(g);

	return 1;
}
void GOM_Clear()
{
	LinkedList* l = objects;
	while (l)
	{
		free(l->curr);
		l = l->next;
	}
	LL_Clear(objects);
}
int GOM_GetIndex(GameObject* go)
{
	return LL_GetIndexPtr(objects, go);
}

GameObject* GOM_CreateGameObject()
{
	GameObject* go = GameObject_new();
	objects = LL_Add(objects, go);
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
