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
		printf("Warning! Starting or Ending node is NULL. [LinkedList* GetPath]\n");
		return NULL;
	}

	LinkedList* openList = NULL; // List of nodes that we want to test.
	LinkedList* closedList = NULL; // List of nodes that we've already tested.
	LinkedList* path = NULL; // Return list if a valid path between the starting and ending nodes is found.
	LinkedList* isacopy = NULL;

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
		//int lowestFIndex = 0;
		// Loop through openList to find node with the lowest F cost.
		for (LinkedList* it = openList; it != NULL; it = it->next)
		{
			if (lowestF == NULL || lowestF->fCost > ((AStar_Node*)it->curr)->fCost)
			{
				lowestF = (AStar_Node*)it->curr;
			}
		}

		if (lowestF) // We found it!!
		{
			int sizeOl = LL_GetCount(openList);
			sizeOl = LL_GetCount(closedList);
			
			LL_RemovePtr(&openList, lowestF);
			LL_Add(&closedList, lowestF);

			if (lowestF->row == ending->row && lowestF->column == ending->column) // Found ending node!
			{
				// Need to add code here to backtrack to get the full path.
				printf("Found end node! Time to create path. [LinkedList* GetPath()]\n");
				printf("Found end node! OpenList size = %d [LinkedList* GetPath()]\n", LL_GetCount(openList));
				printf("Found end node! ClosedList size = %d [LinkedList* GetPath()]\n", LL_GetCount(closedList));

				while (lowestF != NULL)
				{
					printf("PNode Position = [%d,%d]\n", lowestF->row, lowestF->column);

					AStar_Node* mapNode = &map->map[lowestF->row][lowestF->column];
					mapNode->fCost = lowestF->fCost;
					mapNode->gCost = lowestF->gCost;
					mapNode->hCost = lowestF->hCost;
					mapNode->parent = lowestF->parent;
					if (mapNode->type != NODE_START && mapNode->type != NODE_END)
						mapNode->type = NODE_PATH;

					LL_Add(&path, mapNode);
					lowestF = lowestF->parent;
				}
				LL_Reverse(&path);

				if (isacopy)
				{
					for (; isacopy->next != NULL; isacopy = isacopy->next)
						free(isacopy->curr);
					free(isacopy->curr);
					isacopy = GetHead(isacopy);
					LL_Clear(&isacopy);
				}
				LL_Clear(&openList);
				LL_Clear(&closedList);

				for (int i = 0; i < 8; ++i)
					neighbours[i] = NULL;

				return path;
			}

			int lowestRow = lowestF->row;
			int lowestCol = lowestF->column;
			AStar_Node* neighbour = NULL;
			for (int i = 0; i < 8; ++i)
				neighbours[i] = NULL;
			// Now get neighbours of the lowestF node.
			for (int i = 0; i < directions; ++i)
			{		
				lowestRow = lowestF->row + deltaRow[i];
				lowestCol = lowestF->column + deltaCol[i];

				if (lowestRow >= 0 && lowestRow < map->rows && 
					lowestCol >= 0 && lowestCol < map->columns)
				{
					neighbour = &map->map[lowestRow][lowestCol];
					if (neighbour->type != NODE_WALL) // Only add if the node is not a wall.
					{
						neighbours[i] = malloc(sizeof(AStar_Node));
						if (neighbours[i])
						{
							// Creating a copy here because nodes at the same position can have different gCosts.
							// Depending on how they were calculated.
							//memcpy(neighbours[i], neighbour, sizeof(AStar_Node));
							neighbours[i]->column = neighbour->column;
							neighbours[i]->row = neighbour->row;
							neighbours[i]->type = neighbour->type;
							neighbours[i]->fCost = neighbour->fCost;
							neighbours[i]->gCost = neighbour->gCost;
							neighbours[i]->hCost = neighbour->hCost;
							neighbours[i]->parent = lowestF;
							LL_Add(&isacopy, neighbours[i]);
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
				for (LinkedList* it = closedList; it; it = it->next)
				{
					temp = (AStar_Node*)it->curr;
					if (temp->row == neighbours[i]->row &&
						temp->column == neighbours[i]->column) // Neighbour node is already in closed list.
					{
						skipNode = true;
						break;
					}
				}
				if (skipNode)
					continue;

				// Calculate costs for neighbouring node.
				if (i % 2 == 0)
					neighbours[i]->gCost = lowestF->gCost + 14;
				else
					neighbours[i]->gCost = lowestF->gCost + 10;
				neighbours[i]->hCost = Estimate(neighbours[i]->row, neighbours[i]->column, ending->row, ending->column);
				neighbours[i]->fCost = neighbours[i]->gCost + neighbours[i]->hCost;

				// Check if neighbouring node is in the open list.
				temp = NULL;
				skipNode = false;
				for (LinkedList* it = openList; it; it = it->next)
				{
					temp = (AStar_Node*)it->curr;
					if (temp->row == neighbours[i]->row &&
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
	if (isacopy)
	{
		for (; isacopy->next != NULL; isacopy = isacopy->next)
			free(isacopy->curr);
		free(isacopy->curr);
		isacopy = GetHead(isacopy);
		LL_Clear(&isacopy);
	}
	LL_Clear(&openList);
	LL_Clear(&closedList);
	for (int i = 0; i < 8; ++i)
		neighbours[i] = NULL;

	return NULL;
}
