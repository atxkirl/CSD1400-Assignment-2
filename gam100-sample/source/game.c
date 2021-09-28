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

void game_init(void)
{
    RM_Init();
    GOM_Init();

    GameObject* g = GOM_CreateGameObject();
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(20, 20);

    RM_AddRenderObject(g);
}

void game_update(void)
{
    RM_Render();
}

void game_exit(void)
{
    RM_ClearRenderObjects();
    GOM_Clear();
}
