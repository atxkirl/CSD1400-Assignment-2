/*!
@file Weiyi.c
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

void Weiyi_init(void)
{
    SM_SystemsInit();

    GameObject* g = GOM_Create(CIRCLE);
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);
    RM_AddComponent(g);
}

void Weiyi_update(void)
{
    SM_SystemsPreUpdate();

    //Do stuff here...


    SM_SystemsUpdate();

    SM_SystemsLateUpdate();
}

void Weiyi_exit(void)
{
    SM_SystemsExit();

    _CrtDumpMemoryLeaks();
}

void Weiyi_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Weiyi_init;
    *update = Weiyi_update;
    *exit = Weiyi_exit;
}
