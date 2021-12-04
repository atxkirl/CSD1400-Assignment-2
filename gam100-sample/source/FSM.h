/*
@file	FSM.h
@author	Adrian Tan (t.xingkhiangadrian)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@date	06/11/2021
@brief	Contains structs that hold data relevant for AI statemachine, and AI
		pathfinding.
*//*______________________________________________________________________*/

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AStar.h"
#include "LinkedList.h"
#include "GameObjectManager.h"
#include "RenderManager.h"
#include "CollisionManager.h"

typedef struct FSM FSM;
typedef void (*StateFunctionPtr)(FSM*); // (FSM* controller)

/// <summary>
/// Each FSM will hold references to state functions, which will be changed when changing states.
/// </summary>
struct FSM
{
	// Enemy Variables:
	GameObject* controlledObject;	// The actual GameObject that this FSM is tagged to.
	CP_Vector controlledObjForward;	// The forward vector of the controlled GameObject.
	Renderer* controlledRenderer;
	Collider* controlledCollider;

	GameObject* controlledFOVObject;
	Renderer* controlledFOVRenderer;
	// Enemy State Display:
	GameObject* displayObject;
	Renderer* displayRenderer;

	// Target Variables:
	GameObject* targetObject;		// The GameObject that this AI is tracking. Typically would be the Player.
	CP_Vector* targetPosition;		// The Position of where the AI is supposed to go to. Typically would be set to targetObject's position, but can be changed to be a point in the map.
	CP_Vector targetPrevPosition;	// The Position of targetPosition since last pathing update. Used to check if repathing is needed.

	// State Variables:
	StateFunctionPtr onEnter;		// Function pointer to the current state's OnEnter() function.
	StateFunctionPtr onExit;		// Function pointer to the current state's OnExit() function.
	StateFunctionPtr onUpdate;		// Function pointer to the current state's OnUpdate() function.
	char* currentState;				// The Name of the current state of the FSM.
	char* nextState;				// The Name of the next state to change to.

	// Pathfinding Variables:
	AStar_Map* map;					// The map that this AI is currently in, used to calculating pathfinding.
	LinkedList* movementPath;		// LinkedList of map Nodes that the AI will follow for movement.
	float moveSpeed;				// The movement speed attached to the current state.

	// Detection Variables:
	int immediateDetectionRadius;	// Radius (in tiles) where the AI will immediately detect the Player, regardless of line of sight.
	int fovDetectionRadius;			// Radius (in tiles) where the AI will detect the Player, if Player is within FOV angle.
	float fovDetectionHalfAngle;	// Angle (in degrees) where the AI will detect the Player, if within FOV distance.
	CP_Vector fovDetectionForward;	// Forward vector of controlled GameObject scaled with tile size and fov detection radius.

	// Functionality Variables:
	float waitTime, elapsedTime;	// Allows a State to be "held" in wait for awhile, either as a countdown, or to do something for waitTime amount of seconds.
	float tileSize;					// Size of a single map tile. Set when creating this FSM.
	int searchCount;				// Number of times the enemy will search locations during Search state.
};

/// <summary>
/// Container to hold all function pointers for a State, as well as it's name.
/// </summary>
typedef struct
{
	char* name;
	StateFunctionPtr onEnter;
	StateFunctionPtr onExit;
	StateFunctionPtr onUpdate;
}State;
