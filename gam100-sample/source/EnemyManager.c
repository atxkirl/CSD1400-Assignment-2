#include "EnemyManager.h"

Enemy* EM_CreateEnemy(char* enemyName, char* startingStateName, CP_Vector position, AStar_Map* map)
{
	Enemy* enemy = malloc(sizeof(Enemy));
	if (enemy)
	{
		enemy->go = GOM_Create(CIRCLE);
		enemy->go->isEnabled = true;
		enemy->go->tag = enemyName;
		enemy->currentMap = map;
		
		enemy->go->position = position;
		enemy->go->scale.x = 20.f; // TEST
		enemy->go->scale.y = 20.f; // TEST

		enemy->renderer = RM_AddComponent(enemy->go);

		enemy->stateMachine = FSM_CreateFSM(startingStateName, enemy->go);
		
		enemy->movementPath = NULL;
		enemy->movementPathIndex = 0;

		enemy->targetRow = 0;
		enemy->targetCol = 0;
		enemy->targetPrevRow = 0;
		enemy->targetPrevCol = 0;

		LL_Add(&enemyList, enemy);
	}
	return enemy;
}

void EM_Init()
{
	movementUpdateTime = 1.f / movementUpdateFreq;
	movementTimeElapsed = 0.f;

	pathingUpdateTime = 1.f / pathingUpdateFreq;
	pathingTimeElapsed = 0.f;

	enemyList = NULL;
}

void EM_Update()
{
	movementTimeElapsed += CP_System_GetDt();
	pathingTimeElapsed += CP_System_GetDt();

	if (movementTimeElapsed > movementUpdateTime)
	{
		EM_Update_FSMAndMovement();
		movementTimeElapsed -= movementTimeElapsed;
	}
	if (pathingTimeElapsed > pathingUpdateTime)
	{
		EM_Update_Pathing();
		pathingTimeElapsed -= pathingTimeElapsed;
	}
}

void EM_Clear()
{
	LL_Clear(&enemyList);
	enemyList = NULL;
}

/// <summary>
/// Update loop for enemy movement.
/// </summary>
void EM_Update_FSMAndMovement()
{
	Enemy* currentEnemy = NULL;
	float deltaX = 0.f;
	float deltaY = 0.f;
	float moveSpeed = 0.f;

	// Loops through all Enemies and updates the position, based on their current movement path and speed.
	for (int i = 0; i < LL_GetCount(enemyList); ++i)
	{
		currentEnemy = (Enemy*)LL_Get(enemyList, i);
		if (currentEnemy == NULL)
			continue;

		// Update FSM
		if (currentEnemy->stateMachine != NULL)
		{
			moveSpeed = currentEnemy->stateMachine->currentStateSpeed;
			currentEnemy->stateMachine->onUpdate(currentEnemy->go, currentEnemy->stateMachine->target);
		}

		// Update Movement
		if (currentEnemy->movementPath != NULL)
		{
			AStar_Node* currentNode = (AStar_Node*)LL_Get(currentEnemy->movementPath, currentEnemy->movementPathIndex);
			if (currentNode != NULL)
			{
				deltaX = (float)fabs((double)currentEnemy->go->position.x - (double)currentNode->position.x);
				deltaY = (float)fabs((double)currentEnemy->go->position.y - (double)currentNode->position.y);

				// Close enough to count as "reaching" the node.
				if (deltaX < distanceForReached && deltaY < distanceForReached)
				{
					if (currentEnemy->movementPathIndex > 0)
					{
						--currentEnemy->movementPathIndex;
					}
					else
					{
						LL_Clear(&currentEnemy->movementPath);
					}
				}
				// Move to position of the current head of the linked list.
				else
				{
					CP_Vector direction = CP_Vector_Subtract(currentNode->position, currentEnemy->go->position);
					direction = CP_Vector_Normalize(direction);

					currentEnemy->go->position = CP_Vector_Add(currentEnemy->go->position, CP_Vector_Scale(direction, moveSpeed * CP_System_GetDt()));
				}
			}
		}
	}
}

/// <summary>
/// Update loop for enemy A* pathing.
/// </summary>
void EM_Update_Pathing()
{
	Enemy* currentEnemy = NULL;
	GameObject* targetGo = NULL;
	int currEnemyPathSize = 0;

	// Loops through all Enemies, then recalculates their A* pathing for movement if enough time has passed.
	// Useful for making Enemies follow moving targets, such as the Player.
	for (int i = 0; i < LL_GetCount(enemyList); ++i)
	{
		currentEnemy = (Enemy*)LL_Get(enemyList, i);
		if (currentEnemy == NULL)
			continue;

		if (currentEnemy->stateMachine != NULL && currentEnemy->stateMachine->target != NULL)
		{
			targetGo = currentEnemy->stateMachine->target;

			// Don't repath if target's row/col are the same as previous update.
			AStar_GetRowCol(targetGo->position, currentEnemy->currentMap, &currentEnemy->targetRow, &currentEnemy->targetCol);
			if (currentEnemy->targetRow == currentEnemy->targetPrevRow && currentEnemy->targetCol == currentEnemy->targetPrevCol)
				continue;

			// Calculate new path
			if (LL_GetCount(currentEnemy->movementPath) > 0) // Set starting position to the enemy's current target position, to remove rebounding behavior.
			{
				AStar_Node* currMovePos = (AStar_Node*)LL_Get(currentEnemy->movementPath, currentEnemy->movementPathIndex);
				AStar_GetPathWorldPosition(currMovePos->position, targetGo->position, &currentEnemy->movementPath, currentEnemy->currentMap);
			}
			else // Set starting position to the enemy's position if they don't have a previous path.
			{
				AStar_GetPathWorldPosition(currentEnemy->go->position, targetGo->position, &currentEnemy->movementPath, currentEnemy->currentMap);
			}
			
			// Adjust new startpoint, to prevent AI from moving back to where it started from, looks weird.
			currEnemyPathSize = LL_GetCount(currentEnemy->movementPath);
			currentEnemy->movementPathIndex = currEnemyPathSize - 1;

			// Save row/col position
			AStar_GetRowCol(targetGo->position, currentEnemy->currentMap, &currentEnemy->targetPrevRow, &currentEnemy->targetPrevCol);
		}
	}
}
