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
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Helpers.h"
#include "CollisionManager.h"

GameObject* g = NULL;
int tempSize = 0;

int hy_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(left->obj->tag, "player") == 0 
        && strcmp(right->obj->tag, "wall") == 0)
    {
        GameObject* p = left->obj;
        //GameObject* w = right->obj;
        printf("OUCH!");
        //push the player out of wall
        //wall -> player vector
        //CP_Vector relative = CP_Vector_Subtract(p->position, w->position);
        //CP_Vector pVel = CP_Vector_Normalize(left->velocity);
        //if x is positive, check minx
        
        p->position = CP_Vector_Subtract(p->position, CP_Vector_Scale(left->velocity, CP_System_GetDt()));
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
    RM_Init();
    GOM_Init();

    g = GOM_CreateGameObject(CIRCLE, PRI_GAME_OBJECT);
    g->scale = CP_Vector_Set(15, 15);
    g->position = CP_Vector_Set(50, 20);
    g->tag = "player";
    CLM_AddCollider(g, hy_OnCollision, COL_CIRCLE, g->scale.x);

    //int width = CP_System_GetWindowWidth();
    //int height = CP_System_GetWindowHeight();

    GameObject* button = GOM_CreateGameObject(RECTANGLE, PRI_UI);
    button->position = CP_Vector_Set(80, 20);
    //button->rotation = 10.0f;
    button->scale = CP_Vector_Set(20, 10);
    button->color = CP_Color_Create(255, 255, 255, 255);
    button->text = "test";
    button->tag = "test";
    CLM_AddCollider(button, hy_OnCollision, RECTANGLE, button->scale.x, button->scale.y);

    GameObject* wall = GOM_CreateGameObject(RECTANGLE, PRI_GAME_OBJECT);
    wall->position = CP_Vector_Set(100,50);
    //button->rotation = 10.0f;
    wall->scale = CP_Vector_Set(60,30);
    wall->color = CP_Color_Create(200, 200, 200, 255);
    wall->tag = "wall";
    CLM_AddCollider(wall, hy_OnCollision, COL_BOX, wall->scale.x, wall->scale.y);
    

    //GameObject* wad = GOM_CreateGameObject(CIRCLE, GAME_OBJECT);
    //wad->position = CP_Vector_Set(0.1f * width, 0.9f * height);
    //wad->scale = CP_Vector_Set(0.1f * width, 30);
    //wad->color = CP_Color_Create(255, 255, 255, 255);
}

void HongYu_update(void)
{
    float dt = CP_System_GetDt();
    float spd = 200.0f;
    Collider* gc = CLM_GetComponent(g);
    gc->velocity = CP_Vector_Set(0, 0);
    if (CP_Input_KeyDown((enum CP_KEY)KEY_W))
    {
        g->position.y -= spd * dt;
        gc->velocity = CP_Vector_Set(0, -spd);
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_S))
    {
        g->position.y += spd * dt;
        gc->velocity = CP_Vector_Set(0, spd);
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_A))
    {
        g->position.x -= spd * dt;
        gc->velocity = CP_Vector_Set(-spd, 0);
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_D))
    {
        g->position.x += spd * dt;
        gc->velocity = CP_Vector_Set(spd, 0);
    }
    GameObject* clickPoint = NULL;
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
        //Creates a point obj to test collision against button
        clickPoint = GOM_CreateGameObject(CIRCLE, PRI_GAME_OBJECT);//param doesnt matter
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

    RM_SetCameraPosition(g->position);

    CLM_CheckCollisions();

    //imagine late update
    if (clickPoint)
    {
        CLM_RemoveGO(clickPoint);
        GOM_Delete(clickPoint);
    }


    RM_Render();
}

void HongYu_exit(void)
{
    CLM_Clear();
    RM_ClearRenderObjects();
    GOM_Clear();
}

void HongYu_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = HongYu_init;
    *update = HongYu_update;
    *exit = HongYu_exit;
}
