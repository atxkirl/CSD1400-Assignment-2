#include "BBEM_States.h"
#include "Helpers.h"

//-------------//
// BBEM Global //
//-------------//
//
// - Variables that ALL states will use!

static const int immediateDetectionRadius = 2; // Radius of 2 tiles around the AI.

//-----------//
// BBEM Idle //
//-----------//
//
// - During IDLE, the AI will wait for 3-5 seconds, just chill in one place.
// - Once the wait time is over, the AI will change to ROAM state.
// - HOWEVER, if Player walks within a certain radius, the AI will change to CHASE state.

static const float idleSpeed = 0.f;

void FSMState_BBEM_Idle_OnEnter(FSM* controller, CP_Vector* newTargetPosition)
{
	printf("BBEM_Idle_OnEnter()\n");

	// Set main variables.
	controller->waitTime = FRAND(3, 5);
	controller->elapsedTime = 0.f;
	controller->moveSpeed = idleSpeed;
}

void FSMState_BBEM_Idle_OnExit(FSM* controller, CP_Vector* newTargetPosition)
{
	controller->waitTime = 0.f;
	controller->elapsedTime = 0.f;
}

void FSMState_BBEM_Idle_OnUpdate(FSM* controller, CP_Vector* newTargetPosition)
{
	// Is Player too near me?
	float distance = CP_Vector_Length(CP_Vector_Subtract(controller->controlledObject->position, controller->targetObject->position));
	if (distance <= (immediateDetectionRadius * controller->tileSize))
	{
		controller->nextState = "BBEM_Chase";
		return;
	}

	// Wait for time
	controller->elapsedTime += CP_System_GetDt();
	if (controller->elapsedTime >= controller->waitTime)
	{
		controller->nextState = "BBEM_Roam";
		return;
	}
}

//-----------//
// BBEM Roam //
//-----------//
//
// - During ROAM, the AI will pick a location and walk towards it, using A* path.
// - Upon reaching the location, the AI will change to IDLE state.
// - HOWEVER, if Player walks within a certain radius, the AI will change to CHASE state.

static const float roamSpeed = 100.f;
static const int roamRadiusMin = 1;		// Minimum radius around player.
static const int roamRadiusMax = 2;

void FSMState_BBEM_Roam_OnEnter(FSM* controller, CP_Vector* newTargetPosition)
{
	printf("BBEM_Roam_OnEnter()\n");

	// Set main variables.
	controller->moveSpeed = roamSpeed;
	
	// Set the AI to target the Player
	*controller->targetPosition = controller->targetObject->position;

	// Get random position near the Player.
	AStar_GetTile(controller->targetPosition , *controller->targetPosition, controller->map, roamRadiusMin, roamRadiusMax);
	// Calculate path to target position.
	AStar_GetPathWorldPosition(controller->controlledObject->position, *controller->targetPosition, &controller->movementPath, controller->map);
}

void FSMState_BBEM_Roam_OnExit(FSM* controller, CP_Vector* newTargetPosition)
{
}

void FSMState_BBEM_Roam_OnUpdate(FSM* controller, CP_Vector* newTargetPosition)
{
	// Is Player too near me?
	float distance = CP_Vector_Length(CP_Vector_Subtract(controller->controlledObject->position, controller->targetObject->position));
	if (distance <= (immediateDetectionRadius * controller->tileSize))
	{
		controller->nextState = "BBEM_Chase";
		return;
	}

	// Reached destination?
	if (controller->movementPath == NULL)
	{
		controller->nextState = "BBEM_Idle";
		return;
	}
}

//------------//
// BBEM Chase //
//------------//
//
// - During CHASE, the AI will move towards the Player's position.
// - Upon reaching the Player, the AI will change to IDLE state. (Note, on Chase Exit, deal damage to Player)
// - HOWEVER, if Player walks outside a certain radius, the AI will change to SEARCH state. (Note, in future add Line-of-Sight as well.)

static const float chaseSpeed = 100.f;
static const int chaseLoseRadius = 6;
static const float chaseRepathDist = 0.5f;

void FSMState_BBEM_Chase_OnEnter(FSM* controller, CP_Vector* newTargetPosition)
{
	printf("BBEM_Chase_OnEnter()\n");

	controller->moveSpeed = chaseSpeed;

	// Set AI to chase player.
	*controller->targetPosition = controller->targetObject->position;
	AStar_GetPathWorldPosition(controller->controlledObject->position, *controller->targetPosition, &controller->movementPath, controller->map);
	// Save the targetPosition.
	controller->targetPrevPosition = *controller->targetPosition;
}

void FSMState_BBEM_Chase_OnExit(FSM* controller, CP_Vector* newTargetPosition)
{
}

void FSMState_BBEM_Chase_OnUpdate(FSM* controller, CP_Vector* newTargetPosition)
{
	// Keep track of Player position.
	*controller->targetPosition = controller->targetObject->position;

	float distance = CP_Vector_Length(CP_Vector_Subtract(controller->controlledObject->position, controller->targetObject->position));
	// Is Player too far from AI?
	if (distance > (chaseLoseRadius * controller->tileSize))
	{
		printf("Where'd he go??\n");
		controller->nextState = "BBEM_Search";
		return;
	}

	// Calculate the distance (if any) that the AI's target has moved since previous frame.
	float distanceMoved = CP_Vector_Length(CP_Vector_Subtract(*controller->targetPosition, controller->targetPrevPosition));
	// If distance is more than half a tile, AI's pathing should be recalculated!
	if (distanceMoved >= (controller->tileSize * chaseRepathDist))
	{
		// If AI already has a path, let the starting point for this pathing be the AI's current path node's position.
		if (controller->movementPath != NULL)
		{
			AStar_Node* temp = (AStar_Node*)controller->movementPath->curr;
			if (temp)
			{
				AStar_GetPathWorldPosition(temp->position, *controller->targetPosition, &controller->movementPath, controller->map);
			}
		}
		// If no existing path, then just path from AI's current position.
		else
			AStar_GetPathWorldPosition(controller->controlledObject->position, *controller->targetPosition, &controller->movementPath, controller->map);
		
		// Save the targetPosition.
		controller->targetPrevPosition = *controller->targetPosition;
	}
}

//-------------//
// BBEM Search //
//-------------//
//
// - During Search, the AI will pick a location near the Player's last seen location and move towards it.
// - IF Player is not found, the AI will pick another location within 3 tiles and move towards it. (Note, found means Player is not hidden, and within radius of AI.)
// - IF Player if still not found, the AI will change to IDLE state. (Note, found means Player is not hidden, and within radius of AI.)
// - HOWEVER, if Player is found, the AI will change to CHASE state.

static const float searchSpeed = 100.f;
static const int searchMin = 1;
static const int searchMax = 3;

void FSMState_BBEM_Search_OnEnter(FSM* controller, CP_Vector* newTargetPosition)
{
	printf("BBEM_Search_OnEnter()\n");

	controller->moveSpeed = searchSpeed;

	// Get how many spots the AI will "search" through.
	controller->searchCount = RAND(searchMin, searchMax);
}

void FSMState_BBEM_Search_OnExit(FSM* controller, CP_Vector* newTargetPosition)
{
}

void FSMState_BBEM_Search_OnUpdate(FSM* controller, CP_Vector* newTargetPosition)
{
	// If reached position, then get the next search position.
	if (controller->movementPath == NULL)
	{
		if (controller->searchCount > 0)
		{
			printf("Hmm... Searching... Count = %d\n", controller->searchCount);

			// Get the search location.
			AStar_GetTile(controller->targetPosition, *controller->targetPosition, controller->map, roamRadiusMin, roamRadiusMax);
			// Calculate path to search position.
			AStar_GetPathWorldPosition(controller->controlledObject->position, *controller->targetPosition, &controller->movementPath, controller->map);

			--controller->searchCount;
		}
		else
		{
			controller->nextState = "BBEM_Idle";
		}
	}
}
