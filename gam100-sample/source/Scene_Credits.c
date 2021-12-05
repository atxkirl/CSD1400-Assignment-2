/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Scene_Credits.c
@author     Koh Xin Yun (xinyun.k)
@course     CSD1400 Software Engineering Project 1
@Team       BananaBoi
@brief      Scene that displays the credits
*//*________________________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "SystemManager.h"

GameObject* credits, *credits2, *credits_cross, *credits_right, *credits_left;
Renderer* credits_render, * credits1_render, * credits2_render, * credits_cross_highlight;
Renderer* credits_rightbtn, * credits_rightbtn_highlight, * credits_leftbtn, * credits_leftbtn_highlight;

int on_page1 = 0;
int on_page2 = 0;

/// <summary>
/// Handles the collision of the the GameObjects based on the left and right collider parameters.
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void SceneCredits_OnCollision(Collider* left, Collider* right)
{
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        // close scene
        if (strcmp(((GameObject*)left->obj)->tag, "exit") == 0)
            SceneManager_ChangeSceneByName("mainmenu");

        // to page 2
        if (strcmp(((GameObject*)left->obj)->tag, "page2") == 0) {
            credits2_render->isEnabled = true;
            credits1_render->isEnabled = false;
            credits_rightbtn->color = CP_Color_Create(0, 0, 0, 0);
            credits_leftbtn->color = CP_Color_Create(0, 0, 0, 255);

            on_page2 = 1;
            on_page1 = 0;
        }

        // back to page 1
        if (strcmp(((GameObject*)left->obj)->tag, "page1") == 0) {
            credits2_render->isEnabled = false;
            credits1_render->isEnabled = true;
            credits_rightbtn->color = CP_Color_Create(0, 0, 0, 255);
            credits_leftbtn->color = CP_Color_Create(0, 0, 0, 0);

            on_page1 = 1;
            on_page2 = 0;
        }
    }

    if (strcmp(((GameObject*)right->obj)->tag, "Mouse") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "exit") == 0)
        {
            credits_cross_highlight->isEnabled = true;
        }

        if (strcmp(((GameObject*)left->obj)->tag, "page1") == 0) {
            credits_leftbtn_highlight->isEnabled = true;
        }

        if (strcmp(((GameObject*)left->obj)->tag, "page2") == 0) {
            credits_rightbtn_highlight->isEnabled = true;
        }
    }
}

/// <summary>
/// Initialises variables in this scene.
/// </summary>
void SceneCredits_init(void)
{
    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);

    GameObject* bg = GOM_Create2(RECTANGLE,
        CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* bgr = RM_AddComponent(bg);
    bgr->renderPriority = PRI_UI;
    RM_LoadImage(bgr, "Assets/BananaBoi_Title.jpg");

    // render credits scene
    credits = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    credits->tag = "credits";
    credits1_render = RM_AddComponent(credits);
    credits1_render->renderPriority = PRI_UI;
    credits1_render->isEnabled = true;
    RM_LoadImage(credits1_render, "Assets/scenes/credits.png");

    // render credits page 2
    credits2 = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    credits2->tag = "credits";
    credits2_render = RM_AddComponent(credits2);
    credits2_render->renderPriority = PRI_UI;
    credits2_render->isEnabled = false;
    RM_LoadImage(credits2_render, "Assets/scenes/credits2.png");

    // close button
    credits_cross = GOM_Create(RECTANGLE);
    credits_cross->position = CP_Vector_Set(screenWidth * 0.975f, screenHeight * 0.045f);
    credits_cross->scale = CP_Vector_Set(50, 50);
    credits_cross->tag = "exit";
    Collider* credits_exit = CLM_AddComponent(credits_cross);
    CLM_Set(credits_exit, COL_BOX, SceneCredits_OnCollision);
    credits_exit->space = COLSPC_SCREEN;
    credits_render = RM_AddComponent(credits_cross);
    credits_render->renderPriority = PRI_UI;
    RM_LoadImage(credits_render, "Assets/cross.png");
    credits_cross_highlight = RM_AddComponent(credits_cross);
    credits_cross_highlight->renderPriority = PRI_UI;
    RM_LoadImage(credits_cross_highlight, "Assets/crosshighlight.png");

    // right arrow
    credits_right = GOM_Create(RECTANGLE);
    credits_right->position = CP_Vector_Set(screenWidth * 0.92f, screenHeight * 0.5f);
    credits_right->scale = CP_Vector_Set(50, 50);
    credits_right->tag = "page2";
    Collider* credits_rightarrow = CLM_AddComponent(credits_right);
    CLM_Set(credits_rightarrow, COL_BOX, SceneCredits_OnCollision);
    credits_rightarrow->space = COLSPC_SCREEN;
    credits_rightbtn = RM_AddComponent(credits_right);
    credits_rightbtn->renderPriority = PRI_UI;
    RM_LoadImage(credits_rightbtn, "Assets/arrow.png");
    credits_rightbtn_highlight = RM_AddComponent(credits_right);
    credits_rightbtn_highlight->renderPriority = PRI_UI;
    RM_LoadImage(credits_rightbtn_highlight, "Assets/arrowhighlight.png");

    // left arrow
    credits_left = GOM_Create(RECTANGLE);
    credits_left->position = CP_Vector_Set(screenWidth * 0.08f, screenHeight * 0.5f);
    credits_left->scale = CP_Vector_Set(50, 50);
    credits_left->tag = "page1";
    Collider* credits_leftarrow = CLM_AddComponent(credits_left);
    CLM_Set(credits_leftarrow, COL_BOX, SceneCredits_OnCollision);
    credits_leftarrow->space = COLSPC_SCREEN;
    credits_leftbtn = RM_AddComponent(credits_left);
    credits_leftbtn->renderPriority = PRI_UI;
    RM_LoadImage(credits_leftbtn, "Assets/arrow.png");
    credits_leftbtn->isXFlipped = 1;
    credits_leftbtn->color = CP_Color_Create(0, 0, 0, 0);
    credits_leftbtn_highlight = RM_AddComponent(credits_left);
    credits_leftbtn_highlight->renderPriority = PRI_UI;
    RM_LoadImage(credits_leftbtn_highlight, "Assets/arrowhighlight.png");
    credits_leftbtn_highlight->isXFlipped = 1;
 
    SM_SystemsInit();

    
}

/// <summary>
/// Update.
/// </summary>
void SceneCredits_update(void)
{
    credits_cross_highlight->isEnabled = false;
    credits_rightbtn_highlight->isEnabled = false;
    credits_leftbtn_highlight->isEnabled = false;

    if (on_page1 == 1) {
        credits_leftbtn_highlight->color = CP_Color_Create(0, 0, 0, 0);
    }
    else {
        credits_leftbtn_highlight->color = CP_Color_Create(0, 0, 0, 255);
    }
    if (on_page2 == 1) {
        credits_rightbtn_highlight->color = CP_Color_Create(0, 0, 0, 0);
    }
    else {
        credits_rightbtn_highlight->color = CP_Color_Create(0, 0, 0, 255);
    }

    SM_SystemsPreUpdate();
    SM_SystemsUpdate(0);
    SM_SystemsLateUpdate();
}

/// <summary>
/// Exit function that's called before scene change.
/// </summary>
void SceneCredits_exit(void)
{
    SM_SystemsExit();
}

/// <summary>
/// Called by SceneManager to pass function pointers for this scene's init, update and exit.
/// </summary>
/// <param name="init">Function pointer to scene init.</param>
/// <param name="update">Function pointer to scene update.</param>
/// <param name="exit">Funciton pointer to scene exit.</param>
void SceneCredits_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = SceneCredits_init;
    *update = SceneCredits_update;
    *exit = SceneCredits_exit;
}
