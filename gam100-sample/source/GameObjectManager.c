/*!
@file		GameObjectManager.c
@author		Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team       BananaBoi
@brief		Game Object Manager that manages the lifespan of a game object. It allocates memory for gameobjects
			and deallocates them when they are not used
*/

#include "GameObjectManager.h"
#include <stdlib.h>
#include "SystemManager.h"

LinkedList* objectList = NULL;
//LinkedList* tempList = NULL;

/*!
@brief Initialises the GameObjectManager
@return void
*/
void GOM_Init()
{

}

/*!
@brief Deletes the gameobject
@param go - ptr of the gameobject to be deleted
@return int 1 if successful else 0
*/
int GOM_Delete(GameObject* g)
{
	LL_RemovePtr(&objectList, g);
	free(g);

	return 1;
}
/*!
@brief Clear and free memory of all the gameobjects
@return void
*/
void GOM_Clear()
{
	LinkedList* l = objectList;
	while (l)
	{
		free(l->curr);
		l = l->next;
	}
	LL_Clear(&objectList);
	//LL_Clear(&tempList);
}
/*!
@brief Returns the index of the gameobject in the objectList
@param go - ptr to the gameobject
@return int index of the gameobject
*/
int GOM_GetIndex(GameObject* go)
{
	return LL_GetIndexPtr(objectList, go);
}

//void GOM_ClearTempObjects()
//{
//	LinkedList* l = tempList;
//	while (l)
//	{
//		LL_RemovePtr(&objectList, l->curr);
//		free(l->curr);
//		l = l->next;
//	}
//	LL_Clear(&tempList);
//}

/*!
@brief Creates a GameObject and return the pointer to it
This function takes as input a type and priority.
Gameobject is automatically stored in container for better control of lifespan.
Special scenarios: -
@param type - the type of the object is used on how it is rendered
@return pointer to gameobject created
*/
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

	LL_Add(&objectList, go);
	//RM_AddComponent(go);
	return go;
}

/*!
@brief Creates a GameObject that lasts for one frame
@param type - the type of the object is used on how it is rendered
@return pointer to gameobject created
*/
GameObject* GOM_CreateTemp(OBJECT_TYPE type)
{
	GameObject* go = GOM_Create(type);
	//LL_Add(&tempList, go);
	SM_DeleteGameObject(go);
	return go;
}

/*!
@brief Creates a GameObject and return the pointer to it
@param type - type of the object
@param pos - position of the object
@param rot - rotation of the object
@param scale - scale of the object
@return pointer to gameobject created
*/
GameObject* GOM_Create2(OBJECT_TYPE type, CP_Vector pos, float rot, CP_Vector scale)
{
	GameObject* go = GOM_Create(type);
	go->position = pos;
	go->rotation = rot;
	go->scale = scale;
	return go;
}
/*!
@brief Creates a GameObject and return the pointer to it
@param pos - position of the object
@param rot - rotation of the object
@param scale - scale of the object
@return pointer to gameobject created
*/
GameObject* GOM_Create3(CP_Vector pos, float rot, CP_Vector scale)
{
	return GOM_Create2(RECTANGLE, pos, rot, scale);
}
/*!
@brief Get the number of objects
@return int number of objects
*/
int GOM_GetCount()
{
	return LL_GetCount(objectList);
}

//LinkedList* GOM_GetTempObjects()
//{
//	return tempList;
//}

/// <summary>
/// Factory pattern for game object creation <unused>
/// </summary>
/// <param name="type">- enum type of game object</param>
/// <returns>- pointer to newly created game object</returns>
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
