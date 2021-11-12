#include "FSM.h"

FSM* FSM_CreateFSM(char* startingStateName, GameObject* controller)
{
	FSM* value = malloc(sizeof(FSM));
	if (value != NULL)
	{
		State* state = FindState(startingStateName);
		if (state != NULL)
		{
			printf("Found state! Name=%s\n", state->name);
			value->onEnter = state->onEnter;
			value->onExit = state->onExit;
			value->onUpdate = state->onUpdate;
			value->currentState = state->name;
			value->currentStateSpeed = state->stateSpeed;
		}
		else
		{
			printf("Didn't find state! Name=%s\n", startingStateName);
		}

		value->go = controller;
		value->target = NULL;

		LL_Add(&stateMachines, value);
		return value;
	}
	return NULL;
}

void FSM_ChangeState(char* stateName, GameObject* controller, GameObject* target)
{
	if (stateMachines == NULL)
		return;

	// Loop through all StateMachines and find the one with the correct controller GameObject

	FSM* temp = FindStateMachine(controller);
	if (temp)
	{
		// Invoke onExit of the current state before changing.
		temp->onExit(controller, target);

		// Search through all states to find the matching one,
		// then change the function ptrs here.
		State* state = FindState(stateName);
		if (state)
		{
			// Change the function ptrs here.
			temp->onEnter = state->onEnter;
			temp->onExit = state->onExit;
			temp->onUpdate = state->onUpdate;

			// Update the name of the current state.
			temp->currentState = state->name;

			// Update the speed
			temp->currentStateSpeed = state->stateSpeed;

			// Invoke onEnter of the newly changed current state.
			temp->onEnter(controller, target);
		}
	}
}

void FSM_Init(void)
{
	// Big Bad Evil Monkey States

	BBEM_Idle.name = "BBEM_Idle";
	BBEM_Idle.stateSpeed = 100.f;
	BBEM_Idle.onEnter = FSMState_Shared_Idle_OnEnter;
	BBEM_Idle.onExit = FSMState_Shared_Idle_OnExit;
	BBEM_Idle.onUpdate = FSMState_Shared_Idle_OnUpdate;

	BBEM_Roam.name = "BBEM_Roam";
	BBEM_Roam.stateSpeed = 150.f;
	BBEM_Roam.onEnter = FSMState_BBEM_Roam_OnEnter;
	BBEM_Roam.onExit = FSMState_BBEM_Roam_OnExit;
	BBEM_Roam.onUpdate = FSMState_BBEM_Roam_OnUpdate;

	BBEM_Chase.name = "BBEM_Chase";
	BBEM_Chase.stateSpeed = 200.f;
	BBEM_Chase.onEnter = FSMState_BBEM_Roam_OnEnter;
	BBEM_Chase.onExit = FSMState_BBEM_Roam_OnExit;
	BBEM_Chase.onUpdate = FSMState_BBEM_Roam_OnUpdate;

	BBEM_Search.name = "BBEM_Search";
	BBEM_Search.stateSpeed = 100.f;
	BBEM_Search.onEnter = FSMState_BBEM_Roam_OnEnter;
	BBEM_Search.onExit = FSMState_BBEM_Roam_OnExit;
	BBEM_Search.onUpdate = FSMState_BBEM_Roam_OnUpdate;

	LL_Add(&states, &BBEM_Roam);
	LL_Add(&states, &BBEM_Chase);
	LL_Add(&states, &BBEM_Search);


	// Mini Monke States

	MM_Idle.name = "MM_Idle";
	MM_Idle.stateSpeed = 100.f;
	MM_Idle.onEnter = FSMState_Shared_Idle_OnEnter;
	MM_Idle.onExit = FSMState_Shared_Idle_OnExit;
	MM_Idle.onUpdate = FSMState_Shared_Idle_OnUpdate;

	MM_Roam.name = "MM_Roam";
	MM_Roam.stateSpeed = 100.f;
	MM_Roam.onEnter = FSMState_MM_Roam_OnEnter;
	MM_Roam.onExit = FSMState_MM_Roam_OnExit;
	MM_Roam.onUpdate = FSMState_MM_Roam_OnUpdate;

	MM_Chase.name = "MM_Chase";
	MM_Chase.stateSpeed = 100.f;
	MM_Chase.onEnter = FSMState_MM_Roam_OnEnter;
	MM_Chase.onExit = FSMState_MM_Roam_OnExit;
	MM_Chase.onUpdate = FSMState_MM_Roam_OnUpdate;

	LL_Add(&states, &MM_Roam);
	LL_Add(&states, &MM_Chase);
}

void FSM_Clear(void)
{
	LL_Clear(&states);
	LL_Clear(&stateMachines);
}

State* FindState(char* stateName)
{
	for (int n = 0; n < LL_GetCount(states); ++n)
	{
		State* state = (State*)LL_Get(states, n);
		if (state != NULL)
		{
			if (strcmp(state->name, stateName) == 1)
			{
				return state;
			}
		}
	}

	return NULL;
}

FSM* FindStateMachine(GameObject* controller)
{
	if (stateMachines == NULL || LL_IsEmpty(stateMachines))
		return NULL;

	for (int i = 0; i < LL_GetCount(stateMachines); ++i)
	{
		FSM* temp = (FSM*)LL_Get(stateMachines, i);
		if (temp == NULL)
			continue;

		if (temp->go == controller)
			return temp;
	}

	return NULL;
}
