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

	movementSpeed = 0.f;
	deltaX = deltaY = 0.f;
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

	//for (int i = 0; i < LL_GetCount(enemyList); ++i)
	//{
	//	Enemy* temp = (Enemy*)enemyList->curr;
	//	if (temp == NULL)
	//		continue;


	//}
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
	// Loops through all Enemies and updates the position, based on their current movement path and speed.
	for (int i = 0; i < LL_GetCount(enemyList); ++i)
	{
		Enemy* temp = (Enemy*)enemyList->curr;
		if (temp == NULL)
			continue;

		// Update FSM
		if (temp->stateMachine != NULL)
		{
			movementSpeed = temp->stateMachine->currentStateSpeed;
			temp->stateMachine->onUpdate(temp->go, temp->stateMachine->target);
		}

		// Update Movement
		if (temp->movementPath != NULL)
		{
			AStar_Node* currentNode = (AStar_Node*)LL_Get(temp->movementPath, temp->movementPathIndex);
			if (currentNode != NULL)
			{
				deltaX = (float)fabs((double)temp->go->position.x - (double)currentNode->position.x);
				deltaY = (float)fabs((double)temp->go->position.y - (double)currentNode->position.y);

				// Close enough to count as "reaching" the node.
				if (deltaX < distanceForReached && deltaY < distanceForReached)
				{
					if (temp->movementPathIndex > 0)
					{
						printf("Reached a node!\n");
						--temp->movementPathIndex;
					}
					else
					{
						printf("Reached the end!\n");
						LL_Clear(&temp->movementPath);
					}
				}
				// Move to position of the current head of the linked list.
				else
				{
					CP_Vector direction = CP_Vector_Subtract(currentNode->position, temp->go->position);
					direction = CP_Vector_Normalize(direction);

					temp->go->position = CP_Vector_Add(temp->go->position, CP_Vector_Scale(direction, movementSpeed * CP_System_GetDt()));

					printf("Moving! Pos [%6.2f, %6.2f]\n", temp->go->position.x, temp->go->position.y);
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
	// Loops through all Enemies, then recalculates their A* pathing for movement if enough time has passed.
	// Useful for making Enemies follow moving targets, such as the Player.
	for (int i = 0; i < LL_GetCount(enemyList); ++i)
	{
		Enemy* temp = (Enemy*)enemyList->curr;
		if (temp == NULL)
			continue;

		if (temp->stateMachine != NULL && temp->stateMachine->target != NULL)
		{
			GameObject* enemyGo = temp->go;
			GameObject* targetGo = temp->stateMachine->target;

			// Don't repath if positions are the same.
			AStar_GetRowCol(targetGo->position, temp->currentMap, &temp->targetRow, &temp->targetCol);
			if (temp->targetRow == temp->targetPrevRow && temp->targetCol == temp->targetPrevCol)
				continue;

			// Calculate new path
			AStar_GetPathWorldPosition(enemyGo->position, targetGo->position, &temp->movementPath, temp->currentMap);
			temp->movementPathIndex = LL_GetCount(temp->movementPath) - 1;
			// Save row/col position
			AStar_GetRowCol(targetGo->position, temp->currentMap, &temp->targetPrevRow, &temp->targetPrevCol);
		}
	}
}
