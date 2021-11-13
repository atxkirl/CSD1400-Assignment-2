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
#include "CollisionManager.h"
#include "Player.h"
#include "Objective_Pickup.h"

#include <stdlib.h>

void xy_OnCollision(Collider* left, Collider* right)
{
   
}

void Xinyun_init(void)
{
    SM_SystemsInit();

    PLY_CreatePlayer(90, 90);

    OB_PickupInit();
}

void Xinyun_update(void)
{
    SM_SystemsPreUpdate();

    PLY_Update();

    SM_SystemsUpdate();

    OB_PickupUpdate();

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
