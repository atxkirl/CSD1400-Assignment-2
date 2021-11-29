/*!
@file       HongYu.c
@author		Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team       BananaBoi
@brief		A test scene for hong yu to edit and test his functions
*/

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SystemManager.h"
#include "Objective_Fixboat.h"
#include "Objective_BreakCoconut.h"
#include "Player.h"

static GameObject* g = NULL;
static int tempSize = 0;
static GameObject* grass = NULL;

static int itempickedup = 0;

/// <summary>
/// hy's test scene collision response function
/// </summary>
/// <param name="left">- the collider of the owner of this response</param>
/// <param name="right">- the collider of the other object</param>
void hy_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(left->obj->tag, "player") == 0 
        && strcmp(right->obj->tag, "Trigger") == 0)
    {
        printf("HELLO");

        SM_DeleteGameObject(right->obj);
        //deletes the trigger when collided
        itempickedup = 1;
        printf("PICKEDUP\n");

        return;
    }
    else if (strcmp(left->obj->tag, "test") == 0 && strcmp(right->obj->tag, "Click") == 0)
    {
        tempSize = ++tempSize % 3;
        switch (tempSize)
        {
        case 0:
            CP_System_SetWindowSize(400, 400);
            break;
        case 1:
            CP_System_SetWindowSize(800, 600);
            break;
        case 2:
            CP_System_SetWindowSize(1280, 720);
            break;
        }
        return;
    }

    if (strcmp(left->obj->tag, "player") == 0
        && strcmp(right->obj->tag, "grass") == 0)
    {
        Renderer* r = RM_GetComponent(right->obj);
        r->color.a = 180;
        return;
    }

    if (strcmp(left->obj->tag, "player") == 0
        && strcmp(right->obj->tag, "objective") == 0)
    {

        if (CP_Input_KeyTriggered(KEY_E))
        {
            OB_FixBoatTrigger();
        }

        return;
    }
    else if (strcmp(left->obj->tag, "player") == 0
        && strcmp(right->obj->tag, "objective2") == 0)
    {
        if (CP_Input_KeyTriggered(KEY_E))
            OB_BreakCoconutTrigger();
        return;
    }
}

/// <summary>
/// hy's test scene init
/// </summary>
void HongYu_init(void)
{
    SM_SystemsInit();

    PLY_CreatePlayer(90, 90);

    g = GOM_Create2(RECTANGLE, CP_Vector_Set(50, 20), 0.0f, CP_Vector_Set(50, 50));
    g->tag = "player";
    Renderer* r = RM_AddComponent(g);
    RM_LoadImage(r, "Assets/bananaboi.png");
    CLM_Set(CLM_AddComponent(g), COL_BOX, hy_OnCollision);

    //int width = CP_System_GetWindowWidth();
    //int height = CP_System_GetWindowHeight();

    GameObject* button = GOM_Create2(RECTANGLE, 
        CP_Vector_Set(80, 20), 0.0f, CP_Vector_Set(30, 20));
    r = RM_AddComponent(button);
    RM_SetText(r, "test");
    r->renderPriority = PRI_UI;
    button->tag = "test";
    Collider* c = CLM_AddComponent(button);
    CLM_Set(c, COL_BOX, hy_OnCollision);
    c->space = COLSPC_SCREEN;

    GameObject* wall = GOM_Create2(RECTANGLE, 
        CP_Vector_Set(0, 0), 0.0f, CP_Vector_Set(50, 50));
    r = RM_AddComponent(wall);
    RM_LoadImage(r, "Assets/testspritesheet.png");
    Animation* a = AM_AddComponent(wall);
    AM_SetSprite(a, 4, 2, 8, 4, NULL);
    CLM_Set(CLM_AddComponent(wall), COL_BOX, hy_OnCollision);
    
    wall = GOM_Create2(CIRCLE,
        CP_Vector_Set(30, 80), 0.0f, CP_Vector_Set(30, 30));
    RM_AddComponent(wall);
    //wall->tag = "wall";
    CLM_Set(CLM_AddComponent(wall), COL_CIRCLE, NULL);

    wall = GOM_Create2(CIRCLE,
        CP_Vector_Set(30, 80), 0.0f, CP_Vector_Set(30, 30));
    r = RM_AddComponent(wall);
    r->color = CP_Color_Create(200, 180, 180, 255);
    RM_SetText(r, "lock");
    c = CLM_AddComponent(wall);
    CLM_Set(c, COL_CIRCLE, NULL);
    c->isLockedPos = 1;

    wall = GOM_Create2(CIRCLE,
        CP_Vector_Set(80, 80), 0.0f, CP_Vector_Set(30, 30));
    r = RM_AddComponent(wall);
    r->color = CP_Color_Create(200, 100, 100, 200);
    RM_SetText(r, "TRIGGER");
    wall->tag = "Trigger";
    c = CLM_AddComponent(wall);
    CLM_Set(c, COL_CIRCLE, NULL);
    //c->isLockedPos = 1;
    c->isTrigger = 1;

    wall = GOM_Create2(RECTANGLE,
        CP_Vector_Set(0, 200), 0.0f, CP_Vector_Set(50, 50));
    r = RM_AddComponent(wall);
    RM_LoadImage(r, "Assets/sand-tiles/sand-tile-0.png");
    c = CLM_AddComponent(wall);
    CLM_Set(c, COL_BOX, hy_OnCollision);
    c->isLockedPos = 1;

    grass = GOM_Create2(RECTANGLE,
        CP_Vector_Set(200, 200), 0.0f, CP_Vector_Set(50, 50));
    grass->tag = "grass";
    r = RM_AddComponent(grass);
    RM_LoadImage(r, "Assets/redcoral.png");
    c = CLM_AddComponent(grass);
    CLM_Set(c, COL_BOX, hy_OnCollision);
    c->isLockedPos = 1;
    c->isTrigger = 1;

    wall = GOM_Create2(RECTANGLE, CP_Vector_Set(-100, 0), 0, CP_Vector_Set(50, 50));
    wall->tag = "objective";
    r = RM_AddComponent(wall);
    r->color = CP_Color_Create(255, 128, 128, 128);
    RM_SetText(r, "Fix Boat");
    c = CLM_AddComponent(wall);
    CLM_Set(c, COL_BOX, hy_OnCollision);
    c->isTrigger = 1;

    wall = GOM_Create2(RECTANGLE, CP_Vector_Set(-100, -100), 0, CP_Vector_Set(50, 50));
    wall->tag = "objective2";
    r = RM_AddComponent(wall);
    r->color = CP_Color_Create(128, 255, 128, 128);
    RM_SetText(r, "Break Coconut");
    c = CLM_AddComponent(wall);
    CLM_Set(c, COL_BOX, hy_OnCollision);
    c->isTrigger = 1;

    OB_FixBoatInit();
    OB_BreakCoconutInit();
}

/// <summary>
/// hy's test scene update
/// </summary>
void HongYu_update(void)
{
    SM_SystemsPreUpdate();
    RM_GetComponent(grass)->color.a = 255;

    //float dt = CP_System_GetDt();
    //float spd = 200.0f;
    //Collider* gc = CLM_GetComponent(g);
    //gc->velocity = CP_Vector_Set(0, 0);
    //if (CP_Input_KeyDown((enum CP_KEY)KEY_W))
    //{
    //    g->position.y -= spd * dt;
    //}
    //if (CP_Input_KeyDown((enum CP_KEY)KEY_S))
    //{
    //    g->position.y += spd * dt;
    //}
    //if (CP_Input_KeyDown((enum CP_KEY)KEY_A))
    //{
    //    g->position.x -= spd * dt;
    //}
    //if (CP_Input_KeyDown((enum CP_KEY)KEY_D))
    //{
    //    g->position.x += spd * dt;
    //}
    GameObject* clickPoint = NULL;
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
        ////Creates a point obj to test collision against button
        //clickPoint = GOM_CreateTemp(EMPTY);//param doesnt matter
        //clickPoint->position = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
        //clickPoint->tag = "Click";
        //Collider* c = CLM_AddComponent(clickPoint);
        //CLM_Set(c, COL_POINT, NULL);
        //c->space = COLSPC_SCREEN;

        CP_Vector pos = RM_MousePositionToWorldSpace(CP_Input_GetMouseX(), CP_Input_GetMouseY());
        clickPoint = GOM_Create(CIRCLE);//param doesnt matter
        clickPoint->position = CP_Vector_Set(pos.x, pos.y);
        clickPoint->scale = CP_Vector_Set(10.0f, 10.0f);
        clickPoint->isEnabled = 1;
        //clickPoint->tag = "Click";
        //c = CLM_AddComponent(clickPoint);
        //CLM_Set(c, COL_POINT, NULL);
        //c->space = COLSPC_WORLD;
        RM_AddComponent(clickPoint);
    }

    char str[20] = {'\0'};
    sprintf_s(str, 20,"%.1f,%.1f", g->position.x, g->position.y);
    //g->text = str;
    Renderer* r = (Renderer*)SM_GetComponent(g, COM_RENDERER);
    r->textColor = CP_Color_Create(255, 255, 255, 255);
    RM_SetText(r, str);
    //r->text = str;


    PLY_Update();

    SM_SystemsUpdate(0);
    //RM_SetCameraPosition(g->position);
    OB_FixBoatUpdate();
    OB_BreakCoconutUpdate();
    SM_SystemsLateUpdate();
}

/// <summary>
/// hy's test scene exit
/// </summary>
void HongYu_exit(void)
{
    SM_SystemsExit();
}

/// <summary>
/// assign function pointers of hy's test scene init, update and exit to scene manager to call
/// </summary>
/// <param name="init">- pointer to assign hy's test scene init</param>
/// <param name="update">- pointer to assign hy's test scene update</param>
/// <param name="exit">- pointer to assign hy's test scene exit</param>
void HongYu_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = HongYu_init;
    *update = HongYu_update;
    *exit = HongYu_exit;
}
