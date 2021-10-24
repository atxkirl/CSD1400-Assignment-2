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

    g = GOM_Create2(RECTANGLE, CP_Vector_Set(50, 20), 0.0f, CP_Vector_Set(50, 50));
    g->tag = "player";
    Renderer* r = RM_AddComponent(g);
    RM_LoadImage(r, "Assets/bananaboi.png");
    CLM_Set(CLM_AddComponent(g), COL_BOX, hy_OnCollision);

    //int width = CP_System_GetWindowWidth();
    //int height = CP_System_GetWindowHeight();

    GameObject* button = GOM_Create2(RECTANGLE, 
        CP_Vector_Set(80, 20), 0.0f, CP_Vector_Set(30, 20));
    r = RM_AddComponent(button);
    r->text = "test";
    r->renderPriority = PRI_UI;
    button->tag = "test";
    Collider* c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, hy_OnCollision);
    c->space = COLSPC_SCREEN;

    GameObject* wall = GOM_Create2(RECTANGLE, 
        CP_Vector_Set(0, 0), 0.0f, CP_Vector_Set(50, 50));
    r = RM_AddComponent(wall);
    RM_LoadImage(r, "Assets/testspritesheet.png");
    Animation* a = AM_AddComponent(wall);
    AM_SetSprite(a, 4, 2, 8, 4);
    CLM_Set(CLM_AddComponent(wall), COL_BOX, hy_OnCollision);
    
    wall = GOM_Create2(CIRCLE,
        CP_Vector_Set(30, 80), 0.0f, CP_Vector_Set(30, 30));
    RM_AddComponent(wall);
    //wall->tag = "wall";
    CLM_Set(CLM_AddComponent(wall), COL_CIRCLE, NULL);

    wall = GOM_Create2(CIRCLE,
        CP_Vector_Set(30, 80), 0.0f, CP_Vector_Set(30, 30));
    r = RM_AddComponent(wall);
    r->color = CP_Color_Create(200, 180, 180, 255);
    r->text = "lock";
    c = CLM_AddComponent(wall);
    CLM_Set(c, COL_CIRCLE, NULL);
    c->isLockedPos = 1;

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
        clickPoint = GOM_CreateTemp(EMPTY);//param doesnt matter
        clickPoint->position = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
        clickPoint->isEnabled = 0;
        clickPoint->tag = "Click";
        Collider* c = CLM_AddComponent(clickPoint);
        CLM_Set(c, COL_POINT, NULL);
        c->space = COLSPC_SCREEN;
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
    Renderer* r = (Renderer*)SM_GetComponent(g, COM_RENDERER);
    r->textColor = CP_Color_Create(255, 255, 255, 255);

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
