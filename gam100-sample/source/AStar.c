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
/// Clears the parents of all nodes within a list.
/// </summary>
/// <param name="list -">The list to clear.</param>
static void OrphaniseList(LinkedList* list)
{
	if (list)
	{
		LinkedList* ll = list;
		while (ll)
		{
			AStar_Node* temp = (AStar_Node*)ll->curr;
			if (temp)
				temp->parent = NULL;

			ll = ll->next;
		}
	}
}

/// <summary>
/// Calculates a path from the starting node to the ending node, both of which must be part of the map grid.
/// </summary>
/// <param name="starting -">Pointer to the starting node.</param>
/// <param name="ending -">Pointer to the end node.</param>
/// <param name="path -">Pointer to the path list that we want to populate.</param>
/// <param name="map -">The overall map that contains all the nodes that we can traverse.</param>
void AStar_GetPath(AStar_Node* starting, AStar_Node* ending, LinkedList** path, AStar_Map* map)
{
	if (starting == NULL || ending == NULL)
	{
		printf("Warning! Starting or Ending node is NULL. [LinkedList* GetPath]\n");
		return;
	}
	if (starting == ending)
	{
		printf("Warning! Starting and Ending node is the same. [LinkedList* GetPath]\n");
		return;
	}

	printf("Calculating new path... Start [%d, %d]  End [%d, %d]\n", starting->row, starting->column, ending->row, ending->column);

	LinkedList* openList = NULL; // List of nodes that we want to test.
	LinkedList* closedList = NULL; // List of nodes that we've already tested.
	starting->hCost = Estimate(starting->row, starting->column, ending->row, ending->column);
	starting->fCost = starting->hCost;

	LL_Add(&openList, starting);
	LL_Clear(path);

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
			// Prev node to check for circular parenting.
			AStar_Node* prevNode = lowestF;
			// Add nodes to path list, in reverse order.
			for (; lowestF; lowestF = lowestF->parent)
			{
				if (prevNode == lowestF->parent)
				{
					printf("Circular parenting!\n");
					break;
				}
				printf("path\n");
				LL_Add(path, lowestF);
				prevNode = lowestF;
			}
			LL_Reverse(path);

			// Remove parents from all the nodes in these lists, to prevent any future parenting errors when calling for pathfinding.
			OrphaniseList(*path);
			OrphaniseList(openList);
			OrphaniseList(closedList);

			LL_Clear(&openList);
			LL_Clear(&closedList);
			return;
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

			// Skip walls.
			if (neighbour->type == NODE_WALL)
				continue;
			// Skip nodes in the closed list, aka "already checked".
			if (LL_ContainsPtr(closedList, neighbour))
				continue;
			// Check diagonal if they have walls for neighbours.
			int cornerRow = lowestRow + deltaRowNeighbour[i];
			int cornerCol = lowestCol + deltaColNeighbour[i];
			if ((&map->map[lowestRow][cornerCol])->type == NODE_WALL || (&map->map[cornerRow][lowestCol])->type == NODE_WALL)
				continue;

			// Calculate costs for neighbouring nodes.
			int gCost, hCost, fCost;
			if (i % 2 == 0)
				gCost = lowestF->gCost + 14; // Diagonal nodes cost more than cardinal nodes.
			else
				gCost = lowestF->gCost + 10;
			hCost = Estimate(neighbour->row, neighbour->column, ending->row, ending->column); // Estimate remaining distance to the ending node.
			fCost = gCost + hCost;

			// Add neighbour node
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

	printf("Warning! There is no valid path between the Starting and Ending nodes. [LinkedList* GetPath]\n");

	// Remove parents from all the nodes in these lists, to prevent any future parenting errors when calling for pathfinding.
	OrphaniseList(openList);
	OrphaniseList(closedList);

	LL_Clear(&openList);
	LL_Clear(&closedList);
	LL_Clear(path);
}

/// <summary>
/// Calculates a path from the starting node to the ending node, using world coordinates to approximate the start and end nodes.
/// </summary>
/// <param name="startPo -">World position to start pathing from.</param>
/// <param name="endPos -">World position to path towards.</param>
/// <param name="path -">Pointer to the path list that we want to populate.</param>
/// <param name="map -">The overall map that contains all the nodes that we can traverse.</param>
void AStar_GetPathWorldPosition(CP_Vector startPos, CP_Vector endPos, LinkedList** path, AStar_Map* map)
{
	int startRow, startCol, endRow, endCol;
	startRow = startCol = endRow = endCol = -1;

	AStar_GetRowCol(startPos, map, &startRow, &startCol);
	AStar_GetRowCol(endPos, map, &endRow, &endCol);

	if (startRow > -1 && startCol > -1 && endRow > -1 && endCol > -1)
	{
		AStar_GetPath(&map->map[startRow][startCol], &map->map[endRow][endCol], path, map);
	}
}

/// <summary>
/// Function to estimate the row and column of a world position.
/// </summary>
/// <param name="position -">World position that we want to estimate the grid position of.</param>
/// <param name="map -">The overall map that contains all nodes.</param>
/// <param name="row -">Reference to the row of the estimated node. This function will assign the value of the estimated row to this.</param>
/// <param name="col -">Reference to the column of the estimated node. This function will assign the value of the estimated column to this.</param>
bool AStar_GetRowCol(CP_Vector position, AStar_Map* map, int* row, int* col)
{
	float deltaX, deltaY;
	for (int r = 0; r < map->rows; ++r)
	{
		for (int c = 0; c < map->columns; ++c)
		{
			deltaX = (float)fabs((double)map->map[r][c].position.x - (double)position.x);
			deltaY = (float)fabs((double)map->map[r][c].position.y - (double)position.y);
			if (deltaX < positionToNodeSnap && deltaY < positionToNodeSnap)
			{
				*row = r;
				*col = c;
				return true;
			}
		}
	}
	printf("Unable to find a Tile near position [%4.2f,%4.2f]!\n", position.x, position.y);
	return false;
}

/// <summary>
/// Returns the location of a valid floor Tile in a map.
/// </summary>
/// <param name="position -">World position to search around from.</param>
/// <param name="map -">The overall map that contains all the nodes.</param>
/// <param name="radiusMin -">Minimum radius from position.</param>
/// <param name="radiusMax -">Maximum radius from position.</param>
void AStar_GetTile(CP_Vector* returnPosition, CP_Vector epicenter, AStar_Map* map, int radiusMin, int radiusMax)
{
	// Pick a location around the position.
	int dRow, dCol;
	int negativeRow, negativeCol;
	int row, col;
	int detectInfLoop = 0;

	// Get the row and column values of the Player.
	if (!AStar_GetRowCol(epicenter, map, &row, &col))
	{
		printf("AStar_GetTile() was unable to get a valid Tile! Returning the input position instead.\n");
		return;
	}

	// Look to find valid position in map
	while (true)
	{
		if (detectInfLoop++ >= INFINITE_LOOP)
		{
			printf("WARNING! Stuck in an infinite loop!!!\n");
			break;
		}

		dRow = RAND(radiusMin, radiusMax);
		dCol = RAND(radiusMin, radiusMax);
		negativeRow = RAND(0, 1);
		negativeCol = RAND(0, 1);

		// Look for valid row number
		while (1)
		{
			if (negativeRow && (row - dRow) >= 0)
			{
				row -= dRow;
				break;
			}
			else if ((row + dRow) < map->rows)
			{
				row += dRow;
				break;
			}
			dRow = RAND(radiusMin, radiusMax);
		}

		// Look for valid col number
		while (1)
		{
			if (negativeCol && (col - dCol) >= 0)
			{
				col -= dCol;
				break;
			}
			else if ((col + dCol) < map->columns)
			{
				col += dCol;
				break;
			}
			dCol = RAND(radiusMin, radiusMax);
		}

		// Check if the given row/col is a wall or not.
		if (map->map[row][col].type == NODE_WALL)
			continue;

		// Passed all the tests, now we can set the target position!
		printf("Found valid location!\n");
		*returnPosition = map->map[row][col].position;
		return;
	}

	printf("AStar_GetTile() was unable to get a valid Tile! Returning the input position instead.\n");
}

/// <summary>
/// Initializes the value of a Node.
/// </summary>
/// <param name="node -">Reference to a node pointer, so we can modify the values of it.</param>
/// <param name="row -">New value of this node's row.</param>
/// <param name="col -">New value of this node's column.</param>
/// <param name="position -">World position of this node.</param>
/// <param name="type -">Type of this node. Used in pathfinding for walkable/unwalkable tiles.</param>
void AStar_InitializeNode(AStar_Node** node, int row, int col, CP_Vector position, AStar_Type type)
{
	(*node)->row = row;
	(*node)->column = col;
	(*node)->position = position;
	
	(*node)->parent = NULL;
	(*node)->gCost = 0;
	(*node)->hCost = 0;
	(*node)->fCost = 0;

	(*node)->type = type;
}

/// <summary>
/// Allocates memory for a 2D array of nodes, used to represent all the pathable tiles in a level.
/// </summary>
/// <param name="map -">Pointer to the map struct to populate.</param>
/// <param name="row -">Number of rows for the 2D array.</param>
/// <param name="col -">Number of columns for the 2D array.</param>
void AStar_InitializeMap(AStar_Map* map, int row, int col)
{
	// Don't bother if row/col is smaller than 1, if not malloc will explode.
	if (row < 1 || col < 1)
		return;

	map->map = malloc(sizeof(AStar_Node*) * row);
	if (map->map)
	{
		for (int r = 0; r < row; ++r)
		{
			map->map[r] = malloc(sizeof(AStar_Node) * col);
		}
	}

	map->rows = row;
	map->columns = col;
}

/// <summary>
/// Frees the memory allocated to the provided map. Call this at the exit of any scene that has pathfinding.
/// </summary>
/// <param name="map">Pointer to the map to clear.</param>
void AStar_ClearMap(AStar_Map* map)
{
	if (map->map != NULL)
	{
		for (int row = 0; row < map->rows; ++row)
		{
			free(map->map[row]);
			map->map[row] = NULL;
		}
		free(map->map);
		map->map = NULL;
	}
}
