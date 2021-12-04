/*
@file	Weiyi.c
@author	Tan Wee Yi (weeyi.t)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@brief	TEST scene to test out player script for the functionailty.
*//*______________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "DetectMemoryLeak.h"
#include "SystemManager.h"
#include "CollisionManager.h"
#include "Player.h"
#include "SceneManager.h"
#include "SoundManager.h"

#include <time.h>
#include <stdlib.h>

GameObject* e = NULL;
GameObject* e2 = NULL;
GameObject* e3 = NULL;
GameObject* e4 = NULL;
GameObject* e5 = NULL;
Collider* k;
Renderer* r;

/// <summary>
/// Handles collision that is in the scene
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void wy_OnCollision(Collider* left, Collider* right) {

    if (strcmp(left->obj->tag, "obj1") == 0 && strcmp(right->obj->tag, "player") == 0) SM_DeleteGameObject(e3);

    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "test") == 0)
            SceneManager_ChangeSceneByName("gameEnd");

    }

    return;

}

/// <summary>
/// Inits the scene with buttons and also the colliders for them.
/// </summary>
/// <param name=""></param>
void Weiyi_init(void)
{
    SM_SystemsInit();
    float xScale = CP_System_GetWindowWidth() / 100.0f, yScale = CP_System_GetWindowHeight() / 100.0f;


    PLY_CreatePlayer(90,90);

    e = GOM_Create2(CIRCLE, CP_Vector_Set(20, 20), 0.0f, CP_Vector_Set(50, 50));
    e->tag = "BBEM";
    k = CLM_AddComponent(e);
    CLM_Set(k,COL_CIRCLE, NULL);

    
    e2 = GOM_Create2(RECTANGLE, CP_Vector_Set(10, 10), 0.0f, CP_Vector_Set(15, 15));
    e2->tag = "mud";
    k = CLM_AddComponent(e2);
    CLM_Set(k, COL_BOX, NULL);
    k->isTrigger = 1;
    

    e3 = GOM_Create2(CIRCLE, CP_Vector_Set(50, 50), 0.0f, CP_Vector_Set(15, 15));
    e3->tag = "obj1";
    k = CLM_AddComponent(e3);
    CLM_Set(k, COL_CIRCLE, wy_OnCollision);
    k->isTrigger = 1;

    e4 = GOM_Create2(RECTANGLE, CP_Vector_Set(130, 130), 0.0f, CP_Vector_Set(30, 30));
    e4->tag = "box";
    k = CLM_AddComponent(e4);
    CLM_Set(k,COL_BOX, NULL);
    k->isLockedPos = 1;

    e5 = GOM_Create2(RECTANGLE, CP_Vector_Set(50*xScale, 90*yScale), 0.0f, CP_Vector_Set(30, 30));
    e5->tag = "test";
    k = CLM_AddComponent(e5);
    CLM_Set(k, COL_BOX, wy_OnCollision);
    k->space = COLSPC_SCREEN;
    k->isLockedPos = 1;
    r = RM_AddComponent(e5);
    //r->text = "test";
    RM_SetText(r, "test");
    r->renderPriority = PRI_UI;


    r = RM_AddComponent(e);
    r = RM_AddComponent(e2);
    r = RM_AddComponent(e3);
    r = RM_AddComponent(e4);
    //r->text = "hide";
    RM_SetText(r, "hide");

    SDM_Init();
    SDM_PlayBgMusic(2);
}

/// <summary>
/// Updates the game scene to check for when there is a mouse press and also handles the drawing of
/// the buttons.
/// </summary>
/// <param name=""></param>
void Weiyi_update(void)
{
    SM_SystemsPreUpdate();
   
    PLY_Update();
    SM_SystemsUpdate(0);

    SM_SystemsLateUpdate();
}

/// <summary>
/// Exits the scene.
/// </summary>
/// <param name=""></param>
void Weiyi_exit(void)
{
    SM_SystemsExit();

    _CrtDumpMemoryLeaks();
    SDM_StopBgMusic();
}

/// <summary>
/// Inits the scene for the scenemanager so that it can 
/// </summary>
/// <param name="init"></param>
/// <param name="update"></param>
/// <param name="exit"></param>
void Weiyi_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Weiyi_init;
    *update = Weiyi_update;
    *exit = Weiyi_exit;
}
