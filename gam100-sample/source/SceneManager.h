#pragma once

#include "cprocessing.h"

/* Forward Declarations for all scene Init functions: */
void game_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void game2_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);

/* Scene Struct */
typedef struct
{
	char* name;
	FunctionPtr init;
	FunctionPtr update;
	FunctionPtr exit;
}Scene;

Scene* CreateScene(char* name, FunctionPtr init, FunctionPtr update, FunctionPtr exit);

/* Scene Manager */
void SceneManager_Initialize();
void SceneManager_ChangeScene(Scene* nextScene);
void SceneManager_ChangeSceneByName(char* sceneName);

Scene* currentScene;