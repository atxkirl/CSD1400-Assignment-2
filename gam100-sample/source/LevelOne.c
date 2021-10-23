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
#include "RenderManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Helpers.h"
#include "LevelEditor.h"
#include "FileParser.h"
#include "Loader.h"
#include "CollisionManager.h"
#include "Colors.h"

void LevelOneUI_render();

int LevelOne_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "objone") == 0)
            SetObjectiveComplete(1, 1);
    }
    return CLM_RESPONSE_REMOVENONE;
}

void LevelOne_init(void)
{
    RM_Init();
    GOM_Init();

    //RM_AddRenderObject(g);
    LoaderInit();

    LoadGrid("test", 0);
    //here is where ill load objectives
    LoadObjectives("Obj1");

    GameObject* button = GOM_CreateGameObject(RECTANGLE, PRI_UI);
    button = GOM_CreateGameObject(RECTANGLE, PRI_UI);
    button->scale = CP_Vector_Set(75.f, 50.f);
    button->position = CP_Vector_Set(80.0f, 200.0f);
    button->tag = "objone";
    button->type = RECTANGLE;
    button->color = CP_Color_Create(255, 0, 0, 50);
    CLM_AddCollider(button, LevelOne_OnCollision, COL_BOX, 75.f, 50.f);
}

void LevelOne_update(void)
{
    GameObject* clickPoint = NULL;
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
        //Creates a point obj to test collision against button
        clickPoint = GOM_CreateGameObject(RECTANGLE, PRI_GAME_OBJECT);
        clickPoint->position = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
        clickPoint->isEnabled = 0;
        clickPoint->tag = "Click";
        CLM_AddCollider(clickPoint, NULL, COL_POINT);
    }

    CLM_CheckCollisions();

    //imagine late update
    if (clickPoint)
    {
        CLM_RemoveGO(clickPoint);
        GOM_Delete(clickPoint);
    }

    CP_Graphics_ClearBackground(COLOR_BLUE);
    LevelOneUI_render();
    LoaderUpdate();
}

void LevelOne_exit(void)
{
    LoaderExit();
    RM_ClearRenderObjects();
    GOM_Clear();
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
    CP_Graphics_DrawRect(50.f, 170.f, 75.f, 50.f);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    CP_Font_DrawText("Objective", 55, 200);
}
