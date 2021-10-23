/*!
@file HongYu.c
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

GameObject* g = NULL;
int tempSize = 0;

int hy_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(left->obj->tag, "player") == 0 
        && strcmp(right->obj->tag, "wall") == 0)
    {
        //GameObject* p = left->obj;
        //GameObject* w = right->obj;
        //printf("OUCH!");
        //push the player out of wall
        //wall -> player vector
        //CP_Vector relative = CP_Vector_Subtract(p->position, w->position);
        //CP_Vector pVel = CP_Vector_Normalize(left->velocity);
        //if x is positive, check minx
        
        //p->position = CP_Vector_Subtract(p->position, CP_Vector_Scale(left->velocity, CP_System_GetDt()));
    }
    else if (strcmp(left->obj->tag, "test") == 0 && strcmp(right->obj->tag, "Click") == 0)
    {
        tempSize = ++tempSize % 3;
        switch (tempSize)
        {
        case 0:
            CP_System_SetWindowSize(400, 400);
            break;
        case 1:
            CP_System_SetWindowSize(800, 600);
            break;
        case 2:
            CP_System_SetWindowSize(1280, 720);
            break;
        }

    }
    return CLM_RESPONSE_REMOVENONE;
}

void HongYu_init(void)
{
    SM_SystemsInit();

    g = GOM_Create(RECTANGLE, PRI_GAME_OBJECT);
    g->scale = CP_Vector_Set(15, 15);
    g->position = CP_Vector_Set(50, 20);
    g->tag = "player";
    CLM_AddCollider(g, hy_OnCollision, COL_BOX, g->scale.x, g->scale.y);

    //int width = CP_System_GetWindowWidth();
    //int height = CP_System_GetWindowHeight();

    GameObject* button = GOM_Create(RECTANGLE, PRI_UI);
    button->position = CP_Vector_Set(80, 20);
    //button->rotation = 10.0f;
    button->scale = CP_Vector_Set(30, 20);
    button->color = CP_Color_Create(255, 255, 255, 255);
    button->text = "test";
    button->tag = "test";
    CLM_AddCollider(button, hy_OnCollision, RECTANGLE, button->scale.x, button->scale.y);

    GameObject* wall = GOM_Create(RECTANGLE, PRI_GAME_OBJECT);
    wall->position = CP_Vector_Set(0,0);
    //button->rotation = 10.0f;
    wall->scale = CP_Vector_Set(50,25);
    wall->color = CP_Color_Create(200, 200, 200, 122);
    wall->tag = "wall";
    CLM_AddCollider(wall, hy_OnCollision, COL_BOX, wall->scale.x, wall->scale.y);
    
    wall = GOM_Create(CIRCLE, PRI_GAME_OBJECT);
    wall->position = CP_Vector_Set(30, 80);
    wall->scale = CP_Vector_Set(30, 30);
    wall->color = CP_Color_Create(200, 200, 200, 255);
    //wall->tag = "wall";
    CLM_AddCollider(wall, NULL, COL_CIRCLE, wall->scale.x);

    wall = GOM_Create(CIRCLE, PRI_GAME_OBJECT);
    wall->position = CP_Vector_Set(30, 80);
    wall->scale = CP_Vector_Set(30, 30);
    wall->color = CP_Color_Create(200, 180, 180, 255);
    Collider* c = CLM_AddCollider(wall, NULL, COL_CIRCLE, wall->scale.x);
    c->isLockedPos = 1;
    wall->text = "lock";

    //GameObject* wad = GOM_CreateGameObject(CIRCLE, GAME_OBJECT);
    //wad->position = CP_Vector_Set(0.1f * width, 0.9f * height);
    //wad->scale = CP_Vector_Set(0.1f * width, 30);
    //wad->color = CP_Color_Create(255, 255, 255, 255);
}

void HongYu_update(void)
{
    SM_SystemsPreUpdate();
    float dt = CP_System_GetDt();
    float spd = 200.0f;
    //Collider* gc = CLM_GetComponent(g);
    //gc->velocity = CP_Vector_Set(0, 0);
    if (CP_Input_KeyDown((enum CP_KEY)KEY_W))
    {
        g->position.y -= spd * dt;
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_S))
    {
        g->position.y += spd * dt;
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_A))
    {
        g->position.x -= spd * dt;
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_D))
    {
        g->position.x += spd * dt;
    }
    GameObject* clickPoint = NULL;
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
        //Creates a point obj to test collision against button
        clickPoint = GOM_CreateTemp(CIRCLE, PRI_GAME_OBJECT);//param doesnt matter
        clickPoint->position = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
        clickPoint->isEnabled = 0;
        clickPoint->tag = "Click";
        CLM_AddCollider(clickPoint, NULL, COL_POINT);
    }
    if (CP_Input_KeyTriggered((enum CP_KEY)KEY_1))
    {
        RM_SetCameraScale(CP_Vector_Set(1.0f, 1.0f));
    }
    else if (CP_Input_KeyTriggered((enum CP_KEY)KEY_2))
    {
        
    }

    char str[20];
    sprintf_s(str, 20,"%.1f,%.1f", g->position.x, g->position.y);
    //g->text = str;
    g->textColor = CP_Color_Create(255, 255, 255, 255);

    SM_SystemsUpdate();
    RM_SetCameraPosition(g->position);

    ////imagine late update
    //if (clickPoint)
    //{
    //    CLM_RemoveGO(clickPoint);
    //    GOM_Delete(clickPoint);
    //}
    SM_SystemsLateUpdate();
}

void HongYu_exit(void)
{
    SM_SystemsExit();
}

void HongYu_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = HongYu_init;
    *update = HongYu_update;
    *exit = HongYu_exit;
}
