/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file	    gameEndScene.c 
@author	    Tan Wee Yi (weeyi.t)
@course	    CSD1400 Software Engineering Project 1
@Team	    BananaBoi
@brief	    Scene that will be at the end of the game / when the player loses.
*//*________________________________________________________________________________________*/

#define BUTTON_WIDTH 230.f
#define BUTTON_HEIGHT 50.f

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "SystemManager.h"
#include "Colors.h"
#include "SoundManager.h"

#define GAMEEND_MAXBUTTONS 2
Collider* gameEnd_buttons[GAMEEND_MAXBUTTONS];

/// <summary>
/// Handles the collision inside of the game scene
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void gameEnd_OnCollision(Collider* left, Collider* right) {

    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "restart") == 0)
        {
            SceneManager_ChangeSceneByName("levelone");
            SDM_ChangeBgMusic(2);
            SDM_PlayBgMusic(3);
            SDM_PlaySFX(4);
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "exit") == 0) {
            SceneManager_ChangeSceneByName("mainmenu");
            SDM_ChangeBgMusic(1);
            SDM_PlaySFX(4);
        }

    }
}

/// <summary>
/// Inits the scene with buttons and also the colliders for them.
/// </summary>
/// <param name=""></param>
void gameEnd_init(void)
{
    float xScale = CP_System_GetWindowWidth() / 100.0f, yScale = CP_System_GetWindowHeight()/100.0f;
    SM_SystemsInit();

    Collider* gEnd = NULL;
    Renderer* rEnd = NULL;

    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    GameObject* bg = GOM_Create2(RECTANGLE,
        CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* bgr = RM_AddComponent(bg);
    bgr->renderPriority = PRI_UI;
    RM_LoadImage(bgr, "Assets/scenes/Game_Over.png");


    GameObject* eButton = GOM_Create2(RECTANGLE, CP_Vector_Set(50 * xScale, 90 * yScale), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    eButton->tag = "exit";
    gEnd = CLM_AddComponent(eButton);
    CLM_Set(gEnd, COL_BOX, gameEnd_OnCollision);
    gEnd->space = COLSPC_SCREEN;
    rEnd = RM_AddComponent(eButton);
    rEnd->renderPriority = PRI_UI;
    RM_LoadImage(rEnd, "Assets/Backgrounds/button-light.png");
    rEnd->textColor = COLOR_LIGHTYELLOW;
    rEnd->textScale = CP_Vector_Set(1.5f, 1.5f);
    RM_SetText(rEnd, "Return to Main Menu");
    gameEnd_buttons[0] = gEnd;

    GameObject* rButton = GOM_Create2(RECTANGLE, CP_Vector_Set(50 * xScale, 80 * yScale), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    rButton->tag = "restart";
    gEnd = CLM_AddComponent(rButton);
    CLM_Set(gEnd, COL_BOX, gameEnd_OnCollision);
    gEnd->space = COLSPC_SCREEN;
    rEnd = RM_AddComponent(rButton);
    rEnd->renderPriority = PRI_UI;
    RM_LoadImage(rEnd, "Assets/Backgrounds/button-light.png");
    rEnd->textColor = COLOR_LIGHTYELLOW;
    rEnd->textScale = CP_Vector_Set(1.5f, 1.5f);
    RM_SetText(rEnd, "Restart Game");
    gameEnd_buttons[1] = gEnd;

    SDM_PlayBgMusic(1);

}

/// <summary>
/// Updates the game scene to check for when there is a mouse press and also handles the drawing of
/// the buttons.
/// </summary>
/// <param name=""></param>
void gameEnd_update(void)
{
    SM_SystemsPreUpdate();

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

    for (int i = 0; i < GAMEEND_MAXBUTTONS; i++)
    {
        GameObject* button = gameEnd_buttons[i]->obj;
        CP_Vector spd = CP_Vector_Set(BUTTON_WIDTH * 1.7f, BUTTON_HEIGHT * 1.7f);
        CP_Vector maxv = CP_Vector_Set(BUTTON_WIDTH * 1.3f, BUTTON_HEIGHT * 1.2f);
        CP_Vector def = CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT);
        if (IsBoxCollidePoint(gameEnd_buttons[i], c))
        {
            button->scale.x += spd.x * CP_System_GetDt();
            button->scale.y += spd.y * CP_System_GetDt();
            button->scale.x = min(button->scale.x, maxv.x);
            button->scale.y = min(button->scale.y, maxv.y);
        }
        else
        {
            //scale down
            button->scale.x -= spd.x * CP_System_GetDt();
            button->scale.y -= spd.y * CP_System_GetDt();
            button->scale.x = max(button->scale.x, def.x);
            button->scale.y = max(button->scale.y, def.y);
        }
    }
    


    SM_SystemsUpdate(0);

    SM_SystemsLateUpdate();
}

/// <summary>
/// Exits the scene.
/// </summary>
/// <param name=""></param>
void gameEnd_exit(void)
{
    SM_SystemsExit();
}

/// <summary>
/// Inits the scene for the scenemanager so that it can 
/// </summary>
/// <param name="init"></param>
/// <param name="update"></param>
/// <param name="exit"></param>
void gameEnd_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = gameEnd_init;
    *update = gameEnd_update;
    *exit = gameEnd_exit;
}


