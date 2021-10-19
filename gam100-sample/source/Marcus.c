/*!
@file Marcus.c
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
//#include "GameObject.h"
#include "GameObjectManager.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "CollisionManager.h"
#include "LevelEditor.h"

#define BUTTON_WIDTH 60.f
#define BUTTON_HEIGHT 30.f
#define COLOR_RED CP_Color_Create(255, 0, 0, 255)
#define COLOR_GREEN CP_Color_Create(0, 255, 0, 255)
#define COLOR_BLUE CP_Color_Create(0, 0, 255, 255)
#define COLOR_BLACK CP_Color_Create(0, 0, 0, 255)
#define COLOR_WHITE CP_Color_Create(255, 255, 255, 255)

void MarcusUI_render();

int Marcus_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag,"editor") == 0)
            SceneManager_ChangeSceneByName("editor");
        else if (strcmp(((GameObject*)left->obj)->tag, "levelone") == 0)
            SceneManager_ChangeSceneByName("levelone");
    }
    return CLM_RESPONSE_REMOVENONE;
}

void Marcus_init(void)
{
    RM_Init();
    GOM_Init();

    //RM_AddRenderObject(g);
    GameObject* button = GOM_CreateGameObject(RECTANGLE, PRI_UI);
    button->scale = CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT);
    button->position = CP_Vector_Set(80.0f, 25.0f);
    button->tag = "editor";
    button->type = RECTANGLE;
    button->color = CP_Color_Create(255, 0, 0, 50);
    CLM_AddCollider(button, Marcus_OnCollision, COL_BOX, BUTTON_WIDTH, BUTTON_HEIGHT);

    button = GOM_CreateGameObject(RECTANGLE, PRI_UI);
    button->scale = CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT);
    button->position = CP_Vector_Set(80.0f, 75.0f);
    button->tag = "levelone";
    button->type = RECTANGLE;
    button->color = CP_Color_Create(255, 0, 0, 50);
    CLM_AddCollider(button, Marcus_OnCollision, COL_BOX, BUTTON_WIDTH, BUTTON_HEIGHT);
}

void Marcus_update(void)
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

    MarcusUI_render();
    RM_Render();
}

void Marcus_exit(void)
{
    LevelEditorExit();
    RM_ClearRenderObjects();
    GOM_Clear();
}

void Marcus_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Marcus_init;
    *update = Marcus_update;
    *exit = Marcus_exit;
}

void MarcusUI_render()
{
    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    CP_Graphics_DrawRect(50.f, 10.f, BUTTON_WIDTH, BUTTON_HEIGHT);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    CP_Font_DrawText("Editor", 55, 30);

    CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
    CP_Graphics_DrawRect(50.f, 60.f, BUTTON_WIDTH, BUTTON_HEIGHT);
    CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
    CP_Font_DrawText("One", 55, 80);
}