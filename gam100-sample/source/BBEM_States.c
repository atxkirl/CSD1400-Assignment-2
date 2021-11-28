/*
* @file		BBEM_States.c
* @author	Adrian Tan (t.xingkhiangadrian)
* @course	CSD1400 Software Engineering Project 1
* @Team		BananaBoi
* @date		10/11/2021
* @brief	Contains function definitions for the statemachine functions to
*			implement the Big Bad Evil Monkey behaviors.
*//*----------------------------------------------------------------------*/

#include "BBEM_States.h"
#include "Helpers.h"
#include "Player.h"

//-----------//
// BBEM Idle //
//-----------//
//
// - During IDLE, the AI will wait for 3-5 seconds, just chill in one place.
// - Once the wait time is over, the AI will change to ROAM state.
// - HOWEVER, if Player walks within a certain radius, the AI will change to CHASE state.

static const float idleSpeed = 0.f;

/// <summary>
/// OnEnter function for Idle state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Idle_OnEnter(FSM* controller)
{
	printf("BBEM_Idle_OnEnter()\n");

	// Set main variables.
	controller->waitTime = FRAND(3, 5);
	controller->elapsedTime = 0.f;
	controller->moveSpeed = idleSpeed;
}
/// <summary>
/// OnExit function for Idle state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Idle_OnExit(FSM* controller)
{
	controller->waitTime = 0.f;
	controller->elapsedTime = 0.f;
}
/// <summary>
/// OnUpdate function for Idle state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Idle_OnUpdate(FSM* controller)
{
	// Is Player too near me?
	CP_Vector directionToPlayer = CP_Vector_Subtract(controller->targetObject->position, controller->controlledObject->position);
	CP_Vector enemyForward = CP_Vector_Subtract(controller->fovDetectionForward, controller->controlledObject->position);
	float distance = CP_Vector_Length(directionToPlayer);
	float angle = CP_Vector_Angle(enemyForward, directionToPlayer);
	if (distance <= (controller->immediateDetectionRadius * controller->tileSize) ||
	   (distance <= (controller->fovDetectionRadius * controller->tileSize) && angle < controller->fovDetectionHalfAngle))
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

/// <summary>
/// OnEnter function for Roam state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Roam_OnEnter(FSM* controller)
{
	printf("BBEM_Roam_OnEnter()\n");

	// Set main variables.
	controller->moveSpeed = roamSpeed;
	controller->elapsedTime = 0.f;
	controller->waitTime = 0.f;
	
	// Set the AI to target the Player
	*controller->targetPosition = controller->targetObject->position;

	// Get random position near the Player.
	AStar_GetTile(controller->targetPosition , *controller->targetPosition, controller->map, roamRadiusMin, roamRadiusMax);
	// Calculate path to target position.
	AStar_GetPathWorldPosition(controller->controlledObject->position, *controller->targetPosition, &controller->movementPath, controller->map);
}
/// <summary>
/// OnExit function for Roam state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Roam_OnExit(FSM* controller)
{
}
/// <summary>
/// OnUpdate function for Roam state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Roam_OnUpdate(FSM* controller)
{
	// Is Player too near me?
	CP_Vector directionToPlayer = CP_Vector_Subtract(controller->targetObject->position, controller->controlledObject->position);
	CP_Vector enemyForward = CP_Vector_Subtract(controller->fovDetectionForward, controller->controlledObject->position);
	float distance = CP_Vector_Length(directionToPlayer);
	float angle = CP_Vector_Angle(enemyForward, directionToPlayer);
	if (distance <= (controller->immediateDetectionRadius * controller->tileSize) ||
	   (distance <= (controller->fovDetectionRadius * controller->tileSize) && angle < controller->fovDetectionHalfAngle))
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

static const float chaseSpeed = 200.f;
static const int chaseLoseRadius = 6;
static const float chaseRepathDist = 0.5f;
static const float chaseDamageDist = 1.2f;

/// <summary>
/// OnEnter function for Chase state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Chase_OnEnter(FSM* controller)
{
	printf("BBEM_Chase_OnEnter()\n");

	// Set main variables.
	controller->moveSpeed = chaseSpeed;
	controller->elapsedTime = 0.f;
	controller->waitTime = 0.f;

	// Set AI to chase player.
	*controller->targetPosition = controller->targetObject->position;
	AStar_GetPathWorldPosition(controller->controlledObject->position, *controller->targetPosition, &controller->movementPath, controller->map);
	// Save the targetPosition.
	controller->targetPrevPosition = *controller->targetPosition;
}
/// <summary>
/// OnExit function for Chase state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Chase_OnExit(FSM* controller)
{
	// Keep track of Player position.
	*controller->targetPosition = controller->targetObject->position;
}
/// <summary>
/// OnUpdate function for Chase state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Chase_OnUpdate(FSM* controller)
{
	// Keep track of Player position.
	*controller->targetPosition = controller->targetObject->position;
	// If AI needs to wait, just wait.
	controller->elapsedTime += CP_System_GetDt();
	if (controller->elapsedTime < controller->waitTime)
	{
		return;
	}

	// If Player is invincible, means we've just hit him, so walk slower.
	if (PLY_IsInvincible())
	{
		controller->moveSpeed = 0.8f * chaseSpeed;
	}
	else
	{
		controller->moveSpeed = chaseSpeed;
	}

	float distance = CP_Vector_Length(CP_Vector_Subtract(controller->controlledObject->position, controller->targetObject->position));
	// Is Player within smacking range?
	if (distance <= (chaseDamageDist * controller->tileSize))
	{
		// Deal damage to the player.
		bool pass = PLY_TakeDamage();
		if (pass)
		{
			// Make AI wait for abit before continuing to chase or change state.
			controller->elapsedTime = 0.f;
			controller->waitTime = FRAND(1, 2);
			return;
		}
	}
	// Is Player too far from AI?
	if (distance > (chaseLoseRadius * controller->tileSize) || PLY_IsHidden())
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

static const float searchSpeed = 50.f;
static const int searchMin = 1;
static const int searchMax = 3;
static const int searchRadiusMin = 0;		// Minimum radius around player.
static const int searchRadiusMax = 2;

/// <summary>
/// OnEnter function for Search state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Search_OnEnter(FSM* controller)
{
	printf("BBEM_Search_OnEnter()\n");

	// Set main variables.
	controller->moveSpeed = searchSpeed;
	controller->elapsedTime = 0.f;
	controller->waitTime = 0.f;

	// Get how many spots the AI will "search" through.
	controller->searchCount = RAND(searchMin, searchMax);

	// Pick the first location to search.
	AStar_GetTile(controller->targetPosition, *controller->targetPosition, controller->map, searchRadiusMin, searchRadiusMax);
	// Calculate path to search position.
	AStar_GetPathWorldPosition(controller->controlledObject->position, *controller->targetPosition, &controller->movementPath, controller->map);
	--controller->searchCount;
}
/// <summary>
/// OnExit function for Search state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Search_OnExit(FSM* controller)
{

}
/// <summary>
/// OnUpdate function for Search state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Search_OnUpdate(FSM* controller)
{
	// Is Player too near me and visible?
	CP_Vector directionToPlayer = CP_Vector_Subtract(controller->targetObject->position, controller->controlledObject->position);
	CP_Vector enemyForward = CP_Vector_Subtract(controller->fovDetectionForward, controller->controlledObject->position);
	float distance = CP_Vector_Length(directionToPlayer);
	float angle = CP_Vector_Angle(enemyForward, directionToPlayer);
	if (distance <= (controller->immediateDetectionRadius * controller->tileSize))
	{
		controller->nextState = "BBEM_Chase";
		return;
	}
	else if (!PLY_IsHidden() && distance <= (controller->fovDetectionRadius * controller->tileSize) && angle < controller->fovDetectionHalfAngle)
	{
		controller->nextState = "BBEM_Chase";
		return;
	}

	// Get the next search position, if search count is greater than 0.
	if (controller->movementPath == NULL)
	{
		if (controller->searchCount > 0)
		{
			printf("Hmm... Searching... Count = %d\n", controller->searchCount);

			// Get the search location.
			AStar_GetTile(controller->targetPosition, *controller->targetPosition, controller->map, searchRadiusMin, searchRadiusMax);
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
