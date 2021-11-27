/*
* @file		BBEM_States.h
* @author	Adrian Tan (t.xingkhiangadrian)
* @course	CSD1400 Software Engineering Project 1
* @Team		BananaBoi
* @date		26/11/2021
* @brief	Contains function declarations for the statemachine functions for
*			the Big Bad Evil Monkey behaviors.
*//*--------------------------------------------------------------------------*/

#pragma once

#include "FSM.h"

void FSMState_BBEM_Idle_OnEnter(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_BBEM_Idle_OnExit(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_BBEM_Idle_OnUpdate(FSM* controller, CP_Vector* newTargetPosition);

void FSMState_BBEM_Roam_OnEnter(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_BBEM_Roam_OnExit(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_BBEM_Roam_OnUpdate(FSM* controller, CP_Vector* newTargetPosition);

void FSMState_BBEM_Chase_OnEnter(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_BBEM_Chase_OnExit(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_BBEM_Chase_OnUpdate(FSM* controller, CP_Vector* newTargetPosition);

void FSMState_BBEM_Search_OnEnter(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_BBEM_Search_OnExit(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_BBEM_Search_OnUpdate(FSM* controller, CP_Vector* newTargetPosition);