/*
@file	BBEM_States.c
@author	Adrian Tan (t.xingkhiangadrian)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@date	10/11/2021
@brief	Contains function definitions for the statemachine functions to
		implement the Big Bad Evil Monkey behaviors.
*//*______________________________________________________________________*/

#pragma once

#include "FSM.h"

/// <summary>
/// OnEnter function for Idle state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Idle_OnEnter(FSM* controller);
/// <summary>
/// OnExit function for Idle state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Idle_OnExit(FSM* controller);
/// <summary>
/// OnUpdate function for Idle state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Idle_OnUpdate(FSM* controller);

/// <summary>
/// OnEnter function for Roam state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Roam_OnEnter(FSM* controller);
/// <summary>
/// OnExit function for Roam state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Roam_OnExit(FSM* controller);
/// <summary>
/// OnUpdate function for Roam state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Roam_OnUpdate(FSM* controller);

/// <summary>
/// OnEnter function for Chase state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Chase_OnEnter(FSM* controller);
/// <summary>
/// OnExit function for Chase state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Chase_OnExit(FSM* controller);
/// <summary>
/// OnUpdate function for Chase state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Chase_OnUpdate(FSM* controller);

/// <summary>
/// OnEnter function for Search state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Search_OnEnter(FSM* controller);
/// <summary>
/// OnExit function for Search state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Search_OnExit(FSM* controller);
/// <summary>
/// OnUpdate function for Search state of the BBEM AI.
/// </summary>
/// <param name="controller">The unique FSM instance calling this function.</param>
void FSMState_BBEM_Search_OnUpdate(FSM* controller);