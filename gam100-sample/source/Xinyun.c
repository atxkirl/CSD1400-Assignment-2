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
#include "DetectMemoryLeak.h"
#include "SystemManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Objective_Pickup.h"

#include <stdlib.h>

GameObject* credits_button;

void xy_OnCollision(Collider* left, Collider* right)
{
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "crtest") == 0)
            SceneManager_ChangeSceneByName("credits");

    }
}

void Xinyun_init(void)
{
    float xScale = CP_System_GetWindowWidth() / 100.0f, yScale = CP_System_GetWindowHeight() / 100.0f;

    credits_button = GOM_Create2(RECTANGLE, CP_Vector_Set(50 * xScale, 90 * yScale), 0.0f, CP_Vector_Set(30, 30));
    credits_button->tag = "crtest";
    Collider* ck = CLM_AddComponent(credits_button);
    CLM_Set(ck, COL_BOX, xy_OnCollision);
    ck->space = COLSPC_SCREEN;
    ck->isLockedPos = 1;
    Renderer* cr = RM_AddComponent(credits_button);
    //cr->text = "test";
    RM_SetText(cr, "test");
    cr->renderPriority = PRI_UI;

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
