#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GameObject.h"

typedef struct FSM FSM;
typedef void (*StateFunctionPtr)(FSM*, GameObject*);

/// <summary>
/// Each FSM will hold references to state functions, which will be changed when changing states.
/// </summary>
struct FSM
{
	GameObject* go;				// The GameObject that this FSM is supposed to control. Usually is the Enemy's GameObject.
	CP_Vector goFacingDir;		// The direction that the Enemy is facing towards.
	
	GameObject* target;			// The target GameObject that this FSM is supposed to chase/attack.
	CP_Vector targetPosition;	// The position that the controlled GameObject should move towards. Usually would be set to the Player's position, but can be used to goto positions NEAR the player.
 
	StateFunctionPtr onEnter;	// Function pointer to the current state's OnEnter() function.
	StateFunctionPtr onExit;	// Function pointer to the current state's OnExit() function.
	StateFunctionPtr onUpdate;	// Function pointer to the current state's OnUpdate() function.

	char* currentState;			// Name of the current state.
	char* nextState;			// Name of the next state to change to, if nextState != currentState then call AIM_ChangeState().
	float currentStateSpeed;	// Movement speed for the controlled AI based on the current speed.

	float timeElapsed;			
	float waitTime;				// Can be used to make the AI wait during a State, or do something for a period of time.

	float tileSize;				// The size of one tile in a level.
};

/// <summary>
/// Struct to hold data for an individual State. Helps to consolidate State data in one place.
/// </summary>
typedef struct
{
	char* name;
	float stateSpeed;
	StateFunctionPtr onEnter;
	StateFunctionPtr onExit;
	StateFunctionPtr onUpdate;
}State;
