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