/*!
@file game.c
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains functions of game init, update, exit
*//*______________________________________________________________________*/

#define BUTTON_WIDTH 60.f
#define BUTTON_HEIGHT 30.f
#define COLOR_RED CP_Color_Create(255, 0, 0, 255)
#define COLOR_GREEN CP_Color_Create(0, 255, 0, 255)
#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)
#define COLOR_BLACK CP_Color_Create(0, 0, 0, 255)
#define COLOR_WHITE CP_Color_Create(255, 255, 255, 255)

#include <stdio.h>
#include "cprocessing.h"
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Helpers.h"
#include "SceneManager.h"

void gameUI_render();

int is_btn_colliding(float x, float y, float buttonX, float buttonY) {
    return x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
        y >= buttonY && y <= buttonY + BUTTON_HEIGHT;
}

void game_init(void)
{
    RM_Init();
    GOM_Init();

    GameObject* g = GOM_CreateGameObject();
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(20, 20);

    RM_AddRenderObject(g);
}

void game_update(void)
{
    if (CP_Input_KeyTriggered(KEY_1))
    {
        // Change to game2 scene.
        SceneManager_ChangeSceneByName("game2");
        printf("wAD");
    }
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
        if (is_btn_colliding(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 50.f, 10.f))
        {
            SceneManager_ChangeSceneByName("marcus");
        }

        else if (is_btn_colliding(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 50.f, 60.f))
        {
            SceneManager_ChangeSceneByName("hongyu");
        }

        else if (is_btn_colliding(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 50.f, 110.f))
        {
            SceneManager_ChangeSceneByName("adrian");
        }
    }

    gameUI_render();
    RM_Render();
}

void game_exit(void)
{
    RM_ClearRenderObjects();
    GOM_Clear();
}

void game_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = game_init;
    *update = game_update;
    *exit = game_exit;
}

void gameUI_render()
{
    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    CP_Graphics_DrawRect(50.f, 10.f, BUTTON_WIDTH, BUTTON_HEIGHT);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    CP_Font_DrawText("Marcus", 55, 30);

    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    CP_Graphics_DrawRect(50.f, 60.f, BUTTON_WIDTH, BUTTON_HEIGHT);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    CP_Font_DrawText("Hongyu", 55, 80);

    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    CP_Graphics_DrawRect(50.f, 110.f, BUTTON_WIDTH, BUTTON_HEIGHT);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    CP_Font_DrawText("Adrian", 55, 130);
}