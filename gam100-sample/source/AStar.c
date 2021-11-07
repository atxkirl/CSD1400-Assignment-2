#include "AStar.h"

/// <summary>
/// Estimates the distance between 2 nodes. Multiplied by 10 to return a nice integer number.
/// </summary>
/// <param name="currRow">Node A's row.</param>
/// <param name="currCol">Node B's row.</param>
/// <param name="destRow">Node A's column.</param>
/// <param name="destCol">Node B's column</param>
static int Estimate(int currRow, int currCol, int destRow, int destCol)
{
	double dRow = (double)abs(currRow - destRow);
	double dCol = (double)abs(currCol - destCol);
	return (int)(10.0 * sqrt(dRow * dRow + dCol * dCol));
}

/// <summary>
/// Returns a path from the starting node, to the ending node.
/// Path is a LinkedList of Node*.
/// </summary>
/// <param name="starting -">Starting node.</param>
/// <param name="ending -">Ending node.</param>
/// <param name="map -">The map of which the starting and ending nodes are a part of.</param>
LinkedList* AStar_GetPath(AStar_Node* starting, AStar_Node* ending, AStar_Map* map)
{
	if (starting == NULL || ending == NULL)
	{
		//printf("Warning! Starting or Ending node is NULL. [LinkedList* GetPath]\n");
		return NULL;
	}

	LinkedList* openList = NULL; // List of nodes that we want to test.
	LinkedList* closedList = NULL; // List of nodes that we've already tested.
	LinkedList* path = NULL; // Return list if a valid path between the starting and ending nodes is found.
	starting->hCost = Estimate(starting->row, starting->column, ending->row, ending->column);
	starting->fCost = starting->hCost;

	LL_Add(&openList, starting);

	// Loop while openList still contains nodes. (Means there's still nodes to test)
	// - Get the node with the lowest F cost. This is our "current" node.
	// - Push "current" node to the closed list.
	// - For each of the neighbours surrounding the "current" node:
		// - if neighbour node is on the closed list, ignore it.
		// - if neighbour node is not walkable, ignore it.
		// - if neighbour node is not in the openList, add it to the openList.
		// - else if neighbour node is already on the openList, check if the path to it is better.
	// - If ending node is added to the closed list, means we've found a complete path.
	// - If openList is empty, and we've not added ending node to the closed list, means there is no path.
	while (LL_IsEmpty(openList) == 0)
	{
		AStar_Node* lowestF = NULL;
		for (LinkedList* it = openList; it != NULL; it = it->next)
		{
			if (lowestF == NULL || lowestF->fCost > ((AStar_Node*)it->curr)->fCost)
				lowestF = (AStar_Node*)it->curr;
		}

		// Skip this iteration if a valid lowestF node is not found.
		if (lowestF == NULL)
			continue;

		LL_RemovePtr(&openList, lowestF);
		LL_Add(&closedList, lowestF);

		// Found end node, so now create path.
		if (lowestF == ending)
		{
			// Add nodes to path list, in reverse order.
			for (; lowestF; lowestF = lowestF->parent)
			{
				if (lowestF->type != NODE_START && lowestF->type != NODE_END)
					lowestF->type = NODE_PATH;
				LL_Add(&path, lowestF);
			}
			LL_Clear(&openList);
			LL_Clear(&closedList);
			return path;
		}

		// Get neighbouring nodes.
		int lowestRow = lowestF->row;
		int lowestCol = lowestF->column;
		for (int i = 0; i < directions; ++i)
		{
			lowestRow = lowestF->row + deltaRow[i];
			lowestCol = lowestF->column + deltaCol[i];
			
			// Do not look for neighbours if out of bounds.
			if (lowestRow < 0 || lowestRow >= map->rows ||
				lowestCol < 0 || lowestCol >= map->columns)
				continue;

			AStar_Node* neighbour = &map->map[lowestRow][lowestCol];
			
			// Skip walls and nodes in the closed list.
			if (neighbour->type == NODE_WALL)
				continue;
			if (LL_ContainsPtr(closedList, neighbour))
				continue;

			int gCost, hCost, fCost;
			if (i % 2 == 0)
				gCost = lowestF->gCost + 14;
			else
				gCost = lowestF->gCost + 10;
			hCost = Estimate(neighbour->row, neighbour->column, ending->row, ending->column);
			fCost = gCost + hCost;

			if (LL_ContainsPtr(openList, neighbour) == 0 || fCost < neighbour->fCost)
			{
				neighbour->gCost = gCost;
				neighbour->hCost = hCost;
				neighbour->fCost = fCost;
				neighbour->parent = lowestF;

				LL_SetAdd(&openList, neighbour);
			}
		}
	}

	//printf("Warning! There is no valid path between the Starting and Ending nodes. [LinkedList* GetPath]\n");
	LL_Clear(&openList);
	LL_Clear(&closedList);
	LL_Clear(&path);
	return NULL;
}