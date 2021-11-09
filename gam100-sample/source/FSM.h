#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "GameObject.h"

// States
#include "BBEM_States.h"
#include "MiniMonke_States.h"
#include "Shared_States.h"

typedef void (*StateFunctionPtr)(GameObject*, GameObject*);

/// <summary>
/// Each FSM will hold references to state functions, which will be changed when changing states.
/// </summary>
typedef struct
{
	GameObject* go;		 // The GameObject that this FSM is supposed to control. Usually is the Enemy's GameObject.
	GameObject* target;  // The target GameObject that this FSM is supposed to chase/attack. Usually is the Player, but can be used to set movement waypoints.

	// Use function pointers to point to state functions, 
	// so this way changing states should be alot easier.
	StateFunctionPtr onEnter;	// Function pointer to the current state's OnEnter() function.
	StateFunctionPtr onExit;	// Function pointer to the current state's OnExit() function.
	StateFunctionPtr onUpdate;	// Function pointer to the current state's OnUpdate() function.

	char* currentState;
	float currentStateSpeed;
}FSM;

typedef struct
{
	char* name;
	float stateSpeed;
	StateFunctionPtr onEnter;
	StateFunctionPtr onExit;
	StateFunctionPtr onUpdate;
}State;

//------------------//
//	State Machine	//
//------------------//

LinkedList* stateMachines;
LinkedList* states;

State BBEM_Idle;
State BBEM_Roam;
State BBEM_Chase;
State BBEM_Search;

State MM_Idle;
State MM_Roam;
State MM_Chase;

FSM* FSM_CreateFSM(char* startingStateName, GameObject* controller);

void FSM_ChangeState(char* stateName, GameObject* controller, GameObject* target);

void FSM_Init(void);

void FSM_Clear(void);

static State* FindState(char* stateName);

static FSM* FindStateMachine(GameObject* controller);