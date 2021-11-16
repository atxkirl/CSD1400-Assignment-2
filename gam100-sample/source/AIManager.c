#include "AIManager.h"

void AIM_Init(void)
{
	// Big Bad Evil Monkey States

	BBEM_Idle.name = "BBEM_Idle";
	BBEM_Idle.stateSpeed = 0.f;
	BBEM_Idle.onEnter = FSMState_BBEM_Idle_OnEnter;
	BBEM_Idle.onExit = FSMState_BBEM_Idle_OnExit;
	BBEM_Idle.onUpdate = FSMState_BBEM_Idle_OnUpdate;
	LL_Add(&allStates, &BBEM_Idle);

	BBEM_Roam.name = "BBEM_Roam";
	BBEM_Roam.stateSpeed = 100.f;
	BBEM_Roam.onEnter = FSMState_BBEM_Roam_OnEnter;
	BBEM_Roam.onExit = FSMState_BBEM_Roam_OnExit;
	BBEM_Roam.onUpdate = FSMState_BBEM_Roam_OnUpdate;
	LL_Add(&allStates, &BBEM_Roam);

	BBEM_Chase.name = "BBEM_Chase";
	BBEM_Chase.stateSpeed = 300.f;
	BBEM_Chase.onEnter = FSMState_BBEM_Chase_OnEnter;
	BBEM_Chase.onExit = FSMState_BBEM_Chase_OnExit;
	BBEM_Chase.onUpdate = FSMState_BBEM_Chase_OnUpdate;
	LL_Add(&allStates, &BBEM_Chase);

	BBEM_Search.name = "BBEM_Search";
	BBEM_Search.stateSpeed = 100.f;
	BBEM_Search.onEnter = FSMState_BBEM_Roam_OnEnter;
	BBEM_Search.onExit = FSMState_BBEM_Roam_OnExit;
	BBEM_Search.onUpdate = FSMState_BBEM_Roam_OnUpdate;
	LL_Add(&allStates, &BBEM_Search);



	// Mini Monke States

	MM_Idle.name = "MM_Idle";
	MM_Idle.stateSpeed = 100.f;
	MM_Idle.onEnter = FSMState_MM_Idle_OnEnter;
	MM_Idle.onExit = FSMState_MM_Idle_OnExit;
	MM_Idle.onUpdate = FSMState_MM_Idle_OnUpdate;
	LL_Add(&allStates, &MM_Idle);

	MM_Roam.name = "MM_Roam";
	MM_Roam.stateSpeed = 100.f;
	MM_Roam.onEnter = FSMState_MM_Roam_OnEnter;
	MM_Roam.onExit = FSMState_MM_Roam_OnExit;
	MM_Roam.onUpdate = FSMState_MM_Roam_OnUpdate;
	LL_Add(&allStates, &MM_Roam);

	MM_Chase.name = "MM_Chase";
	MM_Chase.stateSpeed = 100.f;
	MM_Chase.onEnter = FSMState_MM_Roam_OnEnter;
	MM_Chase.onExit = FSMState_MM_Roam_OnExit;
	MM_Chase.onUpdate = FSMState_MM_Roam_OnUpdate;
	LL_Add(&allStates, &MM_Chase);
}

void AIM_Clear(void)
{
	LL_Clear(&allStates);
}

void AIM_ChangeStates(char* stateName, FSM* controller, GameObject* target, CP_Vector targetPosition)
{
	// Exit the current state first.
	if(controller->onExit)
		controller->onExit(controller, controller->target);

	// Look for new state with matching name.
	State* state = AIM_FindState(stateName);
	if (state)
	{
		// Update function pointers to point to new state's functions.
		controller->onEnter = state->onEnter;
		controller->onExit = state->onExit;
		controller->onUpdate = state->onUpdate;

		controller->currentState = state->name;
		controller->nextState = state->name;
		controller->currentStateSpeed = state->stateSpeed;

		controller->timeElapsed = 0.f;

		controller->target = target;
		controller->targetPosition = targetPosition;

		controller->tileSize = GetTileScale();
	}

	// Invoke new state's update.
	if(controller->onEnter)
		controller->onEnter(controller, controller->target);
}

FSM* AIM_CreateFSM(char* startingStateName, GameObject* controller, GameObject* target)
{
	FSM* value = malloc(sizeof(FSM));
	if (value)
	{
		value->onEnter = NULL;
		value->onExit = NULL;
		value->onUpdate = NULL;

		value->go = controller;

		AIM_ChangeStates(startingStateName, value, target, controller->position);

		return value;
	}
	return NULL;
}

State* AIM_FindState(char* stateName)
{
	for (int i = 0; i < LL_GetCount(allStates); ++i)
	{
		State* state = (State*)LL_Get(allStates, i);
		if (state && strcmp(state->name, stateName) == 0)
				return state;
	}

	return NULL;
}
