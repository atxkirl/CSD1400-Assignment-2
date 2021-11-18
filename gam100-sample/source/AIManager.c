#include "AIManager.h"

void AIM_Init(void)
{
	allEnemies = NULL;
	allStates = NULL;

	AIM_InitStates();
}

void AIM_Clear(void)
{
	LL_Clear(&allEnemies);
	LL_Clear(&allStates);

	allEnemies = NULL;
	allStates = NULL;
}

void AIM_Update(void)
{
	// Update Enemy FSM and Movement
	FSM* enemy;
	CP_Vector distance, direction;

	for (int i = 0; i < LL_GetCount(allEnemies); ++i)
	{
		enemy = (FSM*)LL_Get(allEnemies, i);
		if (enemy == NULL)
			continue;

		// Update State
		enemy->onUpdate(enemy, NULL);
		if (strcmp(enemy->currentState, enemy->nextState) != 0)
		{
			printf("Changing State to: %s\n", enemy->nextState);
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
				enemy->movementPath = enemy->movementPath->next;
				printf("Reached a node.\n");
			}
			else
			{
				direction = CP_Vector_Subtract(currentNode->position, enemy->controlledObject->position);
				direction = CP_Vector_Normalize(direction);

				enemy->controlledObject->position = CP_Vector_Add(enemy->controlledObject->position, CP_Vector_Scale(direction, enemy->moveSpeed * CP_System_GetDt()));
				printf("Moving AI! Speed = %.2f\n", enemy->moveSpeed);
			}
		}


#if _DEBUG
		char str[100] = { 0 };
		if (enemy)
		{

			int di = 0;
			if (enemy->movementPath)
			{
				LinkedList* n = enemy->movementPath;
				CP_Color c = CP_Color_Create(0, 0, 255, 255);
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
						
						CP_Color tc = CP_Color_Create(c.r, c.g + di * 20, (int)c.b - di * 20, c.a);
						RM_DebugDrawLine(an->position, ann->position, PRI_GAME_OBJECT, tc);
					}

				}
			}


			sprintf_s(str, 100, "%s\n%2d", enemy->currentState, LL_GetCount(enemy->movementPath));
			RM_SetText(RM_GetComponent(enemy->controlledObject), str);
		}


#endif
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
	BBEM_Search.onEnter = FSMState_BBEM_Roam_OnEnter;
	BBEM_Search.onExit = FSMState_BBEM_Roam_OnExit;
	BBEM_Search.onUpdate = FSMState_BBEM_Roam_OnUpdate;
	LL_Add(&allStates, &BBEM_Search);



	// Mini Monke States

	MM_Idle.name = "MM_Idle";
	MM_Idle.onEnter = FSMState_MM_Idle_OnEnter;
	MM_Idle.onExit = FSMState_MM_Idle_OnExit;
	MM_Idle.onUpdate = FSMState_MM_Idle_OnUpdate;
	LL_Add(&allStates, &MM_Idle);

	MM_Roam.name = "MM_Roam";
	MM_Roam.onEnter = FSMState_MM_Roam_OnEnter;
	MM_Roam.onExit = FSMState_MM_Roam_OnExit;
	MM_Roam.onUpdate = FSMState_MM_Roam_OnUpdate;
	LL_Add(&allStates, &MM_Roam);

	MM_Chase.name = "MM_Chase";
	MM_Chase.onEnter = FSMState_MM_Roam_OnEnter;
	MM_Chase.onExit = FSMState_MM_Roam_OnExit;
	MM_Chase.onUpdate = FSMState_MM_Roam_OnUpdate;
	LL_Add(&allStates, &MM_Chase);
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
	if (targetObject != NULL)
		controller->targetPosition = &targetObject->position;
	else
		controller->targetPosition = NULL;
	controller->tileSize = GetTileScale();

	// Initalize State variables.
	controller->currentState = startStateName;
	controller->nextState = startStateName;
	controller->onEnter = startState->onEnter;
	controller->onExit = startState->onExit;
	controller->onUpdate = startState->onUpdate;

	// Invoke fsm's current state OnEnter function.
	controller->onEnter(controller, controller->targetPosition);
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
	controller->onExit(controller, NULL);

	// Update function ptrs
	controller->onEnter = state->onEnter;
	controller->onUpdate = state->onUpdate;
	controller->onExit = state->onExit;

	// Update state names
	controller->currentState = state->name;
	controller->nextState = state->name;

	// Invoke the onEnter function for the new state.
	controller->onEnter(controller, NULL);
}

State* AIM_FindState(char* stateName)
{
	for (int i = 0; i < LL_GetCount(allStates); ++i)
	{
		State* state = (State*)LL_Get(allStates, i);
		if (state && strcmp(state->name, stateName) == 0)
		{
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
	RM_LoadImage(enemy->controlledRenderer, "Assets/bananaboi.png");
	// Collider
	enemy->controlledCollider = CLM_AddComponent(enemy->controlledObject);
	CLM_Set(enemy->controlledCollider, COL_BOX, NULL);			// TODO: Add function in Enemy to handle collisions with Player to deal damage or something.

	// Pathfinding
	enemy->map = levelMap;
	enemy->movementPath = NULL;
	enemy->moveSpeed = 0.f;
	
	// Initialize StateMachine
	AIM_InitFSM(enemy, startStateName, targetObj);

	LL_Add(&allEnemies, enemy);
	return enemy;
}
