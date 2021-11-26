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
#include "SoundManager.h"

static float logoFadeSpeed = 250.f;
static float logoWaitTimer = 1.5f;
static float elapsedTime = 0.f;

static int digipenState;
static int bananaState;

// Digipen Logo
static GameObject* digipenObj;
static Renderer* digipenRenderer;
// Team Logo
static GameObject* bananaObj;
static Renderer* bananaRenderer;

void update_digipen(void)
{
    if (digipenState == 3)
        return;

    if (digipenState == 1)
    {
        elapsedTime += CP_System_GetDt();
        if (elapsedTime > logoWaitTimer)
        {
            digipenState = 2;
        }
        else
        {
            return;
        }
    }

    if(digipenState != 1)
        digipenRenderer->color.a += (char)(CP_System_GetDt() * logoFadeSpeed); // Slowly increment alpha.

    if (digipenRenderer->color.a > 250 && digipenState == 0)
    {
        logoFadeSpeed *= -1;
        digipenState = 1;
    }
    if (digipenRenderer->color.a < 5 && digipenState == 2)
    {
        digipenRenderer->color.a = 0;
        digipenState = 3;

        elapsedTime = 0.f;
        logoFadeSpeed *= -1;
    }
}

void update_bananaboi(void)
{
    if (bananaState == 3)
        return;

    if (bananaState == 1)
    {
        elapsedTime += CP_System_GetDt();
        if (elapsedTime > logoWaitTimer)
        {
            bananaState = 2;
        }
        else
        {
            return;
        }
    }

    if (bananaState != 1)
        bananaRenderer->color.a += (char)(CP_System_GetDt() * logoFadeSpeed); // Slowly increment alpha.

    if (bananaRenderer->color.a > 250 && bananaState == 0)
    {
        logoFadeSpeed *= -1;
        bananaState = 1;
    }
    if (bananaRenderer->color.a < 5 && bananaState == 2)
    {
        bananaRenderer->color.a = 0;
        bananaState = 3;

        elapsedTime = 0.f;
        logoFadeSpeed *= -1;
    }
}

void splashscreen_init(void)
{
    SM_SystemsInit();
    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);

    GameObject* bg = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* bgRenderer = RM_AddComponent(bg);
    bgRenderer->renderPriority = PRI_UI;
    bgRenderer->color = CP_Color_Create(0, 0, 0, 255);
    
    // Create Digipen Logo
    digipenObj = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.4f));
    digipenRenderer = RM_AddComponent(digipenObj);
    digipenRenderer->renderPriority = PRI_UI;
    digipenRenderer->color = CP_Color_Create(0, 0, 0, 0); // Start off as transparent.
    RM_LoadImage(digipenRenderer, "Assets/Logo_Digipen.png");

    // Create BananaBoi Logo
    bananaObj = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.35f));
    bananaRenderer = RM_AddComponent(bananaObj);
    bananaRenderer->renderPriority = PRI_UI;
    bananaRenderer->color = CP_Color_Create(0, 0, 0, 0); // Start off as transparent.
    RM_LoadImage(bananaRenderer, "Assets/Logo_BananaBoi.png");

    // Initialize variables
    digipenState = 0;
    bananaState = 0;
    elapsedTime = 0.f;
}

void splashscreen_update(void)
{
    if (CP_Input_KeyTriggered(KEY_ESCAPE))
    {
        SceneManager_ChangeSceneByName("mainmenu");
    }

    if (digipenState == 3 && bananaState == 3)
    {
        SceneManager_ChangeSceneByName("mainmenu");
    }
    else if (digipenState != 3)
    {
        update_digipen();
    }
    else
    {
        update_bananaboi();
    }

    SM_SystemsPreUpdate();
    SM_SystemsUpdate(0);
    SM_SystemsLateUpdate();
}

void splashscreen_exit(void)
{
    SM_SystemsExit();

    SDM_Init();
    SDM_PlayBgMusic(1);
}

void splashscreen_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = splashscreen_init;
    *update = splashscreen_update;
    *exit = splashscreen_exit;
}