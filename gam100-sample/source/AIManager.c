/*
* @file		AIManager.c
* @author	Adrian Tan (t.xingkhiangadrian)
* @course	CSD1400 Software Engineering Project 1
* @Team		BananaBoi
* @date		06/11/2021
* @brief	Contains functions to create, modify, and control the behaviors,
*			and movements of Enemy AI.
*//*----------------------------------------------------------------------*/

#include "AIManager.h"
#include "Objectives.h"

void AIM_Init(void)
{
	allEnemies = NULL;
	allStates = NULL;

	AIM_InitStates();
}

void AIM_Clear(void)
{
	// Free any allocated memory for each enemy.
	for (int i = 0; i < LL_GetCount(allEnemies); ++i)
	{
		FSM* enemy = (FSM*)LL_Get(allEnemies, i);
		if (enemy)
		{
			free(enemy->targetPosition);
			LL_Clear(&enemy->movementPath);
			free(enemy);
		}
	}

	// Clear all lists!
	LL_Clear(&allEnemies);
	LL_Clear(&allStates);

	allEnemies = NULL;
	allStates = NULL;
}

void AIM_Update()
{
	// Update Enemy FSM and Movement
	FSM* enemy;
	CP_Vector distance;

	for (int i = 0; i < LL_GetCount(allEnemies); ++i)
	{
		enemy = (FSM*)LL_Get(allEnemies, i);
		if (enemy == NULL)
			continue;

		// Update State
		enemy->onUpdate(enemy);
		if (strcmp(enemy->currentState, enemy->nextState) != 0)
		{
			AIM_ChangeStates(enemy->nextState, enemy);
		}

		// Update Movement
		if (enemy->movementPath)
		{
			AStar_Node* currentNode = (AStar_Node*)enemy->movementPath->curr;
			if (currentNode == NULL)
			{
				continue;
			}

			distance = CP_Vector_Subtract(enemy->controlledObject->position, currentNode->position);
			if (CP_Vector_Length(distance) <= (distanceTillReached * enemy->tileSize))
			{
				//enemy->movementPath = enemy->movementPath->next;
				LL_RemoveIndex(&enemy->movementPath, 0);
			}
			else
			{
				float actualSpeed = enemy->moveSpeed;
				if (!Objectives_GetPlayerUpdate())
				{
					actualSpeed = enemy->moveSpeed * 0.25f;
				}
				enemy->controlledObjForward = CP_Vector_Normalize(CP_Vector_Subtract(currentNode->position, enemy->controlledObject->position));
				enemy->controlledObject->position = CP_Vector_Add(enemy->controlledObject->position, CP_Vector_Scale(enemy->controlledObjForward, actualSpeed * CP_System_GetDt()));

				enemy->fovDetectionForward = CP_Vector_Add(enemy->controlledObject->position, CP_Vector_Scale(enemy->controlledObjForward, enemy->fovDetectionRadius * enemy->tileSize));
			}
		}

		// Debug Lines showing AI's Path
		{
			if (enemy)
			{
				CP_Color c;
				GameObject* go = enemy->controlledObject;

				// Show AI state and movement path.
				int di = 0;
				if (enemy->movementPath)
				{
					LinkedList* n = enemy->movementPath;
					c = CP_Color_Create(255, 0, 0, 255);
					for (; n; n = n->next, di++)
					{
						AStar_Node* an = (AStar_Node*)n->curr;
						AStar_Node* ann = NULL;
						if (n->next)
							ann = (AStar_Node*)n->next->curr;

						if (an && ann)
						{
							if (di > 12)
								di = 0;

							CP_Color tc = CP_Color_Create(c.r - di * 40, c.g + di * 40, c.b, c.a);
							RM_DebugDrawLine(an->position, ann->position, PRI_GAME_OBJECT, tc);
						}

					}
				}

#ifdef _DEBUG
				char str[100] = { 0 };
				sprintf_s(str, 100, "%s\n%2d", enemy->currentState, LL_GetCount(enemy->movementPath));
				RM_SetText(RM_GetComponent(go), str);
#endif

				// Show AI detection cone.
				c = CP_Color_Create(255, 0, 0, 255);
				CP_Matrix rotation;
				CP_Vector rotateForward;

				// Draw fov extents
				c = CP_Color_Create(0, 255, 0, 255);
				rotation = CP_Matrix_Rotate(enemy->fovDetectionHalfAngle);
				rotateForward = CP_Vector_MatrixMultiply(rotation, enemy->controlledObjForward);
				rotateForward = CP_Vector_Normalize(rotateForward);
				RM_DebugDrawLine(go->position, CP_Vector_Add(go->position, CP_Vector_Scale(rotateForward, enemy->fovDetectionRadius * enemy->tileSize)), PRI_GAME_OBJECT, c);
				rotation = CP_Matrix_Rotate(-enemy->fovDetectionHalfAngle);
				rotateForward = CP_Vector_MatrixMultiply(rotation, enemy->controlledObjForward);
				rotateForward = CP_Vector_Normalize(rotateForward);
				RM_DebugDrawLine(go->position, CP_Vector_Add(go->position, CP_Vector_Scale(rotateForward, enemy->fovDetectionRadius * enemy->tileSize)), PRI_GAME_OBJECT, c);
			}
		}
	}
}

void AIM_InitStates(void)
{
	// Big Bad Evil Monkey States

	BBEM_Idle.name = "BBEM_Idle";
	BBEM_Idle.onEnter = FSMState_BBEM_Idle_OnEnter;
	BBEM_Idle.onExit = FSMState_BBEM_Idle_OnExit;
	BBEM_Idle.onUpdate = FSMState_BBEM_Idle_OnUpdate;
	LL_Add(&allStates, &BBEM_Idle);

	BBEM_Roam.name = "BBEM_Roam";
	BBEM_Roam.onEnter = FSMState_BBEM_Roam_OnEnter;
	BBEM_Roam.onExit = FSMState_BBEM_Roam_OnExit;
	BBEM_Roam.onUpdate = FSMState_BBEM_Roam_OnUpdate;
	LL_Add(&allStates, &BBEM_Roam);

	BBEM_Chase.name = "BBEM_Chase";
	BBEM_Chase.onEnter = FSMState_BBEM_Chase_OnEnter;
	BBEM_Chase.onExit = FSMState_BBEM_Chase_OnExit;
	BBEM_Chase.onUpdate = FSMState_BBEM_Chase_OnUpdate;
	LL_Add(&allStates, &BBEM_Chase);

	BBEM_Search.name = "BBEM_Search";
	BBEM_Search.onEnter = FSMState_BBEM_Search_OnEnter;
	BBEM_Search.onExit = FSMState_BBEM_Search_OnExit;
	BBEM_Search.onUpdate = FSMState_BBEM_Search_OnUpdate;
	LL_Add(&allStates, &BBEM_Search);
}

void AIM_InitFSM(FSM* controller, char* startStateName, GameObject* targetObject)
{
	State* startState = AIM_FindState(startStateName);
	if (startState == NULL)
	{
		printf("Unable to create FSM for '%s'! Unable to find state '%s'!\n", controller->controlledObject->tag, startStateName);
		return;
	}

	// Initialize Target variables.
	controller->targetObject = targetObject;
	controller->tileSize = GetTileScale();
	controller->searchCount = 0;
	controller->targetPosition = (CP_Vector*)malloc(sizeof(CP_Vector));
	controller->controlledObjForward = CP_Vector_Set(0.f, -1.f);
	if (controller->targetPosition && targetObject != NULL)
	{
		*controller->targetPosition = targetObject->position;
	}

	// Initialize Detection Variables.
	controller->immediateDetectionRadius = 1;
	controller->fovDetectionHalfAngle = 20.f;
	controller->fovDetectionRadius = 3;

	// Initalize State variables.
	controller->currentState = startStateName;
	controller->nextState = startStateName;
	controller->onEnter = startState->onEnter;
	controller->onExit = startState->onExit;
	controller->onUpdate = startState->onUpdate;

	// Invoke fsm's current state OnEnter function.
	controller->onEnter(controller);
}

void AIM_ChangeStates(char* stateName, FSM* controller)
{
	State* state = AIM_FindState(stateName);
	if (state == NULL)
	{
		printf("Unable to change state to '%s'!\n", stateName);
		return;
	}

	// Invoke the onExit function for the current state before changing.
	controller->onExit(controller);

	// Update function ptrs
	controller->onEnter = state->onEnter;
	controller->onUpdate = state->onUpdate;
	controller->onExit = state->onExit;

	// Update state names
	controller->currentState = state->name;
	controller->nextState = state->name;

	// Invoke the onEnter function for the new state.
	controller->onEnter(controller);
}

State* AIM_FindState(char* stateName)
{
	for (int i = 0; i < LL_GetCount(allStates); ++i)
	{
		State* state = (State*)LL_Get(allStates, i);
		if (state && strcmp(state->name, stateName) == 0)
		{
			printf("Found state %s!\n", state->name);
			return state;
		}
	}

	printf("Unable to find state '%s'\n", stateName);
	return NULL;
}

FSM* AIM_CreateEnemy(char* enemyName, char* startStateName, CP_Vector enemyPos, GameObject* targetObj, AStar_Map* levelMap)
{
	// Create Enemy.
	FSM* enemy = (FSM*)malloc(sizeof(FSM));
	if (enemy == NULL)
	{
		printf("Unable to create Enemy for '%s'! Failed to malloc FSM*.\n", enemyName);
		return NULL;
	}

	// GameObject
	enemy->controlledObject = GOM_Create2(CIRCLE, enemyPos, 0.f, CP_Vector_Set(50.f, 50.f));
	enemy->controlledObject->tag = enemyName;
	// Renderer
	enemy->controlledRenderer = RM_AddComponent(enemy->controlledObject);
	RM_LoadImage(enemy->controlledRenderer, "Assets/monke/bigmonke-front.png");
	// Collider
	enemy->controlledCollider = CLM_AddComponent(enemy->controlledObject);
	CLM_Set(enemy->controlledCollider, COL_BOX, NULL);
	enemy->controlledCollider->isTrigger = 1;

	// Pathfinding
	enemy->map = levelMap;
	enemy->movementPath = NULL;
	enemy->moveSpeed = 0.f;
	
	// Initialize StateMachine
	AIM_InitFSM(enemy, startStateName, targetObj);

	LL_Add(&allEnemies, enemy);
	return enemy;
}

// TODO:
// - Add detection cone with variable detection angle (FOV)
// - Add "hideable" tiles to Search state.
// - Add "emotion" GameObject and Renderers above AI's head. (To display stuff like "??" when AI is searching for Player, and "!!" when AI finds player.