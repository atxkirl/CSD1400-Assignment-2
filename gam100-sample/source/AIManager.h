#pragma once

#include "FSM.h"
#include "Helpers.h"
#include "BBEM_States.h"
#include "MiniMonke_States.h"

//----------------------//
// Finite State Machine //
// - Variables:			//
//----------------------//

LinkedList* allStates;

// Big Bad Evil Monke States:
State BBEM_Idle;
State BBEM_Roam;
State BBEM_Chase;
State BBEM_Search;
// Mini Monke States:
State MM_Idle;
State MM_Roam;
State MM_Chase;


//----------------------//
// Finite State Machine //
// - Functions:			//
//----------------------//

void AIM_ChangeStates(char* stateName, FSM* controller);

static void AIM_InitFSM(FSM* controller, char* startingStateName, GameObject* targetObject);

static void AIM_InitStates(void);

static State* AIM_FindState(char* stateName);


//--------------//
// Enemy Update //
// - Variables:	//
//--------------//

LinkedList* allEnemies;

static const float distanceTillReached = 0.1f;


//--------------//
// Enemy Update //
// - Functions:	//
//--------------//

void AIM_Init(void);

void AIM_Clear(void);

void AIM_Update();

FSM* AIM_CreateEnemy(char* enemyName, char* startStateName, CP_Vector enemyPos, GameObject* targetObj, AStar_Map* levelMap);