#pragma once

#include "LinkedList.h"
#include "FSM.h"
#include "Helpers.h"
#include "BBEM_States.h"
#include "MiniMonke_States.h"

LinkedList* allStates;

State BBEM_Idle;
State BBEM_Roam;
State BBEM_Chase;
State BBEM_Search;

State MM_Idle;
State MM_Roam;
State MM_Chase;


void AIM_Init(void);

void AIM_Clear(void);

void AIM_ChangeStates(char* stateName, FSM* controller, GameObject* target, CP_Vector targetPosition);

FSM* AIM_CreateFSM(char* startingStateName, GameObject* controller, GameObject* target);

static State* AIM_FindState(char* stateName);