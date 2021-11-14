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
	Scene* nextScene = LL_Find(sceneList, findScene, (void*)sceneName);
	SceneManager_ChangeScene(nextScene);
}
