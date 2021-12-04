/*!
@file            Xinyun.c
@author          Koh Xin Yun (xinyun.k)
@course          CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief           This file contains functions of game init, update, exit
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

/// <summary>
/// Handles the collision of the the GameObjects based on the left and right collider parameters.
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void xy_OnCollision(Collider* left, Collider* right)
{
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "crtest") == 0)
            SceneManager_ChangeSceneByName("credits");

    }
}

/// <summary>
/// Initialises variables in this scene.
/// </summary>
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

/// <summary>
/// Update.
/// </summary>
void Xinyun_update(void)
{
    SM_SystemsPreUpdate();

    PLY_Update();

    SM_SystemsUpdate(0);

    OB_PickupUpdate();

    SM_SystemsLateUpdate();

}

/// <summary>
/// Exit function that's called before scene change.
/// </summary>
void Xinyun_exit(void)
{
    SM_SystemsExit();

    _CrtDumpMemoryLeaks();
}

/// <summary>
/// Called by SceneManager to pass function pointers for this scene's init, update and exit.
/// </summary>
/// <param name="init">Function pointer to scene init.</param>
/// <param name="update">Function pointer to scene update.</param>
/// <param name="exit">Funciton pointer to scene exit.</param>
void Xinyun_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Xinyun_init;
    *update = Xinyun_update;
    *exit = Xinyun_exit;
}
