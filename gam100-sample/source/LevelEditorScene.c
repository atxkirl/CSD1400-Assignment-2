/*!
@file LevelEditorScene.c
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains functions of game init, update, exit
*//*______________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "LevelEditor.h"
#include "SystemManager.h"

void LevelEditorScene_init(void)
{
    SM_SystemsInit();

    //RM_AddComponent(g);
    LevelEditorInit();
}

void LevelEditorScene_update(void)
{
    //SM_SystemsPreUpdate();
    LevelEditorUpdate();
    //SM_SystemsUpdate();
    //SM_SystemsLateUpdate();
}

void LevelEditorScene_exit(void)
{
    LevelEditorExit();
    SM_SystemsExit();
}

void LevelEditorScene_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = LevelEditorScene_init;
    *update = LevelEditorScene_update;
    *exit = LevelEditorScene_exit;
}
