/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file	    Adrian.c
@author	    Adrian Tan (t.xingkhiangadrian)
@course	    CSD1400 Software Engineering Project 1
@Team	    BananaBoi
@date	    12/10/2021
@brief	    TEST scene to test out A* pathfinding algorithm by displaying a grid and calling 
            A* to get a path between 2 selected grid cells.
*//*________________________________________________________________________________________*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "DetectMemoryLeak.h"
#include "SystemManager.h"
#include "AStar.h"
#include "SceneManager.h"

#define NODE_SIZE 30

static AStar_Map map;
static LinkedList* path;

static float gWindowWidth;
static float gWindowHeight;

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

/// <summary>
/// Calls A* pathfinding to calculate a path between the startNode and endNode, and calculates the time taken in milliseconds.
/// </summary>
void Adrian_CallAStar(void)
{
    // Clear the map of paths
    for (int row = 0; row < map.rows; ++row)
    {
        for (int col = 0; col < map.columns; ++col)
        {
            if (map.map[row][col].type == NODE_PATH)
                map.map[row][col].type = NODE_DEFAULT;
            map.map[row][col].gCost = 0;
            map.map[row][col].hCost = 0;
            map.map[row][col].fCost = 0;
            map.map[row][col].parent = NULL;
        }
    }

    clock_t t = clock();
    // Calculate A* path
    LL_Clear(&path);
    AStar_GetPath(startNode, endNode, &path, &map);
    t = clock() - t;
    printf("Time taken for A* (ms): %d\n", t);
}

/// <summary>
/// Renders the grid every frame.
/// </summary>
void Adrian_Render(void)
{
    // Clear the window.
    CP_Graphics_ClearBackground(gBackgroundColor);

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
            CP_Graphics_DrawRect(map.map[row][col].position.x, map.map[row][col].position.y, gCellWidth, gCellHeight);
        }
    }

    // Set map nodes to be colored.
    if (path)
    {
        CP_Settings_Fill(pathColor);
        for (int i = 0; i < LL_GetCount(path); ++i)
        {
            AStar_Node* temp = (AStar_Node*)LL_Get(path, i);
            if (temp)
            {
                
                CP_Graphics_DrawRect(temp->position.x, temp->position.y, gCellWidth, gCellHeight);
            }
        }
        CP_Settings_Fill(startColor);
        CP_Graphics_DrawRect(startNode->position.x, startNode->position.y, gCellWidth, gCellHeight);

        CP_Settings_Fill(endColor);
        CP_Graphics_DrawRect(endNode->position.x, endNode->position.y, gCellWidth, gCellHeight);
    }
}

/// <summary>
/// Handles user inputs.
/// </summary>
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

        if (map.map[cellY][cellX].type != NODE_DEFAULT)
        {
            map.map[cellY][cellX].type = NODE_DEFAULT;
        }
        else if(map.map[cellY][cellX].type == NODE_DEFAULT)
        {
            map.map[cellY][cellX].type = NODE_WALL;
        }

        Adrian_CallAStar();
    }

    // Toggle Dialogue box.
    if (CP_Input_KeyDown(KEY_E))
    {
        DM_PrintDialogue("Woah!", DIALOGUE_CLOSEBUTTON);
    }
    else if (CP_Input_KeyDown(KEY_F))
    {
        DM_PrintDialogue("Woah look, a temporary box!", DIALOGUE_LOOKINGAT);
    }
    else if (CP_Input_KeyDown(KEY_G))
    {
        DM_PrintDialogue("Woah look, a temporary box, and the text is so freaking long wow!", DIALOGUE_LOOKINGAT);
    }

    // Exit Game
    if (CP_Input_KeyTriggered(KEY_ESCAPE))
    {
        //CP_Engine_Terminate();
        SceneManager_ChangeSceneByName("mainmenu");
    }
}

/// <summary>
/// Initializes scene variables as well as A* system.
/// </summary>
void Adrian_init(void)
{
    SM_SystemsInit();

    gWindowWidth = (float)CP_System_GetWindowWidth();
    gWindowHeight = (float)CP_System_GetWindowHeight();
    gCellHeight = gWindowHeight / NODE_SIZE;
    gCellWidth = gWindowWidth / NODE_SIZE;

    gBackgroundColor = CP_Color_Create(128, 128, 128, 255);
    gGridColor = CP_Color_Create(0, 0, 0, 255);

    defaultColor = CP_Color_Create(128, 128, 128, 255);
    startColor = CP_Color_Create(0, 255, 0, 255);
    endColor = CP_Color_Create(255, 0, 0, 255);
    wallColor = CP_Color_Create(0, 0, 0, 255);
    pathColor = CP_Color_Create(255, 255, 255, 255);

    // Allocate Memory for map.
    AStar_InitializeMap(&map, 30, 30);

    // Fill map with stuff
    for (int rows = 0; rows < map.rows; ++rows)
    {
        for (int cols = 0; cols < map.columns; ++cols)
        {
            AStar_Node* temp = &map.map[rows][cols];
            AStar_InitializeNode(&temp, rows, cols, CP_Vector_Set(cols * gCellWidth, rows * gCellHeight), NODE_DEFAULT);
        }
    }
}

/// <summary>
/// Update loop called every frame.
/// </summary>
void Adrian_update(void)
{
    SM_SystemsPreUpdate();

    //Do stuff here...
    Adrian_Input();
    Adrian_Render();

    SM_SystemsUpdate(0);
    CP_Settings_Fill(CP_Color_Create(255, 255, 128, 255));

    SM_SystemsLateUpdate();
    CP_Settings_Fill(CP_Color_Create(128, 128, 128, 255));
}

/// <summary>
/// Exit function that's called before scene change.
/// </summary>
void Adrian_exit(void)
{
    // Release memory for map.map
    AStar_ClearMap(&map);
    if (path)
        LL_Clear(&path);
    startNode = NULL;
    endNode = NULL;

    SM_SystemsExit();
    _CrtDumpMemoryLeaks();
    printf("Exitting Scene Adrian!\n");
}

/// <summary>
/// Called by SceneManager to pass function pointers for this scene's init, update and exit.
/// </summary>
/// <param name="init">Function pointer to scene init.</param>
/// <param name="update">Function pointer to scene update.</param>
/// <param name="exit">Funciton pointer to scene exit.</param>
void Adrian_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = Adrian_init;
    *update = Adrian_update;
    *exit = Adrian_exit;
}
