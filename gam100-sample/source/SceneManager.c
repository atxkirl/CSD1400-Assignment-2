/*
@file	SceneManager.c
@author	Adrian Tan (t.xingkhiangadrian)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@date	06/10/2021
@brief	Contains functions to register and change scenes during runtime.
*//*______________________________________________________________________*/

#include "SceneManager.h"
#include <stdio.h>
#include <stdlib.h>

Scene* currentScene;	// Pointer to the current scene.
LinkedList* sceneList;	// Linked List holding all registered scene structs.

/// <summary>
/// Creates a Scene struct to hold data on a single scene to allow for easy scene changing.
/// </summary>
/// <param name="name">Human readable name of the scene.</param>
/// <param name="init">Function pointer to the scene's init function.</param>
/// <param name="update">Function pointer to the scene's update function.</param>
/// <param name="exit">Function pointer to the scene's exit function.</param>
/// <returns>Pointer to a scene struct.</returns>
Scene* CreateScene(char* name, FunctionPtr init, FunctionPtr update, FunctionPtr exit)
{
	if (init && update && exit)
	{
		Scene* scene = malloc(sizeof(Scene));
		if (scene)
		{
			scene->name = name;
			scene->init = init;
			scene->update = update;
			scene->exit = exit;

			return scene;
		}
	}
	return NULL;
}

/// <summary>
/// Initializes the scene manager and registers individual scenes.
/// </summary>
void SceneManager_Initialize()
{
	FunctionPtr init, update, exit;
	Scene* temp;

	splashscreen_sceneInit(&init, &update, &exit);
	temp = CreateScene("splashscreen", init, update, exit);
	LL_Add(&sceneList, temp);
	//currentScene = temp;

	game_sceneInit(&init, &update, &exit);
	temp = CreateScene("mainmenu", init, update, exit);
	LL_Add(&sceneList, temp);

	game2_sceneInit(&init, &update, &exit);
	temp = CreateScene("game2", init, update, exit);
	LL_Add(&sceneList, temp);

	Marcus_sceneInit(&init, &update, &exit);
	temp = CreateScene("marcus", init, update, exit);
	LL_Add(&sceneList, temp);

	HongYu_sceneInit(&init, &update, &exit);
	temp = CreateScene("hongyu", init, update, exit);
	LL_Add(&sceneList, temp);

	Adrian_sceneInit(&init, &update, &exit);
	temp = CreateScene("adrian", init, update, exit);
	LL_Add(&sceneList, temp);

	Weiyi_sceneInit(&init, &update, &exit);
	temp = CreateScene("weiyi", init, update, exit);
	LL_Add(&sceneList, temp);

	Xinyun_sceneInit(&init, &update, &exit);
	temp = CreateScene("xinyun", init, update, exit);
	LL_Add(&sceneList, temp);

	LevelEditorScene_sceneInit(&init, &update, &exit);
	temp = CreateScene("editor", init, update, exit);
	LL_Add(&sceneList, temp);

	LevelOne_sceneInit(&init, &update, &exit);
	temp = CreateScene("levelone", init, update, exit);
	LL_Add(&sceneList, temp);

	LevelOneAStar_sceneInit(&init, &update, &exit);
	temp = CreateScene("leveloneastar", init, update, exit);
	LL_Add(&sceneList, temp);

	gameEnd_sceneInit(&init, &update, &exit);
	temp = CreateScene("gameEnd", init, update, exit);
	LL_Add(&sceneList, temp);

	SceneOptions_sceneInit(&init, &update, &exit);
	temp = CreateScene("options", init, update, exit);
	LL_Add(&sceneList, temp);

	SceneCredits_sceneInit(&init, &update, &exit);
	temp = CreateScene("credits", init, update, exit);
	LL_Add(&sceneList, temp);
}

/// <summary>
/// Changes a scene, based on a pointer to the next scene.
/// </summary>
/// <param name="nextScene">Pointer to the next scene.</param>
void SceneManager_ChangeScene(Scene* nextScene)
{
	if (nextScene)
	{
		CP_Engine_SetNextGameState(nextScene->init, nextScene->update, nextScene->exit);
		currentScene = nextScene;
	}
}

/// <summary>
/// Searches through a Linked List containing all registered Scene structs to find a matching scene.
/// </summary>
/// <param name="curr">Void pointer to fill with scene data.</param>
/// <param name="arg">Void pointer containing the name of the scene to look for.</param>
/// <returns>Pointer to the scene, if there is a matching one, if not, returns NULL.</returns>
void* findScene(void* curr, void* arg) 
{
	char* name = (char*)arg;
	Scene* s = (Scene*)curr;

	if (strcmp(s->name, name) == 0)//match
		return curr;
	return NULL;
}

/// <summary>
/// Changes scenes by searching for matching scenes of the same name.
/// </summary>
/// <param name="sceneName">Human readable name of the scene to change to.</param>
void SceneManager_ChangeSceneByName(char* sceneName)
{
	Scene* nextScene = LL_Find(sceneList, findScene, (void*)sceneName);
	SceneManager_ChangeScene(nextScene);
}

/// <summary>
/// Cleans memory allocated for scenes
/// </summary>
void SceneManager_Exit()
{
	LinkedList* node = sceneList;
	while (node)
	{
		free(node->curr);
		node = node->next;
	}
	LL_Clear(&sceneList);
}
