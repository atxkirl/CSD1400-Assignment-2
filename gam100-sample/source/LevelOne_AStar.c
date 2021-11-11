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

static GameObject* gLOne = NULL;
static GameObject* ObjectiveUI = NULL;

static AStar_Map map;
//TESTCODE
Enemy* enemy1, * enemy2, * enemy3, * enemy4;

void LevelOneAStar_UI_render();
void LevelOneAStar_GridColliderInit();

void LevelOneAStar_AStarInit(void);

void LevelOneAStar_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "objone") == 0)
        {
            SetObjectiveComplete(1, 1);
            for (int i = 0; i < MAX_OBJECTIVES; i++)
            {
                if (oObjectiveList[i].isComplete && !oObjectiveList[i].isSet)
                {
                    ObjectiveUI->position = CP_Vector_Set(180.0f, 50.0f + i * 50.f);

                    Renderer* rObjUI = RM_AddComponent(ObjectiveUI);
                    rObjUI->color = CP_Color_Create(255, 255, 255, 255);
                    rObjUI->text = "Done.";
                    rObjUI->renderPriority = PRI_UI;
                    oObjectiveList[i].isSet = 1;
                }
            }
        }
    }
}

void LevelOneAStar_init(void)
{
    SM_SystemsInit();

    // Loader
    {
        LoaderInit();
        LoadGrid("level01", 0);
        //here is where ill load objectives
        LoadObjectives("Obj1");
    }

    // Scene UI & Player
    {
        // Objective UI
        GameObject* button = GOM_Create(RECTANGLE);
        button->scale = CP_Vector_Set(75.f, 50.f);
        button->position = CP_Vector_Set(80.0f, 200.0f);
        button->tag = "objone";
        button->type = RECTANGLE;
        Renderer* r = RM_AddComponent(button);
        r->color = CP_Color_Create(255, 255, 255, 255);
        r->text = "Objective";
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
            r->text = oObjectiveList[i].cObjective;
            r->renderPriority = PRI_UI;
        }

        // Player
        gLOne = GOM_Create2(RECTANGLE, CP_Vector_Set(500, 500), 0.0f, CP_Vector_Set(50, 50));
        gLOne->tag = "player";
        SetPlayerPosition(gLOne->position.x, gLOne->position.y);
        r = RM_AddComponent(gLOne);
        RM_LoadImage(r, "Assets/bananaboi.png");
        CLM_Set(CLM_AddComponent(gLOne), COL_BOX, LevelOneAStar_OnCollision);
        LevelOneAStar_GridColliderInit();
    }

    // AStar
    {
        AStar_InitializeMap(&map, NumGrids, NumGrids);

        // Set settings of each node in the level based on the level loaded.
        LevelOneAStar_AStarInit();
    }

    // Enemies
    {
        enemy1 = EM_CreateEnemy("Testnemy1", "BBEM_Idle", CP_Vector_Set(66.f, 66.f), &map);
        enemy1->stateMachine->target = gLOne;
    }
}

void LevelOneAStar_update(void)
{
    SM_SystemsPreUpdate();

    float dt = CP_System_GetDt();
    float spd = 200.0f;

    // Player Movement
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


    // Miscellaneous Keyboard Inputs
    {
        if (CP_Input_KeyTriggered(KEY_ESCAPE))
        {
            CP_Engine_Terminate();
        }
    }

    SM_SystemsUpdate();

    RM_SetCameraPosition(gLOne->position);
    SM_SystemsLateUpdate();

    LevelOneAStar_UI_render();

    // Update the enemy of the Player's position.
    enemy1->stateMachine->target = gLOne;
}

void LevelOneAStar_exit(void)
{
    // Release memory for map.map
    AStar_ClearMap(&map);

    LoaderExit();
    SM_SystemsExit();
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

            GameObject* g = GOM_Create2(CIRCLE, temp->position, 0.f, CP_Vector_Set(20.f, 20.f));
            Renderer* rend = RM_AddComponent(g);
            if (temp->type == NODE_WALL)
                rend->color = CP_Color_Create(255, 0, 0, 255);
            else
                rend->color = CP_Color_Create(0, 255, 0, 255);
        }
    }
}