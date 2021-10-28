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

#define NODE_SIZE 30

static AStar_Map map;
static LinkedList* path;

static float gCellHeight;
static float gCellWidth;

static CP_Color gBackgroundColor;
static CP_Color gGridColor;

static CP_Color defaultColor;
static CP_Color startColor;
static CP_Color endColor;
static CP_Color wallColor;
static CP_Color pathColor;

static AStar_Node* startNode;
static AStar_Node* endNode;

void Adrian_CallAStar(void)
{
    // Clear the map of paths
    for (int row = 0; row < NODE_SIZE; ++row)
    {
        for (int col = 0; col < NODE_SIZE; ++col)
        {
            if (map.map[row][col].type == NODE_PATH)
                map.map[row][col].type = NODE_DEFAULT;
        }
    }

    // Calculate A* path
    path = AStar_GetPath(startNode, endNode, &map);
}

void Adrian_Render(void)
{
    float gWindowWidth = (float)CP_System_GetWindowWidth();
    float gWindowHeight = (float)CP_System_GetWindowHeight();

    gBackgroundColor = CP_Color_Create(128, 128, 128, 255);
    gGridColor = CP_Color_Create(0, 0, 0, 255);

    defaultColor = CP_Color_Create(128, 128, 128, 255);
    startColor = CP_Color_Create(0, 255, 0, 255);
    endColor = CP_Color_Create(255, 0, 0, 255);
    wallColor = CP_Color_Create(0, 0, 0, 255);
    pathColor = CP_Color_Create(255, 255, 255, 255);

    // Clear the window.
    CP_Graphics_ClearBackground(gBackgroundColor);

    gCellHeight = gWindowHeight / NODE_SIZE;
    gCellWidth = gWindowWidth / NODE_SIZE;

    CP_Settings_Stroke(gGridColor);
    // Draw vertical grid lines
    for (int x = 0; x < NODE_SIZE; ++x)
    {
        CP_Graphics_DrawLine(x * gCellWidth, 0, x * gCellWidth, gWindowHeight);
    }
    // Draw horizontal grid lines
    for (int y = 0; y < NODE_SIZE; ++y)
    {
        CP_Graphics_DrawLine(0, y * gCellHeight, gWindowHeight, y * gCellHeight);
    }


    // render cells of path
    if (path)
    {
        while (path != NULL)
        {
            AStar_Node* temp = (AStar_Node*)path->curr;
            if (temp)
            {
                map.map[temp->row][temp->column].type = temp->type;
                path = path->next;
                printf("doing a thing\n");
            }
        }
    }


    // render cells of map.map
    for (int row = 0; row < NODE_SIZE; ++row)
    {
        for (int col = 0; col < NODE_SIZE; ++col)
        {
            switch (map.map[row][col].type)
            {
            case NODE_DEFAULT:
                CP_Settings_Fill(defaultColor);
                break;
            case NODE_START:
                CP_Settings_Fill(startColor);
                break;
            case NODE_END:
                CP_Settings_Fill(endColor);
                break;
            case NODE_WALL:
                CP_Settings_Fill(wallColor);
                break;
            case NODE_PATH:
                CP_Settings_Fill(pathColor);
                break;
            }
            CP_Graphics_DrawRect(col * gCellWidth, row * gCellHeight, gCellWidth, gCellHeight);
        }
    }
}

void Adrian_Input(void)
{
    // Toggle cell state.
    if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
    {
        float gMousePositionX = CP_Input_GetMouseWorldX();
        float gMousePositionY = CP_Input_GetMouseWorldY();

        int cellX = (int)(gMousePositionX / gCellWidth);
        int cellY = (int)(gMousePositionY / gCellHeight);

        if (startNode)
        {
            map.map[startNode->row][startNode->column].type = NODE_DEFAULT;
        }

        startNode = &map.map[cellY][cellX];
        startNode->type = NODE_START;

        Adrian_CallAStar();
    }

    if (CP_Input_MouseTriggered(MOUSE_BUTTON_2))
    {
        float gMousePositionX = CP_Input_GetMouseWorldX();
        float gMousePositionY = CP_Input_GetMouseWorldY();

        int cellX = (int)(gMousePositionX / gCellWidth);
        int cellY = (int)(gMousePositionY / gCellHeight);

        if (endNode)
        {
            map.map[endNode->row][endNode->column].type = NODE_DEFAULT;
        }

        endNode = &map.map[cellY][cellX];
        endNode->type = NODE_END;

        Adrian_CallAStar();
    }

    if (CP_Input_MouseTriggered(MOUSE_BUTTON_3))
    {
        float gMousePositionX = CP_Input_GetMouseWorldX();
        float gMousePositionY = CP_Input_GetMouseWorldY();

        int cellX = (int)(gMousePositionX / gCellWidth);
        int cellY = (int)(gMousePositionY / gCellHeight);

        if (map.map[cellY][cellX].type == NODE_WALL)
        {
            map.map[cellY][cellX].type = NODE_DEFAULT;
        }
        else if(map.map[cellY][cellX].type == NODE_DEFAULT)
        {
            map.map[cellY][cellX].type = NODE_WALL;
        }

        Adrian_CallAStar();
    }
}

void Adrian_init(void)
{
    SM_SystemsInit();

    GameObject* g = GOM_Create(CIRCLE);
    g->scale = CP_Vector_Set(20, 20);
    g->position = CP_Vector_Set(50, 20);
    RM_AddComponent(g);
    
    map.rows = NODE_SIZE;
    map.columns = NODE_SIZE;

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
}

void Adrian_update(void)
{
    SM_SystemsPreUpdate();

    //Do stuff here...
    Adrian_Input();
    Adrian_Render();

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
