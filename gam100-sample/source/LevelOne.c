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
#include "Objective_Connect.h"

GameObject* gLOne = NULL;
GameObject* ObjectiveUI = NULL;
CP_Vector vObjectiveOnePos, vObjectiveTwoPos, vObjectiveThreePos;

float screenWidth, screenHeight;
void LevelOneUI_render();
void LevelOneGridColliderInit();

void LevelOne_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "player") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "Objective1Read") == 0)
        {
            if (CP_Input_KeyTriggered(KEY_E))
                OB_ToggleActive();
        }
    }
}

void LevelOne_init(void)
{
    SM_SystemsInit();
    //RM_AddComponent(g);
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    LoaderInit();

    LoadGrid("level01", 0);
    //here is where ill load objectives
    LoadObjectives("Obj1");

    GameObject* ObjectiveUIBox = GOM_Create(WALL);
    ObjectiveUIBox->scale = CP_Vector_Set(screenWidth * 0.175f, screenHeight * 0.15f);
    ObjectiveUIBox->position = CP_Vector_Set(screenWidth * 0.12f, screenHeight * 0.1f);
    ObjectiveUIBox->tag = "ObjectiveUI";
    Renderer* r = RM_AddComponent(ObjectiveUIBox);
    r->color = CP_Color_Create(255, 255, 255, 255);
    r->renderPriority = PRI_UI;

    for (int i = 0; i < MAX_OBJECTIVES; i++)
    {
        //if (!oObjectiveList[i].isComplete)
        //{
        ObjectiveUI = GOM_Create(EMPTY);
        ObjectiveUI->scale = CP_Vector_Set(175.f, 50.f);
        ObjectiveUI->tag = "ObjectiveUI";
        r= RM_AddComponent(ObjectiveUI);
        r->color = CP_Color_Create(255, 255, 255, 255);
        //r->text = "";
        RM_SetText(r, "");

        for (int j = 0; j < NumGrids; j++)
        {
            for (int k = 0; k < NumGrids; k++)
            {
                if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective1") == 0)
                {
                    ObjectiveUI->position = CP_Vector_Set(screenWidth * 0.08f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                    vObjectiveOnePos = ObjectiveUI->position;
                    //r->text = oObjectiveList[0].cObjective;
                    RM_SetText(r, oObjectiveList[0].cObjective);
                    gLoadedGrids->gGrid[j][k]->tag = "Objective1Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, LevelOne_OnCollision);
                    c->isTrigger = 1;
                    Renderer* rObj = RM_AddComponent(c->obj);
                    rObj->color = COLOR_RED;
                    rObj->color.a = 120;
                }
                else if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective2") == 0)
                {
                    ObjectiveUI->position = CP_Vector_Set(screenWidth * 0.07f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                    vObjectiveTwoPos = ObjectiveUI->position;
                    //r->text = oObjectiveList[1].cObjective;
                    RM_SetText(r, oObjectiveList[1].cObjective);
                    gLoadedGrids->gGrid[j][k]->tag = "Objective2Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, LevelOne_OnCollision);
                    c->isTrigger = 1;
                    Renderer* rObj = RM_AddComponent(c->obj);
                    rObj->color = COLOR_RED;
                    rObj->color.a = 120;
                }
                else if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective3") == 0)
                {
                    ObjectiveUI->position = CP_Vector_Set(screenWidth * 0.11f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                    vObjectiveThreePos = ObjectiveUI->position;
                    //r->text = oObjectiveList[2].cObjective;
                    RM_SetText(r, oObjectiveList[2].cObjective);
                    gLoadedGrids->gGrid[j][k]->tag = "Objective3Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, LevelOne_OnCollision);
                    c->isTrigger = 1;
                    Renderer* rObj = RM_AddComponent(c->obj);
                    rObj->color = COLOR_RED;
                    rObj->color.a = 120;
                }
            }
        }

        /*}
        else
        {
            rObjUI->text = "DONE";
        }*/

        r->renderPriority = PRI_UI;
    }

    gLOne = GOM_Create2(RECTANGLE, SetPlayerPosition(), 0.0f, CP_Vector_Set(50, 50));
    gLOne->tag = "player";
    r = RM_AddComponent(gLOne);
    RM_LoadImage(r, "Assets/bananaboi.png");
    CLM_Set(CLM_AddComponent(gLOne), COL_BOX, LevelOne_OnCollision);
    LevelOneGridColliderInit();

    printf("player pos : %f, %f", gLOne->position.x, gLOne->position.y);

    OB_ConnectInit();
}

void LevelOne_update(void)
{
    SM_SystemsPreUpdate();

    float dt = CP_System_GetDt();
    float spd = 200.0f;
    //Collider* gc = CLM_GetComponent(g);
    //gc->velocity = CP_Vector_Set(0, 0);
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

    for (int i = 0; i < MAX_OBJECTIVES; i++)
    {
        Renderer* rObjUI = RM_AddComponent(ObjectiveUI);
        rObjUI->color = CP_Color_Create(255, 255, 255, 255);
        for (int j = 0; j < NumGrids; j++)
        {
            for (int k = 0; k < NumGrids; k++)
            {
                if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective1Done") == 0)
                {
                    ObjectiveUI->position = vObjectiveOnePos;
                    ObjectiveUI->position.x = screenWidth * 0.18f;
                    //rObjUI->text = "Complete!";
                    RM_SetText(rObjUI, "Complete!");
                    gLoadedGrids->gGrid[j][k]->tag = "Objective1Complete";
                }
                else if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective2Done") == 0)
                {
                    ObjectiveUI->position = vObjectiveOnePos;
                    ObjectiveUI->position.x = screenWidth * 0.18f;
                    //rObjUI->text = "Complete!";
                    RM_SetText(rObjUI, "Complete!");
                    gLoadedGrids->gGrid[j][k]->tag = "Objective2Complete";
                }
                else if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective3Done") == 0)
                {
                    ObjectiveUI->position = vObjectiveThreePos;
                    ObjectiveUI->position.x = screenWidth * 0.18f;
                    //rObjUI->text = "Complete!";
                    RM_SetText(rObjUI, "Complete!");
                    gLoadedGrids->gGrid[j][k]->tag = "Objective3Complete";
                }
            }
        }
        rObjUI->renderPriority = PRI_UI;
    }

    OB_ConnectUpdate();

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
