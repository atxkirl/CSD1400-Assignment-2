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

GameObject* g = NULL;

void HongYu_init(void)
{
    RM_Init();
    GOM_Init();

    g = GOM_CreateGameObject(CIRCLE, PRI_GAME_OBJECT);
    g->scale = CP_Vector_Set(10, 10);
    g->position = CP_Vector_Set(50, 20);

    int width = CP_System_GetWindowWidth();
    int height = CP_System_GetWindowHeight();

    GameObject* button = GOM_CreateGameObject(RECTANGLE, PRI_UI);
    button->position = CP_Vector_Set(0.1f * width, 0.9f * height);
    //button->rotation = 10.0f;
    button->scale = CP_Vector_Set(0.1f * width, 30);
    button->color = CP_Color_Create(255, 255, 255, 255);
    button->text = "test";
    

    //GameObject* wad = GOM_CreateGameObject(CIRCLE, GAME_OBJECT);
    //wad->position = CP_Vector_Set(0.1f * width, 0.9f * height);
    //wad->scale = CP_Vector_Set(0.1f * width, 30);
    //wad->color = CP_Color_Create(255, 255, 255, 255);
}

void HongYu_update(void)
{
    float dt = CP_System_GetDt();
    float spd = 200.0f;
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


    RM_Render();
}

void HongYu_exit(void)
{
    RM_ClearRenderObjects();
    GOM_Clear();
}

void HongYu_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = HongYu_init;
    *update = HongYu_update;
    *exit = HongYu_exit;
}
