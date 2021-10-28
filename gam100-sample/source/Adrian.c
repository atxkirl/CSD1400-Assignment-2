/*!
@file Adrian.c
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains functions of game init, update, exit
*//*______________________________________________________________________*/

#include <stdio.h>
#include <stdlib.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "../DetectMemoryLeak.h"
#include "SystemManager.h"
#include "AStar.h"

static AStar_Map map;

void Adrian_init(void)
{
    SM_SystemsInit();

    GameObject* g = GOM_Create(CIRCLE);
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);
    RM_AddComponent(g);
    
    map.rows = 10;
    map.columns = 10;

    // Allocate Memory for map.
    map.map = malloc(sizeof(AStar_Node*) * map.rows);
    if (map.map)
    {
        for (int r = 0; r < map.rows; ++r)
        {
            map.map[r] = malloc(sizeof(AStar_Node) * map.columns);
        }
    }

    // Fill map with stuff
    for (int rows = 0; rows < map.rows; ++rows)
    {
        for (int cols = 0; cols < map.columns; ++cols)
        {
            map.map[rows][cols].row = rows;
            map.map[rows][cols].column = cols;
            map.map[rows][cols].type = NODE_DEFAULT;

            map.map[rows][cols].fCost = 0;
            map.map[rows][cols].gCost = 0;
            map.map[rows][cols].hCost = 0;
            map.map[rows][cols].parent = NULL;
        }
    }

    // Set walls
    map.map[0][3].type = NODE_WALL;
    map.map[1][3].type = NODE_WALL;
    map.map[2][3].type = NODE_WALL;
    map.map[3][3].type = NODE_WALL;
    map.map[4][3].type = NODE_WALL;
    map.map[6][3].type = NODE_WALL;
    map.map[7][3].type = NODE_WALL;
    map.map[8][3].type = NODE_WALL;
    map.map[9][3].type = NODE_WALL;

    // Try pathfinding.
    printf("Test A* Pathfinding START.\n");
    AStar_GetPath(&map.map[1][1], &map.map[0][7], &map);
    printf("Test A* Pathfinding END.\n");

    // [ ][ ][ ][W][ ][ ][ ][E][ ][ ]
    // [ ][S][ ][W][ ][ ][ ][ ][ ][ ]
    // [ ][ ][ ][W][ ][ ][ ][ ][ ][ ]
    // [ ][ ][ ][W][ ][ ][ ][ ][ ][ ]
    // [ ][ ][ ][W][ ][ ][ ][ ][ ][ ]
    // [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
    // [ ][ ][ ][W][ ][ ][ ][ ][ ][ ]
    // [ ][ ][ ][W][ ][ ][ ][ ][ ][ ]
    // [ ][ ][ ][W][ ][ ][ ][ ][ ][ ]
    // [ ][ ][ ][W][ ][ ][ ][ ][ ][ ]
}

void Adrian_update(void)
{
    SM_SystemsPreUpdate();

    //Do stuff here...

    // Exit Game
    if (CP_Input_KeyTriggered(KEY_ESCAPE))
    {
        CP_Engine_Terminate();
    }

    SM_SystemsUpdate();
    CP_Settings_Fill(CP_Color_Create(255, 255, 128, 255));
    SM_SystemsLateUpdate();
    //RM_Render();
    CP_Settings_Fill(CP_Color_Create(128, 128, 128, 255));
}

void Adrian_exit(void)
{
    // Release memory for map.map
    for (int r = 0; r < map.rows; ++r)
    {
        free(map.map[r]);
        map.map[r] = NULL;
    }
    free(map.map);
    map.map = NULL;

    SM_SystemsExit();
    _CrtDumpMemoryLeaks();
    printf("Exitting Scene Adrian!\n");
}

void Adrian_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Adrian_init;
    *update = Adrian_update;
    *exit = Adrian_exit;
}
