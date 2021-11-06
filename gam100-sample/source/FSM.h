#pragma once

#include <stdio.h>
#include "LinkedList.h"
#include "GameObject.h"

// Finite State Machine:
//	- Each FSM will hold a reference to one GameObject
//	- Each FSM will hold references to unique functions, for onEnter, onExit, and update

typedef struct
{
	char* stateName;
	GameObject* go;

	FunctionPtr onEnter;
	FunctionPtr onExit;
	FunctionPtr update;
}FSM;