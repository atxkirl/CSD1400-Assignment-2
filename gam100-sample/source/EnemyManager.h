#pragma once

#include <stdio.h>
#include "LinkedList.h"
#include "AIManager.h"
#include "AStar.h"
#include "GameObjectManager.h"
#include "RenderManager.h"

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


static const float movementUpdateFreq = 30; // Frequency (per second) for how often all Enemy movement should be updated.
static const float pathingUpdateFreq = 3; // Frequency (per second) for how often all Enemy pathfinding should be recalculated.

static float movementTimeElapsed, movementUpdateTime;
static float pathingTimeElapsed, pathingUpdateTime;

static const float distanceForReached = 5.f; // The minimum distance an Enemy must be from a Node's position to count as having "reached" that node.

static const LinkedList* enemyList; // List of all created Enemy instances.

/// <summary>
/// Allocates memory for an Enemy and returns a pointer to it.
/// </summary>
/// <param name="enemyName -">Name of this enemy instance.</param>
/// <param name="startingStateName -">Name of this enemy's starting state.</param>
/// <param name="position -">Spawn position of this enemy.</param>
/// <param name="map -">Pointer to the overall grid that this enemy will be spawned on. Used for pathfinding.</param>
/// <returns>Pointer to a newly allocated Enemy instance.</returns>
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