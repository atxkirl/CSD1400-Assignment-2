/*!
@file Xinyun.c
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

void Xinyun_init(void)
{
    SM_SystemsInit();

    GameObject* g = GOM_Create(CIRCLE, PRI_GAME_OBJECT);
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);

}

void Xinyun_update(void)
{
    SM_SystemsPreUpdate();

    //Do stuff here...


    SM_SystemsUpdate();

    SM_SystemsLateUpdate();
}

void Xinyun_exit(void)
{
    SM_SystemsExit();

    _CrtDumpMemoryLeaks();
}

void Xinyun_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Xinyun_init;
    *update = Xinyun_update;
    *exit = Xinyun_exit;
}
