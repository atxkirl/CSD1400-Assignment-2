#include "SystemManager.h"


void SM_SystemsInit()
{
	GOM_Init();
	RM_Init();
	CLM_Init();
}

void SM_SystemsPreUpdate()
{
}

void SM_SystemsUpdate()
{
	CLM_Update();
}

void SM_SystemsLateUpdate()
{
	LinkedList* arr = GOM_GetTempObjects();
	for (; arr; arr = arr->next)
	{
		SM_DeleteFromAllSystems((GameObject*)(arr->curr));
	}
	GOM_ClearTempObjects();
	RM_Render();
}

void SM_SystemsExit()
{
	CLM_Clear();
	RM_Clear();
	GOM_Clear();
}

void* SM_GetComponent(GameObject* g, COMPONENT c)
{
	void* ret = NULL;
	switch (c)
	{
	case COM_RENDERER:
		ret = RM_GetComponent(g);
		break;
	case COM_COLLISION:
		ret = CLM_GetComponent(g);
		break;
	}
	return ret;
}

void SM_DeleteFromAllSystems(GameObject* g)
{
	for (int i = 0; i < COM_COUNT; i++)
	{
		void* c = SM_GetComponent(g, i);
		if (c)
		{
			switch (i)
			{
			case COM_RENDERER:
				RM_RemoveRenderObject((Renderer*)c);
				break;
			case COM_COLLISION:
				CLM_Remove((Collider*)c);
				break;
			}
		}
	}
}

//void* SM_AddComponent(GameObject* g, COMPONENT c)
//{
//	void* ret = NULL;
//	switch (c)
//	{
//	case COM_RENDERER:
//		ret = RM_AddComponent(g);
//		break;
//	case COM_COLLISION:
//		ret = CLM_AddComponent(g);
//		break;
//	}
//	return ret;
//}
