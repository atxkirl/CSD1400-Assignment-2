/*!
@file LevelOne_AStar.c
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
#include "SceneManager.h"
#include "Objectives.h"
#include "Player.h"
#include "DetectMemoryLeak.h"

static GameObject* bananaBoi = NULL;
static GameObject* ObjectiveUI = NULL;
static float screenWidth;
static float screenHeight;

static GameObject* howtoplay = NULL;
static GameObject* close = NULL;

static AStar_Map map;
FSM* enemy1, * enemy2, * enemy3, * enemy4;

static bool isPaused;

void LevelOneAStar_UI_render();
void LevelOneAStar_GridColliderInit();
void LevelOneAStar_InputUpdate(void);

void LevelOneAStar_AStarInit(void);

void LevelOneAStar_OnCollision(Collider* left, Collider* right)
{
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "exit") == 0)
        {
            howtoplay->isEnabled = 0;
            close->isEnabled = 0;
            isPaused = false;
        }
    }
}

void LevelOneAStar_init(void)
{
    SM_SystemsInit();
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);

    isPaused = true;

    // Loader
    {
        LoaderInit();
        LoadGrid("level01", 0);
        LoadObjectives("Obj1");
        Objectives_Init(screenWidth, screenHeight);
        Objectives_RenderUI();

        // Initialize level colliders
        for (int i = 0; i < NumGrids; i++)
        {
            for (int j = 0; j < NumGrids; j++)
            {
                if (gLoadedGrids->gGrid[i][j]->type == WATER || gLoadedGrids->gGrid[i][j]->type == EMPTY)
                {
                    CLM_Set(CLM_GetComponent(gLoadedGrids->gGrid[i][j]), COL_BOX, LevelOneAStar_OnCollision);
                    CLM_GetComponent(gLoadedGrids->gGrid[i][j])->isLockedPos = 1;
                }
            }
        }
    }

    // Player
    {
        //Insert spawn x,y here
        CP_Vector PlayerPos = GetPlayerPosition();
        bananaBoi = PLY_CreatePlayer(PlayerPos.x, PlayerPos.y);
    }

    // Scene UI
    {
        // Objective UI
        GameObject* button = GOM_Create(RECTANGLE);
        button->scale = CP_Vector_Set(75.f, 50.f);
        button->position = CP_Vector_Set(80.0f, 200.0f);
        button->tag = "objone";
        button->type = RECTANGLE;
        Renderer* r = RM_AddComponent(button);
        r->color = CP_Color_Create(255, 255, 255, 255);
        //r->text = "Objective";
        RM_SetText(r, "Objective!");
        r->renderPriority = PRI_UI;
        Collider* c = CLM_AddComponent(button);
        c->space = COLSPC_SCREEN;
        CLM_Set(c, COL_BOX, LevelOneAStar_OnCollision);


        GameObject* ObjectiveUIBox = GOM_Create(WALL);
        ObjectiveUIBox->scale = CP_Vector_Set(200.f, 70.f);
        ObjectiveUIBox->position = CP_Vector_Set(120.0f, 50.0f);
        ObjectiveUIBox->tag = "ObjectiveUI";
        r = RM_AddComponent(ObjectiveUIBox);
        r->color = CP_Color_Create(255, 255, 255, 255);
        r->renderPriority = PRI_UI;

        for (int i = 0; i < MAX_OBJECTIVES; i++)
        {
            ObjectiveUI = GOM_Create(EMPTY);
            ObjectiveUI->scale = CP_Vector_Set(175.f, 50.f);
            ObjectiveUI->tag = "ObjectiveUI";
            ObjectiveUI->position = CP_Vector_Set(100.0f, 50.0f + i * 50.f);
            r = RM_AddComponent(ObjectiveUI);
            r->color = CP_Color_Create(255, 255, 255, 255);
            //r->text = oObjectiveList[i].cObjective;
            RM_SetText(r, oObjectiveList[i].cObjective);
            r->renderPriority = PRI_UI;
        }

        // render how to play
        howtoplay = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
        howtoplay->tag = "credits";
        Renderer* howtoplayRenderer = RM_AddComponent(howtoplay);
        howtoplayRenderer->renderPriority = PRI_UI;
        RM_LoadImage(howtoplayRenderer, "Assets/scenes/howtoplay.png");
        // close button
        close = GOM_Create(RECTANGLE);
        close->position = CP_Vector_Set(screenWidth * 0.975f, screenHeight * 0.045f);
        close->scale = CP_Vector_Set(50, 50);
        close->tag = "exit";
        Collider* closeCollider = CLM_AddComponent(close);
        CLM_Set(closeCollider, COL_BOX, LevelOneAStar_OnCollision);
        closeCollider->space = COLSPC_SCREEN;
        Renderer* closeRender = RM_AddComponent(close);
        closeRender->renderPriority = PRI_UI;
        RM_LoadImage(closeRender, "Assets/cross.png");

        //// Player
        //player = GOM_Create2(RECTANGLE, CP_Vector_Set(462, 462), 0.0f, CP_Vector_Set(50, 50));
        //player->tag = "player";
        //GetPlayerPosition(player->position.x, player->position.y);
        //r = RM_AddComponent(player);
        //RM_LoadImage(r, "Assets/bananaboi.png");
        //CLM_Set(CLM_AddComponent(player), COL_BOX, LevelOneAStar_OnCollision);
        //LevelOneAStar_GridColliderInit();
    }

    // AStar
    {
        AStar_InitializeMap(&map, NumGrids, NumGrids);

        // Set settings of each node in the level based on the level loaded.
        LevelOneAStar_AStarInit();
    }

    // Enemies
    {
        enemy1 = AIM_CreateEnemy("BBEM", "BBEM_Idle", CP_Vector_Set(198.f, 66.f), bananaBoi, &map);
        //enemy2 = AIM_CreateEnemy("Testnemy2", "BBEM_Idle", CP_Vector_Set(598.f, 66.f), player, &map);
        //enemy3 = AIM_CreateEnemy("Testnemy3", "BBEM_Idle", CP_Vector_Set(698.f, 66.f), player, &map);
    }
}

void LevelOneAStar_update(void)
{
    SM_SystemsPreUpdate();

    if (!isPaused)
    {
        if (Objectives_GetPlayerUpdate())
        {
            PLY_Update();
        }
        Objectives_Update();
    }
    SM_SystemsUpdate(isPaused);

    RM_SetCameraPosition(bananaBoi->position);
    SM_SystemsLateUpdate();

    LevelOneAStar_InputUpdate();
    LevelOneAStar_UI_render();

    CP_Settings_TextSize(20);
    CP_Settings_BlendMode(CP_BLEND_ALPHA);
    CP_Settings_Fill(CP_Color_Create(0, 0, 0, 128));
    CP_Settings_NoStroke();
    CP_Graphics_DrawRect(0, 0, 150, 30);
    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    char buffer[100];
    sprintf_s(buffer, 100, "FPS: %f", CP_System_GetFrameRate());
    CP_Font_DrawText(buffer, 20, 20);

    CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
    CP_Settings_StrokeWeight(1.f);
}

void LevelOneAStar_exit(void)
{
    // Release memory for map.map
    AStar_ClearMap(&map);

    LoaderExit();
    SM_SystemsExit();

    // Print out memory leaks
    //_CrtDumpMemoryLeaks();
}

void LevelOneAStar_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = LevelOneAStar_init;
    *update = LevelOneAStar_update;
    *exit = LevelOneAStar_exit;
}

void LevelOneAStar_UI_render()
{
    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    //CP_Graphics_DrawRect(50.f, 170.f, 75.f, 50.f);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    //CP_Font_DrawText("Objective", 55, 200);
}

void LevelOneAStar_GridColliderInit()
{
    for (int i = 0; i < NumGrids; i++)
    {
        for (int j = 0; j < NumGrids; j++)
        {
            if (gLoadedGrids->gGrid[i][j]->type == WATER || gLoadedGrids->gGrid[i][j]->type == EMPTY)
            {
                CLM_Set(CLM_GetComponent(gLoadedGrids->gGrid[i][j]), COL_BOX, LevelOneAStar_OnCollision);
                CLM_GetComponent(gLoadedGrids->gGrid[i][j])->isLockedPos = 1;
            }

            if (gLoadedObjects->gGrid[i][j]->type == CORAL || gLoadedObjects->gGrid[i][j]->type == GRASS)
            {
                printf("Coral detected!\n");
                CLM_GetComponent(gLoadedObjects->gGrid[i][j])->OnCollision = LevelOneAStar_OnCollision;
            }
        }
    }
}

void LevelOneAStar_AStarInit(void)
{
    for (int r = 0; r < map.rows; ++r)
    {
        for (int c = 0; c < map.columns; ++c)
        {
            AStar_Node* temp = &map.map[r][c];
            AStar_InitializeNode(&temp, r, c, gLoadedGrids->gGrid[r][c]->position, NODE_DEFAULT);

            if (gLoadedGrids->gGrid[r][c]->type == WATER || gLoadedGrids->gGrid[r][c]->type == EMPTY)
            {
                temp->type = NODE_WALL;
            }

            /*GameObject* g = GOM_Create2(CIRCLE, temp->position, 0.f, CP_Vector_Set(20.f, 20.f));
            Renderer* rend = RM_AddComponent(g);
            if (temp->type == NODE_WALL)
                rend->color = CP_Color_Create(255, 0, 0, 255);
            else
                rend->color = CP_Color_Create(0, 255, 0, 255);*/
        }
    }
}

void LevelOneAStar_InputUpdate(void)
{
    if (CP_Input_KeyTriggered(KEY_ESCAPE))
    {
        SceneManager_ChangeSceneByName("mainmenu");
    }
}