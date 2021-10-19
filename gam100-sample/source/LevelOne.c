/*!
@file LevelOne.c
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains functions of game init, update, exit
*//*______________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Helpers.h"
#include "LevelEditor.h"
#include "FileParser.h"

void LevelOne_init(void)
{
    RM_Init();
    GOM_Init();

    //RM_AddRenderObject(g);
    LevelEditorInit();
    LoadGrid("test");
}

void LevelOne_update(void)
{
    LevelEditorUpdate();
}

void LevelOne_exit(void)
{
    LevelEditorExit();
    RM_ClearRenderObjects();
    GOM_Clear();
}

void LevelOne_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = LevelOne_init;
    *update = LevelOne_update;
    *exit = LevelOne_exit;
}
