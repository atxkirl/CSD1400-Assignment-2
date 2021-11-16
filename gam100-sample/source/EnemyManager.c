#include "EnemyManager.h"

/// <summary>
/// Allocates memory for an Enemy and returns a pointer to it.
/// </summary>
/// <param name="enemyName -">Name of this enemy instance.</param>
/// <param name="startingStateName -">Name of this enemy's starting state.</param>
/// <param name="position -">Spawn position of this enemy.</param>
/// <param name="map -">Pointer to the overall grid that this enemy will be spawned on. Used for pathfinding.</param>
/// <returns>Pointer to a newly allocated Enemy instance.</returns>
Enemy* EM_CreateEnemy(char* enemyName, char* startingStateName, CP_Vector position, AStar_Map* map, GameObject* target)
{
	Enemy* enemy = malloc(sizeof(Enemy));
	if (enemy)
	{
		enemy->go = GOM_Create(CIRCLE);
		enemy->go->isEnabled = true;
		enemy->go->tag = enemyName;
		enemy->currentMap = map;
		
		enemy->go->position = position;
		enemy->go->scale.x = 50.f; // TEST
		enemy->go->scale.y = 50.f; // TEST

		enemy->renderer = RM_AddComponent(enemy->go);
		RM_LoadImage(enemy->renderer, "Assets/bananaboi.png"); //TEST

		enemy->stateMachine = AIM_CreateFSM(startingStateName, enemy->go, target);
		
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
	pathingUpdateTime = 1.f / pathingUpdateFreq;
	pathingTimeElapsed = 0.f;

	enemyList = NULL;
}

void EM_Update()
{
	pathingTimeElapsed += CP_System_GetDt();

	// Update all Enemy FSM and Movement.
	EM_Update_FSMAndMovement();

	// Update all Enemy pathfinding.
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
		FSM* fsm = currentEnemy->stateMachine;
		if (fsm)
		{
			moveSpeed = fsm->currentStateSpeed;
			fsm->onUpdate(fsm, fsm->target);

			// If the current and next states are different, then change states.
			if (strcmp(fsm->currentState, fsm->nextState) != 0)
			{
				printf("Changing State to: %s\n", fsm->nextState);
				AIM_ChangeStates(fsm->nextState, fsm, fsm->target, fsm->targetPosition);
			}
		}

		// Update Movement
		if (currentEnemy->movementPath)
		{
			AStar_Node* currentNode = (AStar_Node*)currentEnemy->movementPath->curr;
			if (currentNode)
			{
				deltaX = (float)fabs((double)currentEnemy->go->position.x - (double)currentNode->position.x);
				deltaY = (float)fabs((double)currentEnemy->go->position.y - (double)currentNode->position.y);

				// Close enough to count as "reaching" the node.
				if (deltaX < distanceForReached && deltaY < distanceForReached)
				{
					if (currentEnemy->movementPathIndex > 0)
					{
						currentEnemy->movementPath = currentEnemy->movementPath->prev;
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

					// Move enemy in direction of next node.
					currentEnemy->go->position = CP_Vector_Add(currentEnemy->go->position, CP_Vector_Scale(direction, moveSpeed * CP_System_GetDt()));
					// Set enemy to face in direction of movement.
					if (currentEnemy->stateMachine != NULL)
						currentEnemy->stateMachine->goFacingDir = CP_Vector_Set(direction.x, direction.y);
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
	int currEnemyPathSize = 0;

	// Loops through all Enemies, then recalculates their A* pathing for movement if enough time has passed.
	// Useful for making Enemies follow moving targets, such as the Player.
	for (int i = 0; i < LL_GetCount(enemyList); ++i)
	{
		currentEnemy = (Enemy*)LL_Get(enemyList, i);
		if (currentEnemy == NULL)
			continue;

		if	(currentEnemy->stateMachine != NULL)
		{
			CP_Vector target = currentEnemy->stateMachine->targetPosition;

			// Don't repath if target's row/col are the same as previous update.
			AStar_GetRowCol(target, currentEnemy->currentMap, &currentEnemy->targetRow, &currentEnemy->targetCol);
			if (currentEnemy->targetRow == currentEnemy->targetPrevRow && currentEnemy->targetCol == currentEnemy->targetPrevCol)
				continue;

			// Calculate new path
			if (LL_GetCount(currentEnemy->movementPath) > 0) // Set starting position to the enemy's current target position, to remove rebounding behavior.
			{
				AStar_Node* currMovePos = (AStar_Node*)LL_Get(currentEnemy->movementPath, currentEnemy->movementPathIndex);
				AStar_GetPathWorldPosition(currMovePos->position, target, &currentEnemy->movementPath, currentEnemy->currentMap);
			}
			else // Set starting position to the enemy's position if they don't have a previous path.
			{
				AStar_GetPathWorldPosition(currentEnemy->go->position, target, &currentEnemy->movementPath, currentEnemy->currentMap);
			}
			
			// Adjust new startpoint, to prevent AI from moving back to where it started from, looks weird.
			currEnemyPathSize = LL_GetCount(currentEnemy->movementPath);
			currentEnemy->movementPathIndex = currEnemyPathSize - 1;

			// Save row/col position
			AStar_GetRowCol(target, currentEnemy->currentMap, &currentEnemy->targetPrevRow, &currentEnemy->targetPrevCol);
		}
	}
}
