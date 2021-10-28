#include "AStar.h"

/// <summary>
/// Estimates the distance between 2 nodes. Multiplied by 10 to return a nice integer number.
/// </summary>
/// <param name="currRow">Node A's row.</param>
/// <param name="currCol">Node B's row.</param>
/// <param name="destRow">Node A's column.</param>
/// <param name="destCol">Node B's column</param>
int Estimate(int currRow, int currCol, int destRow, int destCol)
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
		printf("Warning! Starting or Ending node is NULL. [LinkedList* GetPath]\n");
		return NULL;
	}

	LinkedList* openList = NULL; // List of nodes that we want to test.
	LinkedList* closedList = NULL; // List of nodes that we've already tested.
	LinkedList* path = NULL; // Return list if a valid path between the starting and ending nodes is found.

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
		int lowestFIndex = 0;
		// Loop through openList to find node with the lowest F cost.
		for (int i = 0; i < LL_GetCount(openList); ++i)
		{
			void* temp = LL_Get(openList, i);
			if (temp)
			{
				AStar_Node* tempNode = (AStar_Node*)temp;
				if (tempNode)
				{
					if (lowestF == NULL || tempNode->fCost < lowestF->fCost)
					{
						lowestF = tempNode;
						lowestFIndex = i;
					}
				}
			}
		}
		if (lowestF) // We found it!!
		{
			LL_Add(&closedList, LL_Get(openList, lowestFIndex));
			LL_RemoveIndex(&openList, lowestFIndex);

			if (lowestF->row == ending->row && lowestF->column == ending->column) // Found ending node!
			{
				// Need to add code here to backtrack to get the full path.
				printf("Found end node! Time to create path. [LinkedList* GetPath()]\n");
				while (lowestF != NULL)
				{
					printf("PNode Position = [%d,%d]\n", lowestF->row, lowestF->column);
					LL_Add(&path, lowestF);
					lowestF = lowestF->parent;
				}
				printf("Path size %d. [LinkedList* GetPath()]\n", LL_GetCount(path));
				return path;
			}

			int lowestRow = lowestF->row;
			int lowestCol = lowestF->column;
			AStar_Node* neighbour = NULL;
			// Now get neighbours of the lowestF node.
			for (int i = 0; i < directions; ++i)
			{		
				lowestRow = lowestF->row + deltaRow[i];
				lowestCol = lowestF->column + deltaCol[i];

				if (lowestRow > 0 && lowestRow < map->rows && 
					lowestCol > 0 && lowestCol < map->columns)
				{
					neighbour = &map->map[lowestRow + deltaRow[i]][lowestCol + deltaCol[i]];
					if (neighbour != NULL && neighbour->type != NODE_WALL) // Only add if the node is not a wall.
					{
						neighbours[i] = malloc(sizeof(AStar_Node));
						if (neighbours[i])
						{
							// Creating a copy here because nodes at the same position can have different gCosts.
							// Depending on how they were calculated.
							memcpy(neighbours[i], neighbour, sizeof(AStar_Node));
							neighbours[i]->parent = lowestF;
						}
					}
					else
					{
						neighbours[i] = NULL;
					}
				}
			}

			// Loop through neighbouring nodes.
			for (int i = 0; i < directions; ++i)
			{
				if (neighbours[i] == NULL)
					continue;
				
				AStar_Node* temp = NULL;
				bool skipNode = false;

				// Check if neighbouring node is in the closed list.
				for (int x = 0; x < LL_GetCount(closedList); ++x)
				{
					temp = (AStar_Node*)LL_Get(closedList, x);
					if (temp->row == neighbours[i]->row &&
						temp->column == neighbours[i]->column) // Neighbour node is already in closed list.
					{
						if (temp->gCost > neighbours[i]->gCost)
						{
							skipNode = true;
							break;
						}
					}
				}
				if (skipNode)
					continue;

				// Calculate costs for neighbouring node.
				neighbours[i]->gCost = lowestF->gCost + 1;
				neighbours[i]->hCost = Estimate(neighbours[i]->row, ending->row, neighbours[i]->column, ending->column);
				neighbours[i]->fCost = neighbours[i]->gCost + neighbours[i]->hCost;

				// Check if neighbouring node is in the open list.
				temp = NULL;
				skipNode = false;
				for (int x = 0; x < LL_GetCount(openList); ++x)
				{
					temp = (AStar_Node*)LL_Get(openList, x);
					if( temp->row == neighbours[i]->row && 
						temp->column == neighbours[i]->column) // Neighbour node is already in open list.
					{
						if (temp->gCost > neighbours[i]->gCost)
						{
							skipNode = true;
							break;
						}
					}
				}
				if (skipNode)
					continue;

				LL_Add(&openList, neighbours[i]);
			}
		}
	}

	printf("Warning! There is no valid path between the Starting and Ending nodes. [LinkedList* GetPath]\n");
	return NULL;
}
