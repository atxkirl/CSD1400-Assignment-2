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
#include "Helpers.h"
#include "../DetectMemoryLeak.h"
#include "SystemManager.h"

#include <stdlib.h>

void Adrian_init(void)
{
    SM_SystemsInit();

    GameObject* g = GOM_Create(CIRCLE, PRI_GAME_OBJECT);
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);

}

void Adrian_update(void)
{
    SM_SystemsPreUpdate();

    //Do stuff here...

    SM_SystemsUpdate();
    CP_Settings_Fill(CP_Color_Create(255, 255, 128, 255));
    SM_SystemsLateUpdate();
    //RM_Render();
    CP_Settings_Fill(CP_Color_Create(128, 128, 128, 255));
}

void Adrian_exit(void)
{
    SM_SystemsExit();

    _CrtDumpMemoryLeaks();
}

void Adrian_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Adrian_init;
    *update = Adrian_update;
    *exit = Adrian_exit;
}
