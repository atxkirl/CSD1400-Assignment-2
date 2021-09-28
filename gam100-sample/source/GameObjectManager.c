#include "GameObjectManager.h"
#include <stdlib.h>

void GOM_Init()
{
	objectCount = 0;
	memset(objects, 0, 0);
}
GameObject* GOM_CreateGameObject()
{
	GameObject* go = GameObject_new();
	objectCount++;
	memset(objects, sizeof(GameObject*), objectCount);
	objects[objectCount - 1] = go;
	return go;
}
int GOM_Delete(GameObject* g)
{
	int i = GOM_GetIndex(g);
	if (i < 0)
		return i;

	for (int j = i; j < objectCount - 1; ++j)
	{
		objects[i] = objects[i + 1];
	}
	objects[objectCount - 1] = NULL;
	memset(objects, sizeof(GameObject*), objectCount - 1);
	free(g);

	return 1;
}
void GOM_Clear()
{
	for (int i = 0; i < objectCount; ++i)
	{
		free(objects[i]);
	}
	objectCount = 0;
	memset(objects, 0, 0);
}
int GOM_GetIndex(GameObject* go)
{
	for (int i = 0; i < objectCount; ++i)
	{
		if (objects[i] == go)
			return i;
	}
	return -1;
}
