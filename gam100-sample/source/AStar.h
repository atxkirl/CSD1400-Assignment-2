#pragma once

#include <stdio.h>		// For printf() and NULL
#include <stdbool.h>		// For booleans
#include <stdlib.h>		// For malloc()
#include <string.h>		// For memcpy()
#include <math.h>		// For abs() and sqrt()
#include "LinkedList.h"	// For LinkedLists
#include "../CProcessing/inc/cprocessing.h"

/*-------------------*/
/*	AStar Variables  */
/*-------------------*/

typedef enum
{
	NODE_DEFAULT,
	NODE_START,
	NODE_END,
	NODE_WALL,
	NODE_PATH
}AStar_Type;

typedef struct AStar_Node AStar_Node;
struct AStar_Node
{
	int row; // Position of this Node in a map.
	int column; // Position of this Node in a map.
	CP_Vector position; // World position of this Node.
	AStar_Type type; // Type of this node. Is it walkable? Is it a wall?

	int fCost; // Total cost of this node. (f = g + h)
	int gCost; // Distance travelled from start node, to get to this node.
	int hCost; // Estimated distance remaining to end node.
	AStar_Node* parent; // Parent node, used to "remember" the path.
};

typedef struct
{
	AStar_Node** map; // 2D array containing all the grid cells in a single level. Needs to span the entire area where pathfinding is required.
	int rows; // Number of rows in this map.
	int columns; // Number of columns in this map.
}AStar_Map;

static int directions = 8; // Number of directions to check around a node for it's neighbours.
static int deltaRow[] = { -1,  0,  1, -1, -1, 1, 1, 0 };
static int deltaCol[] = { -1, -1, -1,  0,  1, 0, 1, 1 };
static AStar_Node* neighbours[8];
static float const positionToNodeSnap = 33.f; // How close must a world position be to a node's position to count as "inside the node".

//static int directions = 4; // Number of directions to check around a node for it's neighbours.
//static int deltaRow[] = { -1, 0, 0, 1 };
//static int deltaCol[] = { 0, -1, 1, 0 };
//static AStar_Node* neighbours[4];


/*-------------------*/
/*	AStar Functions  */
/*-------------------*/

static int Estimate(int currRow, int currCol, int destRow, int destCol);

/// <summary>
/// Returns a path from the starting node, to the ending node.
/// Path is a LinkedList of Node*.
/// </summary>
/// <param name="starting -">Starting node.</param>
/// <param name="ending -">Ending node.</param>
/// <param name="map -">The map of which the starting and ending nodes are a part of.</param>
LinkedList* AStar_GetPath(AStar_Node* starting, AStar_Node* ending, AStar_Map* map);

LinkedList* AStar_GetPathWorldPosition(float startX, float startY, float endX, float endY, AStar_Map* map);

void AStar_InitializeNode(AStar_Node** node, int row, int col, float posX, float posY, AStar_Type type);

void AStar_InitializeMap(AStar_Map* map, int row, int col);

void AStar_ClearMap(AStar_Map* map);
