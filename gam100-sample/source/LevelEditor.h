/*!
@file            LevelEditor.h
@author          Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course          CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief           This file contains the functions for editing the levels.
*//*______________________________________________________________________*/

#include "../CProcessing/inc/cprocessing.h"
#include "GameObject.h"
// For sizing
#define NumGrids 30

typedef struct LinkedList LinkedList;

enum GridNodeState
{
	NotVisited = 0,
	Visited,
};

typedef struct GridNode
{
	int Curr;
	int Up;
	int Down;
	int Left;
	int Right;
} GridNode;

typedef struct Grid
{
	GameObject* gGrid[NumGrids][NumGrids];
	GridNode nGrid[NumGrids][NumGrids];
} Grid;

/// <summary>
/// Initialises the variables of this file.
/// </summary>
void LevelEditorInit();

/// <summary>
/// Update.
/// </summary>
void LevelEditorUpdate();

/// <summary>
/// Exit.
/// </summary>
void LevelEditorExit();

/// <summary>
/// Places the object based on mouse input.
/// </summary>
void PlaceObject();

/// <summary>
/// Checks the tile at the current position if it is the same iObjType.
/// Assigns the type to the tile if it is not the same.
/// </summary>
/// <param name="fPositionX"></param>
/// <param name="fPositionY"></param>
/// <param name="iObjType - the GameObject type"></param>
void CheckGrid(float fPositionX, float fPositionY, int iObjType);

/// <summary>
/// Renders the GameObjects.
/// </summary>
void RenderObjects();

/// <summary>
/// Saves the current grid to .txt
/// </summary>
void SaveGrid();

/// <summary>
/// Map Generation based on Prims Algorithm.
/// Modified to fit the needs of the project.
/// </summary>
void AutoGenerateGrid();

/// <summary>
/// Checks the surrounding and adds neighbours for the Map Generation to traverse.
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="List"></param>
void AddFrontierCell(int x, int y, LinkedList* List);

/// <summary>
/// Prints the current Object Type for user reference.
/// </summary>
/// <param name="iObjType"></param>
void PrintCurrentType(int iObjType);

/// <summary>
/// Prints the current Object Direction for user reference.
/// </summary>
/// <param name="iObjDirection"></param>
void PrintCurrentDirection(int iObjDirection);

/// <summary>
/// Assigns a direction to the Tile at current position.
/// </summary>
/// <param name="fPositionX"></param>
/// <param name="fPositionY"></param>
/// <param name="iDirection"></param>
void AssignDirection(float fPositionX, float fPositionY, int iDirection);

/// <summary>
/// Loads the tile images based on the Object Type.
/// </summary>
void LoadTileImage();

/// <summary>
/// Loads previously saved map.
/// </summary>
void LoadSavedMap();