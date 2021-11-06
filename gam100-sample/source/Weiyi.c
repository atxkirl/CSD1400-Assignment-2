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
GameObject* e2 = NULL;
GameObject* e3 = NULL;
GameObject* e4 = NULL;
Collider* k;
Renderer* r;

void wy_OnCollision(Collider* left, Collider* right) {

    if (strcmp(left->obj->tag, "obj1") == 0 && strcmp(right->obj->tag, "player") == 0) SM_DeleteGameObject(e3);
    return;

}
void Weiyi_init(void)
{
    SM_SystemsInit();

    PLY_CreatePlayer();

    e = GOM_Create2(CIRCLE, CP_Vector_Set(20, 20), 0.0f, CP_Vector_Set(50, 50));
    e->tag = "enemy";
    k = CLM_AddComponent(e);
    CLM_Set(k,COL_CIRCLE, NULL);

    
    e2 = GOM_Create2(RECTANGLE, CP_Vector_Set(10, 10), 0.0f, CP_Vector_Set(15, 15));
    e2->tag = "mud";
    k = CLM_AddComponent(e2);
    CLM_Set(k, COL_BOX, wy_OnCollision);
    k->isTrigger = 1;
    

    e3 = GOM_Create2(CIRCLE, CP_Vector_Set(50, 50), 0.0f, CP_Vector_Set(15, 15));
    e3->tag = "obj1";
    k = CLM_AddComponent(e3);
    CLM_Set(k, COL_CIRCLE, wy_OnCollision);
    k->isTrigger = 1;

    r = RM_AddComponent(e);
    r = RM_AddComponent(e2);
    r = RM_AddComponent(e3);
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
