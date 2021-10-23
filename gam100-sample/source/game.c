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

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "SystemManager.h"
#include "Colors.h"

void gameUI_render();

int is_btn_colliding(float x, float y, float buttonX, float buttonY) {
    return x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
        y >= buttonY && y <= buttonY + BUTTON_HEIGHT;
}

int game_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag,"adrian") == 0)
            SceneManager_ChangeSceneByName("adrian");
        else if (strcmp(((GameObject*)left->obj)->tag, "marcus") == 0)
            SceneManager_ChangeSceneByName("marcus");
        else if (strcmp(((GameObject*)left->obj)->tag, "hongyu") == 0)
            SceneManager_ChangeSceneByName("hongyu");
        else if (strcmp(((GameObject*)left->obj)->tag, "weiyi") == 0)
            SceneManager_ChangeSceneByName("weiyi");
        else if (strcmp(((GameObject*)left->obj)->tag, "xinyun") == 0)
            SceneManager_ChangeSceneByName("xinyun");
    }
    printf("INSIDE!");
    return CLM_RESPONSE_REMOVENONE;
}

void game_init(void)
{
    SM_SystemsInit();

    GameObject* g = GOM_Create(CIRCLE, PRI_GAME_OBJECT);
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(20, 20);

    GameObject* button = GOM_Create(RECTANGLE, PRI_UI);
    button->scale = CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT);
    button->position = CP_Vector_Set(80.0f, 25.0f);
    button->tag = "marcus"; //For collision
    button->color = CP_Color_Create(255, 0, 0, 50);
    CLM_AddCollider(button, game_OnCollision, COL_BOX, BUTTON_WIDTH, BUTTON_HEIGHT);

    button = GOM_Create(RECTANGLE, PRI_UI);
    button->scale = CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT);
    button->position = CP_Vector_Set(80.0f, 75.0f);
    button->tag = "hongyu";
    button->color = CP_Color_Create(255, 0, 0, 50);
    CLM_AddCollider(button, game_OnCollision, COL_BOX, BUTTON_WIDTH, BUTTON_HEIGHT);

    button = GOM_Create(RECTANGLE, PRI_UI);
    button->scale = CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT);
    button->position = CP_Vector_Set(80.0f, 125.0f);
    button->tag = "adrian";
    button->color = CP_Color_Create(255, 0, 0, 50);
    CLM_AddCollider(button, game_OnCollision, COL_BOX, BUTTON_WIDTH, BUTTON_HEIGHT);

    button = GOM_Create(RECTANGLE, PRI_UI);
    button->scale = CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT);
    button->position = CP_Vector_Set(80.0f, 175.0f);
    button->tag = "weiyi";
    button->color = CP_Color_Create(255, 0, 0, 50);
    CLM_AddCollider(button, game_OnCollision, COL_BOX, BUTTON_WIDTH, BUTTON_HEIGHT);

    button = GOM_Create(RECTANGLE, PRI_UI);
    button->scale = CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT);
    button->position = CP_Vector_Set(80.0f, 225.0f);
    button->tag = "xinyun";
    button->color = CP_Color_Create(255, 0, 0, 50);
    CLM_AddCollider(button, game_OnCollision, COL_BOX, BUTTON_WIDTH, BUTTON_HEIGHT);
}

void game_update(void)
{
    SM_SystemsPreUpdate();
    if (CP_Input_KeyTriggered(KEY_1))
    {
        // Change to game2 scene.
        SceneManager_ChangeSceneByName("game2");
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

        //if (is_btn_colliding(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 50.f, 10.f))
        //{
        //    SceneManager_ChangeSceneByName("marcus");
        //}

        //else if (is_btn_colliding(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 50.f, 60.f))
        //{
        //    SceneManager_ChangeSceneByName("hongyu");
        //}

        //else if (is_btn_colliding(CP_Input_GetMouseX(), CP_Input_GetMouseY(), 50.f, 110.f))
        //{
        //    SceneManager_ChangeSceneByName("adrian");
        //}
    }
    SM_SystemsUpdate();

    gameUI_render();
    SM_SystemsLateUpdate();

    //RM_Render();
}

void game_exit(void)
{
    SM_SystemsExit();
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

    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    CP_Graphics_DrawRect(50.f, 160.f, BUTTON_WIDTH, BUTTON_HEIGHT);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    CP_Font_DrawText("Weiyi", 55, 180);

    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    CP_Graphics_DrawRect(50.f, 210.f, BUTTON_WIDTH, BUTTON_HEIGHT);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    CP_Font_DrawText("Xinyun", 55, 230);
}