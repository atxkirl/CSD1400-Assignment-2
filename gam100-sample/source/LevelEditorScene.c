/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       LevelEditorScene.c
@author     Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course     CSD1400 Software Engineering Project 1
@Team       BananaBoi
@brief      This file contains the scene used for Level Editor.
*//*________________________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "LevelEditor.h"
#include "SystemManager.h"

/// <summary>
/// Initialise the level editor scene
/// </summary>
/// <param name=""></param>
void LevelEditorScene_init(void)
{
    SM_SystemsInit();

    //RM_AddComponent(g);
    LevelEditorInit();
}

/// <summary>
/// Update the level editor
/// </summary>
/// <param name=""></param>
void LevelEditorScene_update(void)
{
    //SM_SystemsPreUpdate();
    LevelEditorUpdate();
    //SM_SystemsUpdate();
    //SM_SystemsLateUpdate();
}

/// <summary>
/// Calls the exit functions from level editor and other systems
/// </summary>
/// <param name=""></param>
void LevelEditorScene_exit(void)
{
    LevelEditorExit();
    SM_SystemsExit();
}

/// <summary>
/// For scene change use
/// </summary>
/// <param name="init"></param>
/// <param name="update"></param>
/// <param name="exit"></param>
void LevelEditorScene_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = LevelEditorScene_init;
    *update = LevelEditorScene_update;
    *exit = LevelEditorScene_exit;
}
