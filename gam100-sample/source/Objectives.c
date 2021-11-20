#include "Objectives.h"
#include "SystemManager.h"
#include "LevelEditor.h"
#include "Loader.h"
#include "Objective_BreakCoconut.h"
#include "Objective_Connect.h"
#include "Objective_Fixboat.h"
#include "Objective_Pickup.h"
#include "Colors.h"
#include "Controls.h"

int iUpdatePlayer;
CP_Vector vObjectiveOnePos, vObjectiveTwoPos, vObjectiveThreePos;
float screenWidth, screenHeight;

GameObject** gObjectives;
int iNumObjectives;
int iCurrentObjective;
GameObject* g_ObjectiveUI[MAX_OBJECTIVES];
Renderer* g_ObjectiveTileOverlay[MAX_OBJECTIVES];

void Objectives_onCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "player") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "Objective1Read") == 0)
        {
            if (CP_Input_KeyTriggered(cControls->cInteract))
            {
                for (int i = 0; i < iNumObjectives; i++)
                {
                    if (((GameObject*)left->obj)->position.x == gObjectives[i]->position.x
                        && ((GameObject*)left->obj)->position.y == gObjectives[i]->position.y)
                    {
                        iCurrentObjective = i;
                    }
                }
                OB_ConnectToggleActive();
                iUpdatePlayer = 0; 
            }
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "Objective2Read") == 0)
        {
            if (CP_Input_KeyTriggered(cControls->cInteract))
            {
                for (int i = 0; i < iNumObjectives; i++)
                {
                    if (((GameObject*)left->obj)->position.x == gObjectives[i]->position.x
                        && ((GameObject*)left->obj)->position.y == gObjectives[i]->position.y)
                    {
                        iCurrentObjective = i;
                    }
                }
                OB_FixBoatTrigger();
                iUpdatePlayer = 0;
            }
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "Objective3Read") == 0)
        {
            if (CP_Input_KeyTriggered(cControls->cInteract))
            {
                for (int i = 0; i < iNumObjectives; i++)
                {
                    if (((GameObject*)left->obj)->position.x == gObjectives[i]->position.x
                        && ((GameObject*)left->obj)->position.y == gObjectives[i]->position.y)
                    {
                        iCurrentObjective = i;
                    }
                }
                OB_BreakCoconutTrigger();
                iUpdatePlayer = 0;
            }
        }

        //if (strcmp(((GameObject*)left->obj)->tag, "Objective4Read") == 0)
        //{
        //    if (CP_Input_KeyTriggered(KEY_E))
        //        OB_ToggleActive();
        //}
    }
}

void Objectives_Init(float fScreenWidth, float fScreenHeight)
{
    //here is where ill load objectives
    LoadObjectives("Obj1");

    iUpdatePlayer = 1;
    screenWidth = fScreenWidth;
    screenHeight = fScreenHeight;
    iNumObjectives = GetNumObjectives();

    gObjectives = malloc(sizeof(GameObject*) * iNumObjectives);
    for (int i = 0; i < iNumObjectives; i++)
    {
        if (gObjectives)
        {
            *(gObjectives+ i) = malloc(sizeof(GameObject));
        }
    }
    iCurrentObjective = 0;
    OB_ConnectInit();
    OB_FixBoatInit();
    OB_BreakCoconutInit();
}

void Objectives_Update()
{
    if (iNumObjectives == 0)
        return;

    if (!OB_IsBreakCoconutActive() && !OB_ConnectGetActive() && !OB_IsFixBoatActive() && !iUpdatePlayer)
        iUpdatePlayer = 1;

    if (OB_ConnectIsComplete() == 1 && strcmp(gObjectives[iCurrentObjective]->tag, "Objective1Read") == 0)
    {
        gObjectives[iCurrentObjective]->tag = "Objective1Done";
    }

    if (OB_IsFixBoatComplete() == 1 && strcmp(gObjectives[iCurrentObjective]->tag, "Objective2Read") == 0)
    {
        gObjectives[iCurrentObjective]->tag = "Objective2Done";
    }

    if (OB_IsBreakCoconutComplete() == 1 && strcmp(gObjectives[iCurrentObjective]->tag, "Objective3Read") == 0)
    {
        gObjectives[iCurrentObjective]->tag = "Objective3Done";
    }

    if (strcmp(gObjectives[iCurrentObjective]->tag, "Objective1Done") == 0)
    {
        Renderer* rObjUI = RM_GetComponent(g_ObjectiveUI[iCurrentObjective]);
        RM_SetText(rObjUI, "Complete!");
        gObjectives[iCurrentObjective]->tag = "Objective1Complete";
        rObjUI->renderPriority = PRI_UI;

        //Collider* c = CLM_GetComponent(gObjectives[iCurrentObjective]); 
        g_ObjectiveTileOverlay[iCurrentObjective]->isEnabled = 0;
        //RM_GetComponent(c->obj)->color.a = 0;
        CLM_RemoveGO(gObjectives[iCurrentObjective]);
        iUpdatePlayer = 1;
    }
    else if (strcmp(gObjectives[iCurrentObjective]->tag, "Objective2Done") == 0)
    {
        Renderer* rObjUI = RM_GetComponent(g_ObjectiveUI[iCurrentObjective]);
        RM_SetText(rObjUI, "Complete!");
        gObjectives[iCurrentObjective]->tag = "Objective2Complete";
        rObjUI->renderPriority = PRI_UI;

        //Collider* c = CLM_GetComponent(gObjectives[iCurrentObjective]);
        g_ObjectiveTileOverlay[iCurrentObjective]->isEnabled = 0;
        //RM_GetComponent(c->obj)->color.a = 0;
        CLM_RemoveGO(gObjectives[iCurrentObjective]);
        iUpdatePlayer = 1;
    }
    else if (strcmp(gObjectives[iCurrentObjective]->tag, "Objective3Done") == 0)
    {
        Renderer* rObjUI = RM_GetComponent(g_ObjectiveUI[iCurrentObjective]);
        RM_SetText(rObjUI, "Complete!");
        gObjectives[iCurrentObjective]->tag = "Objective3Complete";
        rObjUI->renderPriority = PRI_UI;

        //Collider* c = CLM_GetComponent(gObjectives[iCurrentObjective]);
        g_ObjectiveTileOverlay[iCurrentObjective]->isEnabled = 0;
        //RM_GetComponent(c->obj)->color.a = 0;
        CLM_RemoveGO(gObjectives[iCurrentObjective]);
        iUpdatePlayer = 1;
    }

    OB_ConnectUpdate();
    OB_BreakCoconutUpdate();
    OB_FixBoatUpdate();
}

void Objectives_Exit()
{

}

void Objectives_SetPlayerUpdate(int iSetter)
{
    iUpdatePlayer = iSetter;
}

int Objectives_GetPlayerUpdate()
{
    return iUpdatePlayer;
}

void Objectives_RenderUI()
{
    GameObject* ObjectiveUIBox = GOM_Create(WALL);
    ObjectiveUIBox->scale = CP_Vector_Set(screenWidth * 0.175f, screenHeight * 0.15f);
    ObjectiveUIBox->position = CP_Vector_Set(screenWidth * 0.12f, screenHeight * 0.1f);
    ObjectiveUIBox->tag = "ObjectiveUI";
    Renderer* r = RM_AddComponent(ObjectiveUIBox);
    r->color = CP_Color_Create(255, 255, 255, 255);
    r->renderPriority = PRI_UI;

    for (int i = 0; i < iNumObjectives;)
    {
        g_ObjectiveUI[i] = malloc(sizeof(GameObject*));
        for (int j = 0; j < NumGrids; j++)
        {
            for (int k = 0; k < NumGrids; k++)
            {
                if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective1") == 0)
                {
                    g_ObjectiveUI[i] = GOM_Create(EMPTY);
                    g_ObjectiveUI[i]->scale = CP_Vector_Set(175.f, 50.f);
                    g_ObjectiveUI[i]->tag = "ObjectiveUI";
                    r = RM_AddComponent(g_ObjectiveUI[i]);
                    r->color = CP_Color_Create(255, 255, 255, 0);
                    RM_SetText(r, "");
                    g_ObjectiveUI[i]->position = CP_Vector_Set(screenWidth * 0.08f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                    vObjectiveOnePos = g_ObjectiveUI[i]->position;
                    RM_SetText(r, oObjectiveList[0].cObjective);
                    r->renderPriority = PRI_UI;
                    gLoadedGrids->gGrid[j][k]->tag = "Objective1Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, Objectives_onCollision);
                    c->isTrigger = 1;
                    g_ObjectiveTileOverlay[i] = RM_AddComponent(c->obj);
                    g_ObjectiveTileOverlay[i]->color = COLOR_RED;
                    g_ObjectiveTileOverlay[i]->color.a = 120;
                    printf("CONNECT: %d\n", i);
                    gObjectives[i] = gLoadedGrids->gGrid[j][k];
                    ++i;
                }
                else if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective2") == 0)
                {
                    g_ObjectiveUI[i] = GOM_Create(EMPTY);
                    g_ObjectiveUI[i]->scale = CP_Vector_Set(175.f, 50.f);
                    g_ObjectiveUI[i]->tag = "ObjectiveUI";
                    r = RM_AddComponent(g_ObjectiveUI[i]);
                    r->color = CP_Color_Create(255, 255, 255, 0);
                    RM_SetText(r, "");
                    g_ObjectiveUI[i]->position = CP_Vector_Set(screenWidth * 0.07f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                    vObjectiveTwoPos = g_ObjectiveUI[i]->position;
                    RM_SetText(r, oObjectiveList[1].cObjective);
                    r->renderPriority = PRI_UI;
                    gLoadedGrids->gGrid[j][k]->tag = "Objective2Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, Objectives_onCollision);
                    c->isTrigger = 1;
                    g_ObjectiveTileOverlay[i] = RM_AddComponent(c->obj);
                    g_ObjectiveTileOverlay[i]->color = COLOR_RED;
                    g_ObjectiveTileOverlay[i]->color.a = 120;
                    printf("BOAT: %d\n", i);
                    gObjectives[i] = gLoadedGrids->gGrid[j][k];
                    ++i;
                }
                else if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective3") == 0)
                {
                    g_ObjectiveUI[i] = GOM_Create(EMPTY);
                    g_ObjectiveUI[i]->scale = CP_Vector_Set(175.f, 50.f);
                    g_ObjectiveUI[i]->tag = "ObjectiveUI";
                    r = RM_AddComponent(g_ObjectiveUI[i]);
                    r->color = CP_Color_Create(255, 255, 255, 0);
                    RM_SetText(r, "");
                    g_ObjectiveUI[i]->position = CP_Vector_Set(screenWidth * 0.11f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                    vObjectiveThreePos = g_ObjectiveUI[i]->position;
                    RM_SetText(r, oObjectiveList[2].cObjective);
                    r->renderPriority = PRI_UI;
                    gLoadedGrids->gGrid[j][k]->tag = "Objective3Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, Objectives_onCollision);
                    c->isTrigger = 1;
                    g_ObjectiveTileOverlay[i] = RM_AddComponent(c->obj);
                    g_ObjectiveTileOverlay[i]->color = COLOR_RED;
                    g_ObjectiveTileOverlay[i]->color.a = 120;
                    printf("COCO: %d\n", i);
                    gObjectives[i] = gLoadedGrids->gGrid[j][k];
                    ++i;
                }
            }
        }
    }
}