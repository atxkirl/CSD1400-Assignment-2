#include "BBEM_States.h"
#include "LinkedList.h"
#include "Helpers.h"

/* --------------------------------------------------------------- */

static const float idleWaitTime = 3.f;
static const float idleDetectAngle = 20.f; // Angle of half the detection cone.
static const int idleDetectRadius = 4; // Radius of active detection cone, in tiles.
static const int idleImmediateDetectRadius = 2; // Radius of immediate detection, in tiles

void FSMState_BBEM_Idle_OnEnter(FSM* controller, GameObject* target)
{
	controller->timeElapsed = 0.f;
	controller->waitTime = idleWaitTime; // Set AI to wait for 3 seconds, doing nothing.

	// Make AI target Player!
	controller->targetPosition = controller->target->position; 
}

void FSMState_BBEM_Idle_OnExit(FSM* controller, GameObject* target)
{
	controller->waitTime = 0.f;
	controller->timeElapsed = 0.f;
}

void FSMState_BBEM_Idle_OnUpdate(FSM* controller, GameObject* target)
{
	// If the wait time is up, change to Roam state.
	// If target object is within a short radius around the Enemy, change to Chase state.
	// Else if target object is within line of sight of the Enemy, change to Chase state.
	//   - Line of sight will be within a detection angle infront of the AI.

	controller->timeElapsed += CP_System_GetDt();
	float distance = CP_Vector_Length(CP_Vector_Subtract(controller->go->position, controller->targetPosition));

	// If the wait time is up, change to Roam state.
	if (controller->timeElapsed > controller->waitTime)
	{
		controller->nextState = "BBEM_Roam";
	}
	// If target object is within a short radius around the Enemy, change to Chase state.
	else if (distance < (idleImmediateDetectRadius * controller->tileSize))
	{
		controller->nextState = "BBEM_Chase";
	}
	// If target object is within line of sight of the Enemy, change to Chase state.
	else if (0)
	{

	}
}

/* --------------------------------------------------------------- */

static const int roamNearRadiusMin = 2;
static const int roamNearRadiusMax = 6;
static const int roamReachedDistance = 1;

void FSMState_BBEM_Roam_OnEnter(FSM* controller, GameObject* target)
{
	// Search for a position near the target that is walkable.
	while (1)
	{
		//int deltaRow = RAND(roamNearRadiusMin, roamNearRadiusMax);
		//int deltaCol = RAND(roamNearRadiusMin, roamNearRadiusMax);

		controller->targetPosition = target->position; // TEST! For now just set the moveto position as target's position. In future need to change to a randomly selected tile position.
		break;
	}
}

void FSMState_BBEM_Roam_OnExit(FSM* controller, GameObject* target)
{
}

void FSMState_BBEM_Roam_OnUpdate(FSM* controller, GameObject* target)
{
	float distance = CP_Vector_Length(CP_Vector_Subtract(controller->go->position, controller->targetPosition));

	if (distance <= (roamReachedDistance * controller->tileSize))
	{
		controller->nextState = "BBEM_Idle";
	}
}

/* --------------------------------------------------------------- */

static const float chaseDetectAngle = 20.f;
static const int chaseDetectRadius = 4;
static const int chaseAttackRadius = 1;
static const int chaseLostRadius = 6;
static const int chaseDamageRadius = 1;

void FSMState_BBEM_Chase_OnEnter(FSM* controller, GameObject* target)
{
	// Make the AI chase the target!
	controller->targetPosition = target->position;
}

void FSMState_BBEM_Chase_OnExit(FSM* controller, GameObject* target)
{
}

void FSMState_BBEM_Chase_OnUpdate(FSM* controller, GameObject* target)
{
	float distance = CP_Vector_Length(CP_Vector_Subtract(controller->go->position, controller->targetPosition));

	// Target has run too far, time to Search for him around his last position.
	if (distance > (chaseLostRadius * controller->tileSize))
	{
		controller->nextState = "BBEM_Search";
	}
	// Check if target is within striking distance.
	else if (distance < (chaseAttackRadius * controller->tileSize))
	{
		printf("ATTACKING %s!!\n", target->tag);
		controller->nextState = "BBEM_Idle";
	}
	// Check if target is still within sight. (Detection cone w/ radius)
	else if (0)
	{

	}
}

/* --------------------------------------------------------------- */

void FSMState_BBEM_Search_OnEnter(FSM* controller, GameObject* target)
{

}

void FSMState_BBEM_Search_OnExit(FSM* controller, GameObject* target)
{
}

void FSMState_BBEM_Search_OnUpdate(FSM* controller, GameObject* target)
{
}

/* --------------------------------------------------------------- */