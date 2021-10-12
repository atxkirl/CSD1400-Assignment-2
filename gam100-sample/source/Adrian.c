/*!
@file Adrian.c
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
#include "../DetectMemoryLeak.h"

#include <stdlib.h>

void Adrian_init(void)
{
    RM_Init();
    GOM_Init();

    GameObject* g = GOM_CreateGameObject();
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);
}

void Adrian_update(void)
{
    CP_Settings_Fill(CP_Color_Create(255, 255, 128, 255));
    RM_Render();
    CP_Settings_Fill(CP_Color_Create(128, 128, 128, 255));
}

void Adrian_exit(void)
{
    RM_ClearRenderObjects();
    GOM_Clear();

    _CrtDumpMemoryLeaks();
}

void Adrian_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Adrian_init;
    *update = Adrian_update;
    *exit = Adrian_exit;
}
