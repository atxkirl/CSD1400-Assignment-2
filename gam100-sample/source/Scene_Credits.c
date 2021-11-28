#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "SystemManager.h"

GameObject* credits, *credits_cross;
Renderer* credits_render, *credits_cross_highlight;


void SceneCredits_OnCollision(Collider* left, Collider* right)
{
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "exit") == 0)
            SceneManager_ChangeSceneByName("mainmenu");
    }
    if (strcmp(((GameObject*)right->obj)->tag, "Mouse") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "exit") == 0)
        {
            credits_cross_highlight->isEnabled = true;
        }
    }
}

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
    credits_render = RM_AddComponent(credits);
    credits_render->renderPriority = PRI_UI;
    RM_LoadImage(credits_render, "Assets/scenes/credits.png");

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
 
    SM_SystemsInit();

    
}

void SceneCredits_update(void)
{
    credits_cross_highlight->isEnabled = false;

    SM_SystemsPreUpdate();
    SM_SystemsUpdate(0);
    SM_SystemsLateUpdate();
}

void SceneCredits_exit(void)
{
    SM_SystemsExit();
}

void SceneCredits_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = SceneCredits_init;
    *update = SceneCredits_update;
    *exit = SceneCredits_exit;
}
