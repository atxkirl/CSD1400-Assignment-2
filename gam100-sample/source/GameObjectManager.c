#include "GameObjectManager.h"
#include "RenderManager.h"
#include <stdlib.h>
#include "CollisionManager.h"

LinkedList* objectList = NULL;
LinkedList* tempList = NULL;

void GOM_Init()
{

}

int GOM_Delete(GameObject* g)
{
	objectList = LL_RemovePtr(objectList, g);
	free(g);

	return 1;
}
void GOM_Clear()
{
	LinkedList* l = objectList;
	while (l)
	{
		free(l->curr);
		l = l->next;
	}
	objectList = LL_Clear(objectList);
	tempList = LL_Clear(tempList);
}
int GOM_GetIndex(GameObject* go)
{
	return LL_GetIndexPtr(objectList, go);
}

void GOM_ClearTempObjects()
{
	LinkedList* l = tempList;
	while (l)
	{
		objectList = LL_RemovePtr(objectList, l->curr);
		free(l->curr);
		l = l->next;
	}
	tempList = LL_Clear(tempList);
}

GameObject* GOM_Create(OBJECT_TYPE type)
{
	GameObject* go = malloc(sizeof(GameObject));

	if (go)
	{
		go->position = CP_Vector_Set(0.0f, 0.0f);
		go->rotation = 0.0f;
		go->scale = CP_Vector_Set(1.0f, 1.0f);
		go->type = type;
		go->tag = "";
		go->isEnabled = 1;
		//go->renderPriority = priority;
		//go->color = CP_Color_Create(0, 0, 0, 255);

		//go->text = NULL;
		//go->textColor = CP_Color_Create(0, 0, 0, 255);
		//go->textLocalPosition = CP_Vector_Set(0.0f, 0.0f);
		//go->textRotation = 0.0f;
		//go->textScale = CP_Vector_Set(1.0f, 1.0f);
	}

	objectList = LL_Add(objectList, go);
	//RM_AddComponent(go);
	return go;
}

GameObject* GOM_CreateTemp(OBJECT_TYPE type)
{
	GameObject* go = GOM_Create(type);
	tempList = LL_Add(tempList, go);
	return go;
}

GameObject* GOM_Create2(OBJECT_TYPE type, CP_Vector pos, float rot, CP_Vector scale)
{
	GameObject* go = GOM_Create(type);
	go->position = pos;
	go->rotation = rot;
	go->scale = scale;
	return go;
}
GameObject* GOM_Create3(CP_Vector pos, float rot, CP_Vector scale)
{
	return GOM_Create2(RECTANGLE, pos, rot, scale);
}

int GOM_GetCount()
{
	return LL_GetCount(objectList);
}

GameObject* GOM_FactoryCreateGO(int type)
{
	enum OBJECT_TYPE objType = (enum OBJECT_TYPE)type;

	GameObject* go = GOM_Create(objType);
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
