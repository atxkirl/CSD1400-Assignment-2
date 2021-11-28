/*!
@file		GameObjectManager.h
@author		Ow Hong Yu (ow.h)
@course		CSD 1400
@section	A
@brief		Game Object Manager that manages the lifespan of a game object. It allocates memory for gameobjects
			and deallocates them when they are not used
*/

#pragma once

//Controls lifetime of gameobjects
//PREFIX: GOM

#include "GameObject.h"
#include "LinkedList.h"


/*!
@brief Initialises the GameObjectManager
@return void
*/
void GOM_Init();

/*!
@brief Creates a GameObject and return the pointer to it
This function takes as input a type and priority.
Gameobject is automatically stored in container for better control of lifespan.
Special scenarios: -
@param type - the type of the object is used on how it is rendered
@return pointer to gameobject created
*/
GameObject* GOM_Create(OBJECT_TYPE type);
/*!
@brief Creates a GameObject that lasts for one frame
@param type - the type of the object is used on how it is rendered
@return pointer to gameobject created
*/
GameObject* GOM_CreateTemp(OBJECT_TYPE type);
/*!
@brief Creates a GameObject and return the pointer to it
@param type - type of the object
@param pos - position of the object
@param rot - rotation of the object
@param scale - scale of the object
@return pointer to gameobject created
*/
GameObject* GOM_Create2(OBJECT_TYPE type, CP_Vector pos, float rot, CP_Vector scale);
/*!
@brief Creates a GameObject and return the pointer to it
@param pos - position of the object
@param rot - rotation of the object
@param scale - scale of the object
@return pointer to gameobject created
*/
GameObject* GOM_Create3(CP_Vector pos, float rot, CP_Vector scale);

/*!
@brief Deletes the gameobject
@param go - ptr of the gameobject to be deleted
@return int 1 if successful else 0
*/
int GOM_Delete(GameObject*);
/*!
@brief Clear and free memory of all the gameobjects
@return void
*/
void GOM_Clear();
/*!
@brief Returns the index of the gameobject in the objectList
@param go - ptr to the gameobject
@return int index of the gameobject
*/
int GOM_GetIndex(GameObject*);

///*!
//@brief Clears all the temp objects for the frame
//@return void
//*/
//void GOM_ClearTempObjects();

/*!
@brief Get the number of objects
@return int number of objects
*/
int GOM_GetCount();

///*!
//@brief Get list of temp objects
//@return LinkedList* of temp objects
//*/
//LinkedList* GOM_GetTempObjects();

/// <summary>
/// Factory pattern for game object creation <unused>
/// </summary>
/// <param name="type">- enum type of game object</param>
/// <returns>- pointer to newly created game object</returns>
GameObject* GOM_FactoryCreateGO(int type);//int according to OBJECT_TYPE
