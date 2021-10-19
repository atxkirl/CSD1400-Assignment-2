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
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Helpers.h"
#include "LevelEditor.h"

void LevelEditorScene_init(void)
{
    RM_Init();
    GOM_Init();

    GameObject* g = GOM_CreateGameObject();
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);

    //RM_AddRenderObject(g);
    LevelEditorInit();
}

void LevelEditorScene_update(void)
{
    LevelEditorUpdate();
}

void LevelEditorScene_exit(void)
{
    LevelEditorExit();
    RM_ClearRenderObjects();
    GOM_Clear();
}

void LevelEditorScene_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = LevelEditorScene_init;
    *update = LevelEditorScene_update;
    *exit = LevelEditorScene_exit;
}
