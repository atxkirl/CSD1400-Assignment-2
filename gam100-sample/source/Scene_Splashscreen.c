/*!
@file game.c
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains functions of game init, update, exit
*//*______________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "SceneManager.h"
#include "SystemManager.h"
#include "Colors.h"

static float digipenFadeSpeed = 150.f;
static float digipenLogoTimer = 1.f;
static float elapsedTime = 0.f;
static int startCount = 0;

static GameObject* digipen;
static Renderer* digipenRenderer;

void splashscreen_init(void)
{
    SM_SystemsInit();

    // Create Digipen Logo
    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);

    GameObject* bg = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* bgRenderer = RM_AddComponent(bg);
    bgRenderer->renderPriority = PRI_UI;
    bgRenderer->color = CP_Color_Create(0, 0, 0, 255);

    digipen = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.25f));
    digipenRenderer = RM_AddComponent(digipen);
    digipenRenderer->renderPriority = PRI_UI;
    digipenRenderer->color = CP_Color_Create(0, 0, 0, 0); // Start off as transparent.
    RM_LoadImage(digipenRenderer, "Assets/Logo_Digipen.png");
}

void splashscreen_update(void)
{
    if (CP_Input_KeyTriggered(KEY_ESCAPE))
    {
        SceneManager_ChangeSceneByName("mainmenu");
    }

    if (startCount >= 0)
    {
        digipenRenderer->color.a += (char)(CP_System_GetDt() * digipenFadeSpeed); // Slowly increment alpha.
    }
    else if (startCount == -1)
    {
        elapsedTime += CP_System_GetDt();
        if (elapsedTime > digipenLogoTimer)
        {
            digipenFadeSpeed *= -1;
            startCount = 2;
        }
    }

    if (startCount == 0 && digipenRenderer->color.a >= 245)
    {
        startCount = -1;
    }
    else if (startCount == 2 && digipenRenderer->color.a <= 10)
    {
        SceneManager_ChangeSceneByName("mainmenu");
    }

    SM_SystemsPreUpdate();
    SM_SystemsUpdate();
    SM_SystemsLateUpdate();
}

void splashscreen_exit(void)
{
    SM_SystemsExit();
}

void splashscreen_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = splashscreen_init;
    *update = splashscreen_update;
    *exit = splashscreen_exit;
}