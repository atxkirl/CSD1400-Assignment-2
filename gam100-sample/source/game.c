/*!
@file game.c
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
#include "SceneManager.h"

void game_init(void)
{
    RM_Init();
    GOM_Init();

    GameObject* g = GOM_CreateGameObject();
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(20, 20);

}

void game_update(void)
{
    RM_Render();

    if (CP_Input_KeyTriggered(KEY_1))
    {
        // Change to game2 scene.
        SceneManager_ChangeSceneByName("game2");
        printf("wAD");
    }
}

void game_exit(void)
{
    RM_ClearRenderObjects();
    GOM_Clear();
}

void game_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = game_init;
    *update = game_update;
    *exit = game_exit;
}
