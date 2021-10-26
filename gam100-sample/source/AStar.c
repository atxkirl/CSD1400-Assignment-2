#include "AStar.h"

/// <summary>
/// Returns a path from the starting node, to the ending node.
/// Path is a LinkedList of Node*.
/// </summary>
/// <param name="starting -">Starting node.</param>
/// <param name="ending -">Ending node.</param>
/// <param name="map -">The map of which the starting and ending nodes are a part of.</param>
LinkedList* GetPath(AStar_Node* starting, AStar_Node* ending, AStar_Map* map)
{
	if (starting == NULL || ending == NULL)
	{
		printf("Warning! Starting or Ending node is NULL. [LinkedList* GetPath]\n");
		return NULL;
	}

	LinkedList* openList = NULL; // List of nodes that we want to test.
	LinkedList* closedList = NULL; // List of nodes that we've already tested.
	//AStar_Node* neighbours[8];

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
		for (int i = 0; i < LL_GetCount(openList); ++i) // Loop through openList to find node with the lowest F cost.
		{
			LinkedList* temp = LL_Get(openList, i);
			if (temp)
			{
				AStar_Node* tempNode = (AStar_Node*)temp->curr;
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
			LL_Add(&closedList, LL_Get(openList, lowestFIndex)->curr);
			LL_RemoveIndex(&openList, lowestFIndex);

			if (lowestF->row == ending->row && lowestF->column == ending->column) // Found ending node!
			{
				// Need to add code here to backtrack to get the full path.
			}

			// Now get neighbours of the lowestF node.
			//[ ][ ][ ] r-1 r r+1 c-1
			//[ ] x [ ] r-1   r+1 c
			//[ ][ ][ ] r-1 r r+1 c+1
			
		}
	}

	printf("Warning! There is no valid path between the Starting and Ending nodes. [LinkedList* GetPath]\n");
	return NULL;
}
