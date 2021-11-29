/*!
@file            Objectives.c
@author          Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course          CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief           This file contains the functions for all level objectives.
*//*______________________________________________________________________*/

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
#include "SceneManager.h"
#include "Player.h"

int iUpdatePlayer;
CP_Vector vObjectiveOnePos, vObjectiveTwoPos, vObjectiveThreePos;
float screenWidth, screenHeight;

GameObject** gObjectives;
int iNumObjectives;
int iCurrentObjective;
GameObject* g_ObjectiveUI[MAX_OBJECTIVES];
Renderer* g_ObjectiveTileOverlay[MAX_OBJECTIVES];
//GameObject* OBJ_FloatingTextHint[MAX_OBJECTIVES];
#define OVERLAY_BLINK_DEFAULT CP_Color_Create(0, 100, 0, 255)
#define OVERLAY_BLINK_TARGET CP_Color_Create(0, 100 , 0, 50)
#define OVERLAY_BLINK_TIME 2.0f
#define OVERLAY_BLINK_STROKE_WEIGHT 4.0f

/*!
@brief Initialises OBJ_FloatingTextHint[index]
@param index - index of array to be init
@param pos - pos of flt text obj
@return void
*/
//void InitFloatingText(int index, CP_Vector pos);

int iAllObjectivesComplete;
int iPrintExit;
char* cWire;

/// <summary>
/// Handles the collision of the the GameObjects based on the left and right collider parameters.
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void Objectives_onCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "player") == 0)
    {
        // CONNECT WIRES
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
                OB_ConnectTrigger();
                iUpdatePlayer = 0; 
            }
            PLY_ShowInteractHint();
        }

        // FIX BOAT
        else if (strcmp(((GameObject*)left->obj)->tag, "Objective2Read") == 0)
        {
            if (CP_Input_KeyTriggered(cControls->cInteract))
            {
                if (GetBoatParts() == GetLoadedNumBoatParts())
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
                else
                {
                    DM_PrintDialogue("Not Enough Boat Parts!", DIALOGUE_CLOSEBUTTON);
                }
            }
            PLY_ShowInteractHint();
        }

        // BREAK COCONUT
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
            PLY_ShowInteractHint();
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "Exit") == 0 && Objectives_GetCompleteAll())
        {
            SceneManager_ChangeSceneByName("gameEnd");
            printf("yay!\n");
        }

        //if (strcmp(((GameObject*)left->obj)->tag, "Objective4Read") == 0)
        //{
        //    if (CP_Input_KeyTriggered(KEY_E))
        //        OB_ToggleActive();
        //}
    }
}

/// <summary>
/// Initialises the variables in this file.
/// </summary>
/// <param name="fScreenWidth"></param>
/// <param name="fScreenHeight"></param>
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
    OB_PickupInit();

    iAllObjectivesComplete = 0;
    iPrintExit = 0;
    cWire = malloc(sizeof(char) * 100);
}

/// <summary>
/// Update.
/// </summary>
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
        g_ObjectiveUI[iCurrentObjective]->position.x = screenWidth * 0.057f;
        Renderer* rObjUI = RM_GetComponent(g_ObjectiveUI[iCurrentObjective]);
        RM_SetText(rObjUI, "Complete!");
        gObjectives[iCurrentObjective]->tag = "Objective1Complete";
        rObjUI->renderPriority = PRI_UI;

        //Collider* c = CLM_GetComponent(gObjectives[iCurrentObjective]); 
        g_ObjectiveTileOverlay[iCurrentObjective]->isEnabled = 0;
        //RM_GetComponent(c->obj)->color.a = 0;
        CLM_RemoveGO(gObjectives[iCurrentObjective]);
        iUpdatePlayer = 1;
        ++iAllObjectivesComplete;
    }
    else if (strcmp(gObjectives[iCurrentObjective]->tag, "Objective2Done") == 0)
    {
        g_ObjectiveUI[iCurrentObjective]->position.x = screenWidth * 0.057f;
        Renderer* rObjUI = RM_GetComponent(g_ObjectiveUI[iCurrentObjective]);
        RM_SetText(rObjUI, "Complete!");
        gObjectives[iCurrentObjective]->tag = "Objective2Complete";
        rObjUI->renderPriority = PRI_UI;

        //Collider* c = CLM_GetComponent(gObjectives[iCurrentObjective]);
        g_ObjectiveTileOverlay[iCurrentObjective]->isEnabled = 0;
        //RM_GetComponent(c->obj)->color.a = 0;
        CLM_RemoveGO(gObjectives[iCurrentObjective]);
        iUpdatePlayer = 1;
        ++iAllObjectivesComplete;
    }
    else if (strcmp(gObjectives[iCurrentObjective]->tag, "Objective3Done") == 0)
    {
        g_ObjectiveUI[iCurrentObjective]->position.x = screenWidth * 0.057f;
        Renderer* rObjUI = RM_GetComponent(g_ObjectiveUI[iCurrentObjective]);
        RM_SetText(rObjUI, "Complete!");
        gObjectives[iCurrentObjective]->tag = "Objective3Complete";
        rObjUI->renderPriority = PRI_UI;

        //Collider* c = CLM_GetComponent(gObjectives[iCurrentObjective]);
        g_ObjectiveTileOverlay[iCurrentObjective]->isEnabled = 0;
        //RM_GetComponent(c->obj)->color.a = 0;
        CLM_RemoveGO(gObjectives[iCurrentObjective]);
        iUpdatePlayer = 1;
        ++iAllObjectivesComplete;
    }

    if (iAllObjectivesComplete == iNumObjectives && !iPrintExit)
    {
        iPrintExit = 1;
        g_ObjectiveUI[0]->position.x = screenWidth * 0.065f;
        Renderer* rObjUI = RM_GetComponent(g_ObjectiveUI[0]);
        RM_SetText(rObjUI, "Find the Exit!");

        for (int i = 1; i < iNumObjectives; ++i)
        {
            rObjUI = RM_GetComponent(g_ObjectiveUI[i]);
            RM_SetText(rObjUI, "");
        }

        //Add box overlay for exit
        for (int i = 0; i < NumGrids; i++)
        {
            for (int j = 0; j < NumGrids; j++)
            {
                if(gLoadedObjects->gGrid[i][j]->type == EXIT)
                {
                    GameObject* g = GOM_Create(RECTANGLE);
                    g->position = gLoadedObjects->gGrid[i][j]->position;
                    g->rotation = gLoadedObjects->gGrid[i][j]->rotation;
                    g->scale = gLoadedObjects->gGrid[i][j]->scale;
                    Renderer* r = RM_AddComponent(g);
                    //r->renderPriority = PRI_PLY;
                    r->color = COLOR_GREEN;
                    r->color.a = 125;

                    Collider* c = CLM_AddComponent(gLoadedObjects->gGrid[i][j]);
                    CLM_Set(c, COL_BOX, Objectives_onCollision);
                    c->isTrigger = 1;
                    gLoadedObjects->gGrid[i][j]->tag = "Exit";

                    break;
                }
            }
        }
    }

    for (int i = 0; i < MAX_OBJECTIVES; ++i)
    {
        if (strcmp(g_ObjectiveUI[i]->tag, "ObjectiveUI2") == 0)
        {
            if (GetLoadedNumBoatParts() == GetBoatParts())
            {
                g_ObjectiveUI[i]->position = CP_Vector_Set(screenWidth * 0.06f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                RM_SetText(RM_GetComponent(g_ObjectiveUI[i]), oObjectiveList[1].cObjective);
            }
            else
            {
                // ADD RENDERTEXT HERE
                g_ObjectiveUI[i]->position = CP_Vector_Set(screenWidth * 0.12f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                sprintf_s(cWire, 100, "Collect %d more boat parts to fix boat!", GetLoadedNumBoatParts() - GetBoatParts());
                RM_SetText(RM_GetComponent(g_ObjectiveUI[i]), cWire);
            }
        }
    }

    OB_ConnectUpdate();
    OB_BreakCoconutUpdate();
    OB_FixBoatUpdate();

}

/// <summary>
/// Exit.
/// </summary>
void Objectives_Exit()
{
    //for (int i = 0; i < MAX_OBJECTIVES; i++)
    //{
    //    free(g_ObjectiveUI[i]);
    //    free(g_ObjectiveTileOverlay);
    //}
    //for (int i = 0; i < iNumObjectives; i++)
    //{
    //    free(gObjectives[i]);
    //}
    free(cWire);
    free(gObjectives);
    //for (int i = 0; i < iNumObjectives; ++i)
    //{
    //    if (g_ObjectiveUI[i])
    //        free(g_ObjectiveUI[i]);
    //}

}

/// <summary>
/// Toggles the player update when doing objectives.
/// </summary>
/// <param name="iSetter"></param>
void Objectives_SetPlayerUpdate(int iSetter)
{
    iUpdatePlayer = iSetter;
}

/// <summary>
/// Render the Objectives Taskbar UI
/// </summary>
int Objectives_GetPlayerUpdate()
{
    return iUpdatePlayer;
}

/// <summary>
/// Returns the variable that handles the toggle of player update.
/// </summary>
/// <returns>int</returns>
void Objectives_RenderUI()
{
    GameObject* ObjectiveUIBox = GOM_Create(WALL);
    ObjectiveUIBox->scale = CP_Vector_Set(screenWidth * 0.19f, screenHeight * 0.15f);
    ObjectiveUIBox->position = CP_Vector_Set(screenWidth * 0.12f, screenHeight * 0.1f);
    ObjectiveUIBox->tag = "ObjectiveUI";
    Renderer* r = RM_AddComponent(ObjectiveUIBox);
    r->color = CP_Color_Create(255, 255, 255, 255);
    //RM_LoadImage(r, "Assets/TempWindow.png");
    r->renderPriority = PRI_UI;

    for (int i = 0; i < iNumObjectives;)
    {
        g_ObjectiveUI[i] = GOM_Create(EMPTY);
        for (int j = 0; j < NumGrids; j++)
        {
            for (int k = 0; k < NumGrids; k++)
            {
                //CONNECT
                if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective1") == 0)
                {
                    g_ObjectiveUI[i] = GOM_Create(EMPTY);
                    g_ObjectiveUI[i]->scale = CP_Vector_Set(175.f, 50.f);
                    g_ObjectiveUI[i]->tag = "ObjectiveUI1";
                    r = RM_AddComponent(g_ObjectiveUI[i]);
                    r->color = CP_Color_Create(255, 255, 255, 0);
                    RM_SetText(r, "");
                    g_ObjectiveUI[i]->position = CP_Vector_Set(screenWidth * 0.077f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                    vObjectiveOnePos = g_ObjectiveUI[i]->position;
                    RM_SetText(r, oObjectiveList[0].cObjective);
                    r->renderPriority = PRI_UI;
                    gLoadedGrids->gGrid[j][k]->tag = "Objective1Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, Objectives_onCollision);
                    c->isTrigger = 1;
                    g_ObjectiveTileOverlay[i] = RM_AddComponent(c->obj);
                    g_ObjectiveTileOverlay[i]->color = CP_Color_Create(0, 255, 0, 90);
                    g_ObjectiveTileOverlay[i]->strokeWeight = OVERLAY_BLINK_STROKE_WEIGHT;
                    printf("CONNECT: %d\n", i);
                    gObjectives[i] = gLoadedGrids->gGrid[j][k];

                    Animation* anim = AM_AddComponent(g_ObjectiveTileOverlay[i]->go);
                    AM_SetBlink(anim, OVERLAY_BLINK_DEFAULT, OVERLAY_BLINK_TARGET, OVERLAY_BLINK_TIME, 1, 1);
                    anim->forcedRenderer = g_ObjectiveTileOverlay[i];
                    //InitFloatingText(i, gLoadedGrids->gGrid[j][k]->position);
                    ++i;
                }

                // BOAT
                else if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective2") == 0)
                {
                    g_ObjectiveUI[i] = GOM_Create(EMPTY);
                    g_ObjectiveUI[i]->scale = CP_Vector_Set(175.f, 50.f);
                    g_ObjectiveUI[i]->tag = "ObjectiveUI2";
                    r = RM_AddComponent(g_ObjectiveUI[i]);
                    r->color = CP_Color_Create(255, 255, 255, 0);
                    RM_SetText(r, "");
                    vObjectiveTwoPos = g_ObjectiveUI[i]->position;

                    r->renderPriority = PRI_UI;
                    gLoadedGrids->gGrid[j][k]->tag = "Objective2Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, Objectives_onCollision);
                    c->isTrigger = 1;
                    g_ObjectiveTileOverlay[i] = RM_AddComponent(c->obj);
                    g_ObjectiveTileOverlay[i]->color = CP_Color_Create(0, 255, 0, 90);
                    g_ObjectiveTileOverlay[i]->strokeWeight = OVERLAY_BLINK_STROKE_WEIGHT;
                    printf("BOAT: %d\n", i);
                    gObjectives[i] = gLoadedGrids->gGrid[j][k];

                    Animation* anim = AM_AddComponent(g_ObjectiveTileOverlay[i]->go);
                    AM_SetBlink(anim, OVERLAY_BLINK_DEFAULT, OVERLAY_BLINK_TARGET, OVERLAY_BLINK_TIME, 1, 1);
                    anim->forcedRenderer = g_ObjectiveTileOverlay[i];
                    //InitFloatingText(i, gLoadedGrids->gGrid[j][k]->position);
                    ++i;
                }

                // BREAK COCONUT
                else if (strcmp(gLoadedGrids->gGrid[j][k]->tag, "Objective3") == 0)
                {
                    g_ObjectiveUI[i] = GOM_Create(EMPTY);
                    g_ObjectiveUI[i]->scale = CP_Vector_Set(175.f, 50.f);
                    g_ObjectiveUI[i]->tag = "ObjectiveUI3";
                    r = RM_AddComponent(g_ObjectiveUI[i]);
                    r->color = CP_Color_Create(255, 255, 255, 0);
                    RM_SetText(r, "");
                    g_ObjectiveUI[i]->position = CP_Vector_Set(screenWidth * 0.1025f, screenHeight * 0.05f + i * screenHeight * 0.025f);
                    vObjectiveThreePos = g_ObjectiveUI[i]->position;
                    RM_SetText(r, oObjectiveList[2].cObjective);
                    r->renderPriority = PRI_UI;
                    gLoadedGrids->gGrid[j][k]->tag = "Objective3Read";
                    Collider* c = CLM_AddComponent(gLoadedGrids->gGrid[j][k]);
                    CLM_Set(c, COL_BOX, Objectives_onCollision);
                    c->isTrigger = 1;
                    g_ObjectiveTileOverlay[i] = RM_AddComponent(c->obj);
                    g_ObjectiveTileOverlay[i]->color = CP_Color_Create(0, 255, 0, 90);
                    g_ObjectiveTileOverlay[i]->strokeWeight = OVERLAY_BLINK_STROKE_WEIGHT;
                    printf("COCO: %d\n", i);
                    gObjectives[i] = gLoadedGrids->gGrid[j][k];

                    Animation* anim = AM_AddComponent(g_ObjectiveTileOverlay[i]->go);
                    AM_SetBlink(anim, OVERLAY_BLINK_DEFAULT, OVERLAY_BLINK_TARGET, OVERLAY_BLINK_TIME, 1, 1);
                    anim->forcedRenderer = g_ObjectiveTileOverlay[i];
                    //InitFloatingText(i, gLoadedGrids->gGrid[j][k]->position);
                    ++i;
                }
            }
        }
    }
}

/// <summary>
/// Checks if player has completed all the objectives of the level.
/// </summary>
/// <returns>int</returns>
int Objectives_GetCompleteAll()
{
    return (iAllObjectivesComplete == iNumObjectives);
}

//void InitFloatingText(int index, CP_Vector pos)
//{
//    CP_Vector scale = CP_Vector_Set(100, 100);
//    GameObject* g = GOM_Create2(EMPTY, pos, 0, scale);
//    Renderer* r = RM_AddComponent(g);
//    char temp[10] = {0};
//    sprintf_s(temp, 10, "Press %c", cControls->cInteract);
//    RM_SetText(r, temp);
//    r->textScale = CP_Vector_Set(0.5f,0.5f);
//    Animation* an = AM_AddComponent(g);
//    AM_SetUpFloatDownState(an, pos, CP_Vector_Set(pos.x, pos.y - 20), 5.0f, 0.5f, 2.0f);
//    g->isEnabled = 0;
//    OBJ_FloatingTextHint[index] = g;
//}
