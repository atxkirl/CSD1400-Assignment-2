/*!
@file		SystemManager.c
@author		Ow Hong Yu (ow.h)
@course		CSD 1400
@section	A
@brief		System manager that controls all the systems and their order of function calls.
			It makes it easier for scenes to call updates, inits and clears all systems.
*//*______________________________________________________________________*/

#include "SystemManager.h"
#include "LinkedList.h"
#include "Controls.h"

LinkedList* SM_delList = NULL;
LinkedList* SM_timedDel = NULL;

/*!
@brief Delete gameobject from all systems
@param g - gameobject clean delete
@return void
*/
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
			case COM_ANIMATION:
				AM_Remove((Animation*)c);
				break;
			}
		}
	}
}
/*!
@brief Inits all systems
@return void
*/
void SM_SystemsInit()
{
	SM_isPaused = false;

	GOM_Init();
	RM_Init();
	CLM_Init();
	AM_Init();
	DM_Init();
	AIM_Init();
	Init_Controls();
}
/*!
@brief Update systems at the start of the frame
@return void
*/
void SM_SystemsPreUpdate()
{
	//physics if there is
	GameObject* clickPoint = NULL;
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		//Creates a point obj to test collision against button
		clickPoint = GOM_CreateTemp(EMPTY);//param doesnt matter
		clickPoint->position = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		clickPoint->tag = ONCLICK_TAG;
		Collider* c = CLM_AddComponent(clickPoint);
		CLM_Set(c, COL_POINT, NULL);
		c->space = COLSPC_SCREEN;
	}

	//triggers deletion for timeddeletes
	LinkedList* node = SM_timedDel;
	LinkedList* clr = NULL;
	for (; node; node = node->next)
	{
		DeleteAfter* del = (DeleteAfter*)node->curr;
		del->et += CP_System_GetDt();
		if (del->et > del->delAfter)
		{
			SM_DeleteGameObject(del->go);
			LL_Add(&clr, del);
		}
	}

	node = clr;
	for (; node; node = node->next)
	{
		LL_RemovePtr(&SM_timedDel, node->curr);
		free(node->curr);
	}
	LL_Clear(&clr);
}
/*!
@brief Update systems
@param isPause - boolean for if pause or not
@return void
*/
void SM_SystemsUpdate(int isPause)
{
	SM_isPaused = isPause;

	DM_Update();
	CLM_Update();
	AM_Update();
	if(!isPause)
		AIM_Update();
}
/*!
@brief Update systems at the end of the frame
@return void
*/
void SM_SystemsLateUpdate()
{
	LinkedList* arr = SM_delList;
	for (; arr; arr = arr->next)
	{
		SM_DeleteFromAllSystems((GameObject*)(arr->curr));
	}
	LL_Clear(&SM_delList);

	RM_Render();
	DM_LateUpdate();
}
/*!
@brief Exit and clean up systems
@return void
*/
void SM_SystemsExit()
{
	AM_Clear();
	CLM_Clear();
	RM_Clear();
	GOM_Clear();
	DM_Clear();
	AIM_Clear();
	Exit_Controls();
}
/*!
@brief Getter for SM pause state.
@return bool Pause state of SystemManager.
*/
bool SM_IsPaused()
{
	return SM_isPaused;
}
/*!
@brief Find component that is attached to gameobject
@param g - gameobject to find
@param c - component id to find for
@return ptr of component
*/
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
	case COM_ANIMATION:
		ret = AM_GetComponent(g);
		break;
	}
	return ret;
}
/*!
@brief Delete gameobject from all systems AFTER Update()
@param g - gameobject clean delete
@return void
*/
void SM_DeleteGameObject(GameObject* g)
{
	LL_Add(&SM_delList, g);
}
/*!
@brief Delete gameobject from all systems AFTER time in seconds
@param g - gameobject clean delete
@param t - to delete after this time has passed
@return void
*/
void SM_DeleteGameObjectAfter(GameObject* g, float t)
{
	DeleteAfter* del = malloc(sizeof(DeleteAfter));
	if (del)
	{
		del->go = g;
		del->et = 0.0f;
		del->delAfter = t;
	}

	LL_Add(&SM_timedDel, del);
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
