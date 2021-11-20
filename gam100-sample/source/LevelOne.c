/*!
@file LevelOne.c
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains functions of game init, update, exit
*//*______________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "LevelEditor.h"
#include "FileParser.h"
#include "Loader.h"
#include "Colors.h"
#include "SystemManager.h"
#include "Objectives.h"
#include "Objective_Connect.h"
#include "Player.h"

GameObject* gLOne = NULL;

float screenWidth, screenHeight;
void LevelOneUI_render();
void LevelOneGridColliderInit();

void LevelOne_OnCollision(Collider* left, Collider* right)
{

}

void LevelOne_init(void)
{
    SM_SystemsInit();
    //RM_AddComponent(g);
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    LoaderInit();

    //Insert spawn x,y here
    PLY_CreatePlayer(90.f, 90.f);

    //load level and objectives
    LoadGrid("Level01", 0);
    Objectives_Init(screenWidth, screenHeight);
    Objectives_RenderUI();

    gLOne = GOM_Create2(RECTANGLE, SetPlayerPosition(), 0.0f, CP_Vector_Set(50, 50));
    gLOne->tag = "player";
    Renderer *r = RM_AddComponent(gLOne);
    RM_LoadImage(r, "Assets/bananaboi.png");
    CLM_Set(CLM_AddComponent(gLOne), COL_BOX, LevelOne_OnCollision);
    LevelOneGridColliderInit();
}

void LevelOne_update(void)
{
    SM_SystemsPreUpdate();

    float dt = CP_System_GetDt();
    float spd = 200.0f;
    //Collider* gc = CLM_GetComponent(g);
    //gc->velocity = CP_Vector_Set(0, 0);

    if (Objectives_GetPlayerUpdate())
    {
        if (CP_Input_KeyDown((enum CP_KEY)KEY_W))
        {
            gLOne->position.y -= spd * dt;
        }
        if (CP_Input_KeyDown((enum CP_KEY)KEY_S))
        {
            gLOne->position.y += spd * dt;
        }
        if (CP_Input_KeyDown((enum CP_KEY)KEY_A))
        {
            gLOne->position.x -= spd * dt;
        }
        if (CP_Input_KeyDown((enum CP_KEY)KEY_D))
        {
            gLOne->position.x += spd * dt;
        }
    }
    Objectives_Update();
    PLY_Update();

    SM_SystemsUpdate();

    RM_SetCameraPosition(gLOne->position);
    SM_SystemsLateUpdate();

    //CP_Graphics_ClearBackground(COLOR_BLUE);
    LevelOneUI_render();
    //LoaderUpdate();
}

void LevelOne_exit(void)
{
    LoaderExit();
    SM_SystemsExit();
}

void LevelOne_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = LevelOne_init;
    *update = LevelOne_update;
    *exit = LevelOne_exit;
}

void LevelOneUI_render()
{
    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    //CP_Graphics_DrawRect(50.f, 170.f, 75.f, 50.f);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    //CP_Font_DrawText("Objective", 55, 200);
}

void LevelOneGridColliderInit()
{
    for (int i = 0; i < NumGrids; i++)
    {
        for (int j = 0; j < NumGrids; j++)
        {
            if (gLoadedGrids->gGrid[i][j]->type == WATER || gLoadedGrids->gGrid[i][j]->type == EMPTY)
            {
                CLM_Set(CLM_GetComponent(gLoadedGrids->gGrid[i][j]), COL_BOX, LevelOne_OnCollision);
                CLM_GetComponent(gLoadedGrids->gGrid[i][j])->isLockedPos = 1;
            }
        }
    }
}
