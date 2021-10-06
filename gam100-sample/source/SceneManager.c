#include "SceneManager.h"
#include <stdio.h>
#include <stdlib.h>

/* Scene Struct */
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

/* SceneManager */
void SceneManager_Initialize()
{
	FunctionPtr init, update, exit;
	Scene* temp;

	game_sceneInit(&init, &update, &exit);
	temp = CreateScene("game", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	// For now I'll just set currentScene to temp, so that game.c will be the first scene. PLS REMOVE LATER
	currentScene = temp;

	game2_sceneInit(&init, &update, &exit);
	temp = CreateScene("game2", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
}

void SceneManager_ChangeScene(Scene* nextScene)
{
	if (nextScene)
	{
		CP_Engine_SetNextGameState(nextScene->init, nextScene->update, nextScene->exit);
		currentScene = nextScene;
	}
}

void SceneManager_ChangeSceneByName(char* sceneName)
{
	// TODO: 
	// - Loop through the list of Scene* to find the one with the same name
	//   then call SceneManager_ChangeScene to change to that scene.
	// - This will allow for change scene calls to be called in other scenes by changing scenes by name.
}
