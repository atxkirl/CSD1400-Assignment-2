/*!
@file       game.c
@author		Ow Hong Yu (ow.h)
@course		CSD 1400
@section	A
@brief		This is a main menu scenes. It contains all the UI in main menu
*//*______________________________________________________________________*/

#define BUTTON_WIDTH 60.f
#define BUTTON_HEIGHT 30.f

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "SystemManager.h"
#include "Colors.h"
#include "Controls.h"
#include "SoundManager.h"


//Animation* MainMenuStartAnim1, *MainMenuStartAnim2;
Collider* MainMenuStartCollider, *MainMenuOptionsCollider, *MainMenuCreditsCollider;
#define MainMenuStartDefaultScale CP_Vector_Set(200.0f, 50.0f)
#define MainMenuStartMaxScale CP_Vector_Set(300.0f, 75.0f)
#define MainMenuStartScaleSpd CP_Vector_Set(500.0f, 350.0f)

#define MainMenuOptsCredsDefaultScale CP_Vector_Set(150.0f, 40.0f)
#define MainMenuOptsCredsMaxScale CP_Vector_Set(180.0f, 50.0f)
#define MainMenuOptsCredsScaleSpd CP_Vector_Set(150.0f, 150.0f)

#define buttonCount 4
GameObject* buttons[buttonCount]; // Array holding all the buttons for main menu. "Start", "How to Play", "Options", "Credits"

/// <summary>
/// renders the game ui using cp processing
/// </summary>
void gameUI_render();

/// <summary>
/// Checks if point x,y is inside button
/// </summary>
/// <param name="x">- x value of point</param>
/// <param name="y">- y value of point</param>
/// <param name="buttonX">- x pos of button</param>
/// <param name="buttonY">- y pos of button</param>
/// <returns>int - true or false if is collide</returns>
int is_btn_colliding(float x, float y, float buttonX, float buttonY) {
    return x >= buttonX && x <= buttonX + BUTTON_WIDTH &&
        y >= buttonY && y <= buttonY + BUTTON_HEIGHT;
}

/// <summary>
/// Main menu collision response function
/// </summary>
/// <param name="left">- the collider of the owner of this response</param>
/// <param name="right">- the collider of the other object</param>
void game_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag,"adrian") == 0)
            SceneManager_ChangeSceneByName("leveloneastar");
        else if (strcmp(((GameObject*)left->obj)->tag, "astartest") == 0)
            SceneManager_ChangeSceneByName("adrian");
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
        else if (strcmp(((GameObject*)left->obj)->tag, "options") == 0)
            SceneManager_ChangeSceneByName("options");
        else if (strcmp(((GameObject*)left->obj)->tag, "credits") == 0)
            SceneManager_ChangeSceneByName("credits");
        else if (strcmp(((GameObject*)left->obj)->tag, "howtoplay") == 0) //TODO
            SceneManager_ChangeSceneByName("credits");
    }

}

/// <summary>
/// Inits the main menu
/// </summary>
void game_init(void)
{
    SM_SystemsInit();
    //GameObject* g = 
    GOM_Create2(CIRCLE,
        CP_Vector_Set(20, 20), 0.0f, CP_Vector_Set(20, 20));

    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    GameObject* bg = GOM_Create2(RECTANGLE,CP_Vector_Set(0.5f * screenWidth, 0.5f* screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* bgr = RM_AddComponent(bg);
    bgr->renderPriority = PRI_UI;
    RM_LoadImage(bgr, "Assets/BananaBoi_Title.jpg");

    GameObject* button;
    Renderer* r;

#if _DEBUG
    button = GOM_Create2(RECTANGLE, CP_Vector_Set(80.0f, 25.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "marcus"; //For collision
    r->renderPriority = PRI_UI;
    //r->text = "marcus";
    RM_SetText(r, "marcus");
    Collider* c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 75.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "hongyu";
    r->renderPriority = PRI_UI;
    RM_SetText(r, "hongyu");
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 125.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "adrian";
    r->renderPriority = PRI_UI;
    RM_SetText(r, "adrian");
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 175.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "astartest";
    r->renderPriority = PRI_UI;
    RM_SetText(r, "A* Test");
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 225.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "weiyi";
    r->renderPriority = PRI_UI;
    RM_SetText(r, "weiyi");
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

    button = GOM_Create2(RECTANGLE,
        CP_Vector_Set(80.0f, 275.0f), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    r = RM_AddComponent(button);
    button->tag = "xinyun";
    r->renderPriority = PRI_UI;
    RM_SetText(r, "xinyun");
    c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, game_OnCollision);
    c->space = COLSPC_SCREEN;

#endif

    float centerButtonX = 0.5f * screenWidth;
    float centerButtonY = 0.85f * screenHeight;

    //START BUTTON ==========
    button = GOM_Create2(RECTANGLE, CP_Vector_Set(centerButtonX, centerButtonY), 0.0f, MainMenuStartDefaultScale);
    button->tag = "game"; //For collision
    r = RM_AddComponent(button);
    r->renderPriority = PRI_UI;
    RM_LoadImage(r, "Assets/Backgrounds/button-light.png");
    RM_SetText(r, "Start");
    r->textColor = COLOR_LIGHTYELLOW;
    r->textScale = CP_Vector_Set(2, 2);
    MainMenuStartCollider = CLM_AddComponent(button);
    CLM_Set(MainMenuStartCollider, COL_BOX, game_OnCollision);
    MainMenuStartCollider->space = COLSPC_SCREEN;
    MainMenuStartCollider->isTrigger = 1;

    button = GOM_Create2(RECTANGLE, CP_Vector_Set(centerButtonX + (-1.25f * MainMenuStartDefaultScale.x), centerButtonY), 0.0f, MainMenuOptsCredsDefaultScale);
    button->tag = "options"; //For collision
    r = RM_AddComponent(button);
    r->renderPriority = PRI_UI;
    RM_LoadImage(r, "Assets/Backgrounds/button-light.png");
    RM_SetText(r, "Options");
    r->textColor = COLOR_LIGHTYELLOW;
    r->textScale = CP_Vector_Set(1.8f, 1.8f);
    MainMenuOptionsCollider = CLM_AddComponent(button);
    CLM_Set(MainMenuOptionsCollider, COL_BOX, game_OnCollision);
    MainMenuOptionsCollider->space = COLSPC_SCREEN;
    MainMenuOptionsCollider->isTrigger = 1;

    button = GOM_Create2(RECTANGLE, CP_Vector_Set(centerButtonX + (1.25f * MainMenuStartDefaultScale.x), centerButtonY), 0.0f, MainMenuOptsCredsDefaultScale);
    button->tag = "credits"; //For collision
    r = RM_AddComponent(button);
    r->renderPriority = PRI_UI;
    RM_LoadImage(r, "Assets/Backgrounds/button-light.png");
    RM_SetText(r, "Credits");
    r->textColor = COLOR_LIGHTYELLOW;
    r->textScale = CP_Vector_Set(1.8f, 1.8f);
    MainMenuCreditsCollider = CLM_AddComponent(button);
    CLM_Set(MainMenuCreditsCollider, COL_BOX, game_OnCollision);
    MainMenuCreditsCollider->space = COLSPC_SCREEN;
    MainMenuCreditsCollider->isTrigger = 1;
}
/// <summary>
/// updates the main menu every frame
/// </summary>
void game_update(void)
{
    SM_SystemsPreUpdate();
    //if (CP_Input_KeyTriggered(KEY_1))
    //{
    //    // Change to game2 scene.
    //    SceneManager_ChangeSceneByName("game2");
    //}

    //MainMenuStartAnim1->isEnabled = 0;
    //MainMenuStartAnim2->isEnabled = 1;

    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    float mouseX = CP_Input_GetMouseX();
    float mouseY = CP_Input_GetMouseY();

    GameObject* tempMouse = GOM_CreateTemp(EMPTY);
    tempMouse->position = CP_Vector_Set(mouseX, mouseY);
    //tempMouse->scale = CP_Vector_Set(10, 10);
    tempMouse->tag = "Mouse";
    Collider* c = CLM_AddComponent(tempMouse);
    CLM_Set(c, COL_POINT, NULL);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;

    GameObject* button = (GameObject*)MainMenuStartCollider->obj;
    //Renderer* renderer = RM_GetComponent(button);
    if (IsBoxCollidePoint(MainMenuStartCollider, c))
    {
        button->scale.x += MainMenuStartScaleSpd.x * CP_System_GetDt();
        button->scale.y += MainMenuStartScaleSpd.y * CP_System_GetDt();
        button->scale.x = min(button->scale.x, MainMenuStartMaxScale.x);
        button->scale.y = min(button->scale.y, MainMenuStartMaxScale.y);
    }
    else
    {
        //scale down
        button->scale.x -= MainMenuStartScaleSpd.x * CP_System_GetDt();
        button->scale.y -= MainMenuStartScaleSpd.y * CP_System_GetDt();
        button->scale.x = max(button->scale.x, MainMenuStartDefaultScale.x);
        button->scale.y = max(button->scale.y, MainMenuStartDefaultScale.y);
    }

    button = (GameObject*)MainMenuOptionsCollider->obj;
    if (IsBoxCollidePoint(MainMenuOptionsCollider, c))
    {
        button->scale.x += MainMenuOptsCredsScaleSpd.x * CP_System_GetDt();
        button->scale.y += MainMenuOptsCredsScaleSpd.y * CP_System_GetDt();
        button->scale.x = min(button->scale.x, MainMenuOptsCredsMaxScale.x);
        button->scale.y = min(button->scale.y, MainMenuOptsCredsMaxScale.y);
    }
    else
    {
        button->scale.x -= MainMenuOptsCredsScaleSpd.x * CP_System_GetDt();
        button->scale.y -= MainMenuOptsCredsScaleSpd.y * CP_System_GetDt();
        button->scale.x = max(button->scale.x, MainMenuOptsCredsDefaultScale.x);
        button->scale.y = max(button->scale.y, MainMenuOptsCredsDefaultScale.y);
    }

    button = (GameObject*)MainMenuCreditsCollider->obj;
    if (IsBoxCollidePoint(MainMenuCreditsCollider, c))
    {
        button->scale.x += MainMenuOptsCredsScaleSpd.x * CP_System_GetDt();
        button->scale.y += MainMenuOptsCredsScaleSpd.y * CP_System_GetDt();
        button->scale.x = min(button->scale.x, MainMenuOptsCredsMaxScale.x);
        button->scale.y = min(button->scale.y, MainMenuOptsCredsMaxScale.y);
    }
    else
    {
        button->scale.x -= MainMenuOptsCredsScaleSpd.x * CP_System_GetDt();
        button->scale.y -= MainMenuOptsCredsScaleSpd.y * CP_System_GetDt();
        button->scale.x = max(button->scale.x, MainMenuOptsCredsDefaultScale.x);
        button->scale.y = max(button->scale.y, MainMenuOptsCredsDefaultScale.y);
    }

    SM_SystemsUpdate(0);

    //gameUI_render();
    SM_SystemsLateUpdate();

    //RM_Render();
}
/// <summary>
/// exits the main menu
/// </summary>
void game_exit(void)
{
    SM_SystemsExit();
    //SDM_StopAll();
    //SDM_FreeSounds();
}
/// <summary>
/// assign function pointers of main menu's init, update and exit to scene manager to call
/// </summary>
/// <param name="init">- pointer to assign main menu init</param>
/// <param name="update">- pointer to assign main menu update</param>
/// <param name="exit">- pointer to assign main menu exit</param>
void game_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = game_init;
    *update = game_update;
    *exit = game_exit;
}

/// <summary>
/// renders the game ui using cp processing
/// </summary>
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