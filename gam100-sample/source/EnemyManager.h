#pragma once

#include <stdio.h>
#include "LinkedList.h"
#include "FSM.h"
#include "AStar.h"
#include "GameObjectManager.h"
#include "RenderManager.h"

//----------//
//	Enemy	//
//----------//

typedef struct
{
	GameObject* go; // The Enemy's GameObject

	Renderer* renderer;
	FSM* stateMachine;

	int targetRow, targetCol;
	int targetPrevRow, targetPrevCol;

	int movementPathIndex; // The index of the current node in the path.
	LinkedList* movementPath; // LinkedList of path nodes from this Enemy to it's target position.
	AStar_Map* currentMap; // This is the map of the level that this Enemy is currently in.
}Enemy;

//------------------//
//	Enemy Manager	//
//------------------//

static const float movementUpdateFreq = 10;
static const float pathingUpdateFreq = 1;

static float movementTimeElapsed, movementUpdateTime;
static float pathingTimeElapsed, pathingUpdateTime;

static const float distanceForReached = 5.f; // The minimum distance required to a node's position to count as "reached".
static float deltaX, deltaY;
static float movementSpeed;

LinkedList* enemyList;

Enemy* EM_CreateEnemy(char* enemyName, char* startingStateName, CP_Vector position, AStar_Map* map);

void EM_Init();
void EM_Update();
void EM_Clear();

/// <summary>
/// Update loop for enemy movement.
/// </summary>
static void EM_Update_FSMAndMovement();
/// <summary>
/// Update loop for enemy A* pathing.
/// </summary>
static void EM_Update_Pathing();