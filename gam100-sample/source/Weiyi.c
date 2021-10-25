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
#include "CollisionManager.h"
#include "Player.h"

#include <time.h>
#include <stdlib.h>

GameObject* e = NULL;
Renderer* r;

void wy_OnCollision(Collider* left, Collider* right) {

    return;
}
void Weiyi_init(void)
{
    SM_SystemsInit();

    PLY_CreatePlayer();

    e = GOM_Create2(CIRCLE, CP_Vector_Set(20, 20), 0.0f, CP_Vector_Set(50, 50));
    e->tag = "enemy";
    CLM_Set(CLM_AddComponent(e),COL_CIRCLE, wy_OnCollision);

    r = RM_AddComponent(e);

}

void Weiyi_update(void)
{
    SM_SystemsPreUpdate();
   
    PLY_Update();
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
