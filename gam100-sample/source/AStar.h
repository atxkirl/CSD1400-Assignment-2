#pragma once

#include <stdio.h>
#include "LinkedList.h"

/*-------------------*/
/*	AStar Variables  */
/*-------------------*/

typedef struct
{
	int row; // Position of this Node in a map.
	int column; // Position of this Node in a map.

	int fCost; // Total cost of this node. (f = g + h)
	int gCost; // Distance travelled from start node, to get to this node.
	int hCost; // Estimated distance remaining to end node.
}AStar_Node;

typedef struct
{
	AStar_Node** map;
	int rows; // Number of rows in this map.
	int columns; // Number of columns in this map.
}AStar_Map;

int deltaRow[] = {-1, 0, 1};
int deltaCol[] = {-1, 0, 1};

// Lists needed for A*
// - Open List (stores nodes that we want to test)
// - Closed List (stores nodes that we've already tested)
// - Return List (list of path nodes, from start node to end node)


/*-------------------*/
/*	AStar Functions  */
/*-------------------*/

/// <summary>
/// Returns a path from the starting node, to the ending node.
/// Path is a LinkedList of Node*.
/// </summary>
/// <param name="starting -">Starting node.</param>
/// <param name="ending -">Ending node.</param>
/// <param name="map -">The map of which the starting and ending nodes are a part of.</param>
LinkedList* GetPath(AStar_Node* starting, AStar_Node* ending, AStar_Map* map);