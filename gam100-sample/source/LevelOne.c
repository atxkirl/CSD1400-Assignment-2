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
#include "SceneManager.h"

//GameObject* gLOne = NULL;

float screenWidth, screenHeight;
void LevelOneUI_render();
void LevelOneGridColliderInit();

int IsGamePaused;
LinkedList* pauseMenus;
/*!
@brief This function initialises the pause menu
the function will return value 0x87654321.
@return void
*//*______________________________________________________________*/
void InitPause();
/*!
@brief Sets if pause is on or off
@param isPause - 1 to pause 0 to unpause
@return void
*//*______________________________________________________________*/
void SetPause(int isPause);
/*!
@brief This function checks if pauses is on
@return int - true or false of pause state
*//*______________________________________________________________*/
int IsPaused();
/*!
@brief This function clears memory used by pause menu
@return void
*//*______________________________________________________________*/
void ClearPause();

GameObject* GameFpsCounterObj;
/*!
@brief Initialises the fps counter gameobject
@return void
*//*______________________________________________________________*/
void InitDrawFPS();

void LevelOne_OnCollision(Collider* left, Collider* right)
{
    if (strcmp(right->obj->tag, ONCLICK_TAG) == 0)
    {
        if (strcmp(left->obj->tag, "PauseClose") == 0)
        {
            SetPause(0);
        }
        else if (strcmp(left->obj->tag, "ReturnToMainMenu") == 0)
        {
            SceneManager_ChangeSceneByName("mainmenu");
        }
    }
}

void LevelOne_init(void)
{
    SM_SystemsInit();
    //RM_AddComponent(g);
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    LoaderInit();

    //load level and objectives
    LoadGrid("Level01", 0);
    Objectives_Init(screenWidth, screenHeight);

    //gLOne = GOM_Create2(RECTANGLE, SetPlayerPosition(), 0.0f, CP_Vector_Set(50, 50));
    //gLOne->tag = "player";
    //Renderer *r = RM_AddComponent(gLOne);
    //RM_LoadImage(r, "Assets/bananaboi.png");
    //CLM_Set(CLM_AddComponent(gLOne), COL_BOX, LevelOne_OnCollision);

    //Insert spawn x,y here
    CP_Vector PlayerPos = SetPlayerPosition();
    PLY_CreatePlayer(PlayerPos.x, PlayerPos.y);
    Objectives_RenderUI();

    LevelOneGridColliderInit();
    InitPause();
    InitDrawFPS();
}

void LevelOne_update(void)
{
    SM_SystemsPreUpdate();

    if (!IsPaused())
    {
        if (Objectives_GetPlayerUpdate())
        {
            PLY_Update();
        }

        Objectives_Update();
    }


    SM_SystemsUpdate(0);

    Renderer* r = RM_GetComponent(GameFpsCounterObj);
    float fps = 1.0f / CP_System_GetDt();
    char t[7] = { 0 };
    sprintf_s(t, 7, "%5.2f", fps);
    RM_SetText(r, t);

    SM_SystemsLateUpdate();
    LevelOneUI_render(); 
    //LoaderUpdate();
}

void LevelOne_exit(void)
{
    ClearPause();
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


/*!
@brief This function initialises the pause menu
the function will return value 0x87654321.
@return void
*//*______________________________________________________________*/
void InitPause()
{
    pauseMenus = NULL;
    //just to be safe, can load again
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    GameObject* g = GOM_Create(RECTANGLE);
    g->position = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f);
    g->scale = CP_Vector_Set(screenHeight * 0.5f, screenHeight * 0.5f);
    Renderer* r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    LL_Add(&pauseMenus, g);
    GameObject* temp = g;

    g = GOM_Create(EMPTY);
    g->position = CP_Vector_Set(temp->position.x, temp->position.y - temp->scale.y * 0.2f);
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->textScale = CP_Vector_Set(3.0f, 3.0f);
    RM_SetText(r, "Pause");
    LL_Add(&pauseMenus, g);

    g = GOM_Create(RECTANGLE);
    g->position = CP_Vector_Set(temp->position.x + temp->scale.x * 0.5f, temp->position.y - temp->scale.y * 0.5f);
    g->scale = CP_Vector_Set(50, 50);
    g->tag = "PauseClose";
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    RM_LoadImage(r, "Assets/cross.png");
    Collider* c = CLM_AddComponent(g);
    CLM_Set(c, COL_BOX, LevelOne_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;
    LL_Add(&pauseMenus, g);

    g = GOM_Create(RECTANGLE);
    g->position = CP_Vector_Set(temp->position.x, temp->position.y + temp->scale.y * 0.2f);
    g->scale = CP_Vector_Set(220, 50);
    g->tag = "ReturnToMainMenu";
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->color = CP_Color_Create(255, 255, 255, 255);
    r->textScale = CP_Vector_Set(1.5f, 1.5f);
    RM_SetText(r, "Return to main menu");
    c = CLM_AddComponent(g);
    CLM_Set(c, COL_BOX, LevelOne_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;
    LL_Add(&pauseMenus, g);

    LinkedList* n = pauseMenus;
    for (; n; n = n->next)
    {
        GameObject* t = (GameObject*)n->curr;
        t->isEnabled = 0;
    }
}
void SetPause(int isPause)
{
    LinkedList* n = pauseMenus;
    for (; n; n = n->next)
    {
        GameObject* g = (GameObject*)n->curr;
        g->isEnabled = isPause;
    }
}
/*!
@brief This function checks if pauses is on
@return int - true or false of pause state
*//*______________________________________________________________*/
int IsPaused()
{
    if (CP_Input_KeyTriggered((CP_KEY)KEY_ESCAPE) || CP_Input_KeyTriggered((CP_KEY)KEY_P))
    {
        SetPause(1);
    }
    int b_IsPaused = 0;
    if (pauseMenus)
        b_IsPaused = ((GameObject*)pauseMenus->curr)->isEnabled;
    
    if (b_IsPaused)
    {
        LinkedList* n = pauseMenus; GameObject* button = NULL;
        for (; n; n = n->next)
        {
            GameObject* tn = (GameObject*)n->curr;
            if (strcmp(tn->tag, "ReturnToMainMenu") == 0)
            {
                button = tn;
                break;
            }
        }
        if (button)
        {
            float mouseX = CP_Input_GetMouseX();
            float mouseY = CP_Input_GetMouseY();
            GameObject* tempMouse = GOM_CreateTemp(EMPTY);
            tempMouse->position = CP_Vector_Set(mouseX, mouseY);
            Collider* c = CLM_AddComponent(tempMouse);
            CLM_Set(c, COL_POINT, NULL);
            c->space = COLSPC_SCREEN;
            c->isTrigger = 1;

            Collider* buttonc = CLM_GetComponent(button);
            CP_Vector spd = CP_Vector_Set(200, 100);
            CP_Vector def = CP_Vector_Set(220, 50);
            CP_Vector max = CP_Vector_Set(260, 70);
            if (IsBoxCollidePoint(buttonc, c))
            {
                button->scale.x += spd.x * CP_System_GetDt();
                button->scale.y += spd.y * CP_System_GetDt();
                button->scale.x = min(button->scale.x, max.x);
                button->scale.y = min(button->scale.y, max.y);
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

    }
    



    return b_IsPaused;
}
/*!
@brief This function clears memory used by pause menu
@return void
*//*______________________________________________________________*/
void ClearPause()
{
    LL_Clear(&pauseMenus);
    pauseMenus = NULL;
}

void InitDrawFPS()
{
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    CP_Vector padding = CP_Vector_Set(20, 20);
    CP_Vector size = CP_Vector_Set(50, 20);
    GameFpsCounterObj = GOM_Create(EMPTY);
    GameFpsCounterObj->position = CP_Vector_Set(screenWidth - padding.y - size.x * 0.5f, 0 + padding.y + size.y * 0.5f);
    GameFpsCounterObj->scale = size;
    Renderer* r = RM_AddComponent(GameFpsCounterObj);
    r->renderPriority = PRI_UI;
    RM_SetText(r, "FPS:");
    r->textColor = COLOR_GREEN;
    GameFpsCounterObj->isEnabled = 0;

#if _DEBUG
    GameFpsCounterObj->isEnabled = 1;
#endif
}

void RenderFpsCounter()
{
}
