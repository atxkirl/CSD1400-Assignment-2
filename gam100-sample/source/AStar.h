#pragma once

#include <stdio.h>		// For printf() and NULL
#include <stdbool.h>		// For booleans
#include <stdlib.h>		// For malloc()
#include <string.h>		// For memcpy()
#include <math.h>		// For abs() and sqrt()
#include "LinkedList.h"	// For LinkedLists

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
	AStar_Type type; // Type of this node. Is it walkable? Is it a wall?

	int fCost; // Total cost of this node. (f = g + h)
	int gCost; // Distance travelled from start node, to get to this node.
	int hCost; // Estimated distance remaining to end node.
	AStar_Node* parent; // Parent node, used to "remember" the path.
};

typedef struct
{
	AStar_Node** map; // NOTE: Convert this to a 1D array, then use row/col values to simulate a 2D array.
	int rows; // Number of rows in this map.
	int columns; // Number of columns in this map.
}AStar_Map;

static int directions = 8; // Number of directions to check around a node for it's neighbours.
static int deltaRow[] = { -1,  0,  1, -1, -1, 1, 1, 0 };
static int deltaCol[] = { -1, -1, -1,  0,  1, 0, 1, 1 };
static AStar_Node* neighbours[8];

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
