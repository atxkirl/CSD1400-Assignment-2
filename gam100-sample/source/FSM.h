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
	GameObject* target;			// The target GameObject that this FSM is supposed to chase/attack.
	CP_Vector* targetPosition;	// The position that the controlled GameObject should move towards. Usually would be set to the Player's position, but can be used to goto positions NEAR the player.

	// Use function pointers to point to state functions, 
	// so this way changing states should be alot easier.
	StateFunctionPtr onEnter;	// Function pointer to the current state's OnEnter() function.
	StateFunctionPtr onExit;	// Function pointer to the current state's OnExit() function.
	StateFunctionPtr onUpdate;	// Function pointer to the current state's OnUpdate() function.

	char* currentState;
	float currentStateSpeed;
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
