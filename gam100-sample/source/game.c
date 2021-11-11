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

void game_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag,"adrian") == 0)
            SceneManager_ChangeSceneByName("leveloneastar");
        else if (strcmp(((GameObject*)left->obj)->tag, "marcus") == 0)
            SceneManager_ChangeSceneByName("marcus");
        else if (strcmp(((GameObject*)left->obj)->tag, "hongyu") == 0)
            SceneManager_ChangeSceneByName("hongyu");
        else if (strcmp(((GameObject*)left->obj)->tag, "weiyi") == 0)
            SceneManager_ChangeSceneByName("weiyi");
        else if (strcmp(((GameObject*)left->obj)->tag, "xinyun") == 0)
            SceneManager_ChangeSceneByName("xinyun");
        else if (strcmp(((GameObject*)left->obj)->tag, "game") == 0)
            SceneManager_ChangeSceneByName("levelone");
    }
    printf("INSIDE!");
}

void game_init(void)
{
    SM_SystemsInit();
    //GameObject* g = 
    GOM_Create2(CIRCLE,
        CP_Vector_Set(20, 20), 0.0f, CP_Vector_Set(20, 20));
    Collider* c = NULL;

    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    GameObject* bg = GOM_Create2(RECTANGLE,
        CP_Vector_Set(0.5f * screenWidth, 0.5f* screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* bgr = RM_AddComponent(bg);
    bgr->renderPriority = PRI_UI;
    RM_LoadImage(bgr, "Assets/BananaBoi_Title.jpg");


    GameObject* button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 25.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    Renderer* r = RM_AddComponent(button);
    button->tag = "marcus"; //For collision
    r->renderPriority = PRI_UI;
    r->text = "marcus";
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 75.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "hongyu";
    r->renderPriority = PRI_UI;
    r->text = "hongyu";
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 125.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "adrian";
    r->renderPriority = PRI_UI;
    r->text = "adrian";
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 175.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "weiyi";
    r->renderPriority = PRI_UI;
    r->text = "weiyi";
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 225.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "xinyun";
    r->renderPriority = PRI_UI;
    r->text = "xinyun";
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    //START BUTTON ==========
    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(0.5f*screenWidth, 450), 0.0f, CP_Vector_Set(100.0f, 40.0f));
    r = RM_AddComponent(button);
    button->tag = "game"; //For collision
    r->renderPriority = PRI_UI;
    r->text = "Start";
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;
}

void game_update(void)
{
    SM_SystemsPreUpdate();
    //if (CP_Input_KeyTriggered(KEY_1))
    //{
    //    // Change to game2 scene.
    //    SceneManager_ChangeSceneByName("game2");
    //}
    SM_SystemsUpdate();

    //gameUI_render();
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