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
	sceneList = LL_Add(sceneList, temp);

	game2_sceneInit(&init, &update, &exit);
	temp = CreateScene("game2", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	sceneList = LL_Add(sceneList, temp);

	Marcus_sceneInit(&init, &update, &exit);
	temp = CreateScene("marcus", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	sceneList = LL_Add(sceneList, temp);

	HongYu_sceneInit(&init, &update, &exit);
	temp = CreateScene("hongyu", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	sceneList = LL_Add(sceneList, temp);

	Adrian_sceneInit(&init, &update, &exit);
	temp = CreateScene("adrian", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	sceneList = LL_Add(sceneList, temp);

	Weiyi_sceneInit(&init, &update, &exit);
	temp = CreateScene("weiyi", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	sceneList = LL_Add(sceneList, temp);

	Xinyun_sceneInit(&init, &update, &exit);
	temp = CreateScene("xinyun", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	sceneList = LL_Add(sceneList, temp);

	LevelEditorScene_sceneInit(&init, &update, &exit);
	temp = CreateScene("editor", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	sceneList = LL_Add(sceneList, temp);

	LevelOne_sceneInit(&init, &update, &exit);
	temp = CreateScene("levelone", init, update, exit);
	// TODO: Add some way to store all the Scene* in a list or array or something.
	sceneList = LL_Add(sceneList, temp);
}

void SceneManager_ChangeScene(Scene* nextScene)
{
	if (nextScene)
	{
		CP_Engine_SetNextGameState(nextScene->init, nextScene->update, nextScene->exit);
		currentScene = nextScene;
	}
}

void* findScene(void* curr, void* arg) 
{
	char* name = (char*)arg;
	Scene* s = (Scene*)curr;

	if (strcmp(s->name, name) == 0)//match
		return curr;
	return NULL;
}
void SceneManager_ChangeSceneByName(char* sceneName)
{
	// TODO: 
	// - Loop through the list of Scene* to find the one with the same name
	//   then call SceneManager_ChangeScene to change to that scene.
	// - This will allow for change scene calls to be called in other scenes by changing scenes by name.

	Scene* nextScene = LL_Find(sceneList, findScene, (void*)sceneName);
	SceneManager_ChangeScene(nextScene);
}
