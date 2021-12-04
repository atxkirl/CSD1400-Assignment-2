/*
@file	SceneManager.c
@author	Adrian Tan (t.xingkhiangadrian)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@date	06/10/2021
@brief	Contains functions to register and change scenes during runtime.
*//*______________________________________________________________________*/

#pragma once

#include "cprocessing.h"
#include "LinkedList.h"

/* Forward Declarations for all scene Init functions: */
void game_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void game2_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void Marcus_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void HongYu_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void Adrian_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void Weiyi_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void Xinyun_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void LevelEditorScene_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void LevelOne_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void LevelOneAStar_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void SceneOptions_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void gameEnd_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void splashscreen_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);
void SceneCredits_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit);

/* Scene Struct */
typedef struct
{
	char* name;
	FunctionPtr init;
	FunctionPtr update;
	FunctionPtr exit;
}Scene;

/// <summary>
/// Creates a Scene struct to hold data on a single scene to allow for easy scene changing.
/// </summary>
/// <param name="name">Human readable name of the scene.</param>
/// <param name="init">Function pointer to the scene's init function.</param>
/// <param name="update">Function pointer to the scene's update function.</param>
/// <param name="exit">Function pointer to the scene's exit function.</param>
/// <returns>Pointer to a scene struct.</returns>
Scene* CreateScene(char* name, FunctionPtr init, FunctionPtr update, FunctionPtr exit);

/* Scene Manager */
/// <summary>
/// Initializes the scene manager and registers individual scenes.
/// </summary>
void SceneManager_Initialize();
/// <summary>
/// Changes a scene, based on a pointer to the next scene.
/// </summary>
/// <param name="nextScene">Pointer to the next scene.</param>
void SceneManager_ChangeScene(Scene* nextScene);
/// <summary>
/// Changes scenes by searching for matching scenes of the same name.
/// </summary>
/// <param name="sceneName">Human readable name of the scene to change to.</param>
void SceneManager_ChangeSceneByName(char* sceneName);
/// <summary>
/// Cleans memory allocated for scenes
/// </summary>
void SceneManager_Exit();
