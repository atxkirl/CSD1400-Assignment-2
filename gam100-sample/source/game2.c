/*!
@file game2.c
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

void game2_init(void)
{
    RM_Init();
    GOM_Init();

    GameObject* g = GOM_CreateGameObject();
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);

}

void game2_update(void)
{
    CP_Settings_Fill(CP_Color_Create(255, 128, 128, 255));
    RM_Render();
    CP_Settings_Fill(CP_Color_Create(128, 128, 128, 255));
}

void game2_exit(void)
{
    RM_ClearRenderObjects();
    GOM_Clear();
}

void game2_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = game2_init;
    *update = game2_update;
    *exit = game2_exit;
}
