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
#include "Helpers.h"
#include "SystemManager.h"
#include "Player.h"

void game2_init(void)
{
    SM_SystemsInit();

    //GameObject* g = GOM_Create(CIRCLE);
    //g->scale = CP_Vector_Set(20, 20);
    //g->position = CP_Vector_Set(50, 20);
    //RM_AddComponent(g);

    PLY_CreatePlayer(50, 20);
}

void game2_update(void)
{
    SM_SystemsPreUpdate();

    PLY_Update();
    SM_SystemsUpdate(0);

    CP_Settings_Fill(CP_Color_Create(255, 128, 128, 255));
    SM_SystemsLateUpdate();
    CP_Settings_Fill(CP_Color_Create(128, 128, 128, 255));
}

void game2_exit(void)
{
    SM_SystemsExit();
}

void game2_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = game2_init;
    *update = game2_update;
    *exit = game2_exit;
}
