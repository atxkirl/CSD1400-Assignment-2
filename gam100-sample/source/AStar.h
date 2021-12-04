/*
@file	AStar.h
@author	Adrian Tan (t.xingkhiangadrian)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@date	27/10/2021
@brief	Contains functions to implement the AStar Pathfinding algorithm.
*//*______________________________________________________________________*/

#pragma once

#include <stdio.h>		// For printf() and NULL
#include <stdbool.h>	// For booleans
#include <stdlib.h>		// For malloc()
#include <string.h>		// For memcpy()
#include <math.h>		// For abs() and sqrt()
#include "LinkedList.h"	// For LinkedLists
#include "Helpers.h"	// For RAND
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
	NODE_PATH,
	NODE_HIDEABLE
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

static float const positionToNodeSnap = 33.f; // How close must a world position be to a node's position to count as "inside the node".

static const int directions = 8; // Number of directions to check around a node for it's neighbours.
static const int deltaRow[] = { -1, 0, 1, 1, 1, 0,-1,-1 }; // Delta to offset a Node's row values by, to find neighbouring nodes.
static const int deltaCol[] = { -1,-1,-1, 0, 1, 1, 1, 0 }; // Delta to offset a Node's column values by, to find neighbouring nodes.
static const int deltaRowNeighbour[] = { 1, 0,-1, 0,-1, 0, 1 };
static const int deltaColNeighbour[] = { 1, 0, 1, 0,-1, 0,-1 };
static AStar_Node* neighbours[8];

//static const int directions = 4; // Number of directions to check around a node for it's neighbours.
//static const int deltaRow[] = { -1, 0, 0, 1 }; // Delta to offset a Node's row values by, to find neighbouring nodes.
//static const int deltaCol[] = { 0, -1, 1, 0 }; // Delta to offset a Node's column values by, to find neighbouring nodes.
//static const int deltaVert[] = { 0, 0, 0, 0}; // Delta to offset a diagonal Node's row values by, to find it's cardinal neighbours. Used for calculating diagonal wall pathing.
//static const int deltaHori[] = { 0, 0, 0, 0}; // Delta to offset a diagonal Node's column values by, to find it's cardinal neighbours. Used for calculating diagonal wall pathing.
//static AStar_Node* neighbours[4];


/*-------------------*/
/*	AStar Functions  */
/*-------------------*/

/// <summary>
/// Estimates the distance between 2 nodes. Multiplied by 10 to return a nice integer number.
/// </summary>
/// <param name="currRow">Node A's row.</param>
/// <param name="currCol">Node B's row.</param>
/// <param name="destRow">Node A's column.</param>
/// <param name="destCol">Node B's column</param>
static int Estimate(int currRow, int currCol, int destRow, int destCol);

/// <summary>
/// Clears the parents of all nodes within a list.
/// </summary>
/// <param name="list -">The list to clear.</param>
static void OrphaniseList(LinkedList* list);

/// <summary>
/// Calculates a path from the starting node to the ending node, both of which must be part of the map grid.
/// </summary>
/// <param name="starting -">Pointer to the starting node.</param>
/// <param name="ending -">Pointer to the end node.</param>
/// <param name="path -">Pointer to the path list that we want to populate.</param>
/// <param name="map -">The overall map that contains all the nodes that we can traverse.</param>
void AStar_GetPath(AStar_Node* starting, AStar_Node* ending, LinkedList** path, AStar_Map* map);

/// <summary>
/// Calculates a path from the starting node to the ending node, using world coordinates to approximate the start and end nodes.
/// </summary>
/// <param name="startPo -">World position to start pathing from.</param>
/// <param name="endPos -">World position to path towards.</param>
/// <param name="path -">Pointer to the path list that we want to populate.</param>
/// <param name="map -">The overall map that contains all the nodes that we can traverse.</param>
void AStar_GetPathWorldPosition(CP_Vector startPos, CP_Vector endPos, LinkedList** path, AStar_Map* map);

/// <summary>
/// Function to estimate the row and column of a world position.
/// </summary>
/// <param name="position -">World position that we want to estimate the grid position of.</param>
/// <param name="map -">The overall map that contains all nodes.</param>
/// <param name="row -">Reference to the row of the estimated node. This function will assign the value of the estimated row to this.</param>
/// <param name="col -">Reference to the column of the estimated node. This function will assign the value of the estimated column to this.</param>
bool AStar_GetRowCol(CP_Vector position, AStar_Map* map, int* row, int* col);

/// <summary>
/// Returns the location of a valid floor Tile in a map.
/// </summary>
/// <param name="position -">World position to search around from.</param>
/// <param name="map -">The overall map that contains all the nodes.</param>
/// <param name="radiusMin -">Minimum radius from position.</param>
/// <param name="radiusMax -">Maximum radius from position.</param>
void AStar_GetTile(CP_Vector* returnPosition, CP_Vector epicenter, AStar_Map* map, int radiusMin, int radiusMax);

/// <summary>
/// Initializes the value of a Node.
/// </summary>
/// <param name="node -">Reference to a node pointer, so we can modify the values of it.</param>
/// <param name="row -">New value of this node's row.</param>
/// <param name="col -">New value of this node's column.</param>
/// <param name="position -">World position of this node.</param>
/// <param name="type -">Type of this node. Used in pathfinding for walkable/unwalkable tiles.</param>
void AStar_InitializeNode(AStar_Node** node, int row, int col, CP_Vector position, AStar_Type type);

/// <summary>
/// Allocates memory for a 2D array of nodes, used to represent all the pathable tiles in a level.
/// </summary>
/// <param name="map -">Pointer to the map struct to populate.</param>
/// <param name="row -">Number of rows for the 2D array.</param>
/// <param name="col -">Number of columns for the 2D array.</param>
void AStar_InitializeMap(AStar_Map* map, int row, int col);

/// <summary>
/// Frees the memory allocated to the provided map. Call this at the exit of any scene that has pathfinding.
/// </summary>
/// <param name="map">Pointer to the map to clear.</param>
void AStar_ClearMap(AStar_Map* map);
