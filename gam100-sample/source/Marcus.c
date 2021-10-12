/*!
@file Marcus.c
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

void Marcus_init(void)
{
    RM_Init();
    GOM_Init();

    GameObject* g = GOM_CreateGameObject();
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);

    RM_AddRenderObject(g);
    LevelEditorInit();
}

void Marcus_update(void)
{
    LevelEditorUpdate();
}

void Marcus_exit(void)
{
    LevelEditorExit();
    RM_ClearRenderObjects();
    GOM_Clear();
}

void Marcus_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Marcus_init;
    *update = Marcus_update;
    *exit = Marcus_exit;
}
