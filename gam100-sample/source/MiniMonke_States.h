#pragma once
#include "FSM.h"

void FSMState_MM_Idle_OnEnter(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_MM_Idle_OnExit(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_MM_Idle_OnUpdate(FSM* controller, CP_Vector* newTargetPosition);

void FSMState_MM_Roam_OnEnter(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_MM_Roam_OnExit(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_MM_Roam_OnUpdate(FSM* controller, CP_Vector* newTargetPosition);

void FSMState_MM_Chase_OnEnter(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_MM_Chase_OnExit(FSM* controller, CP_Vector* newTargetPosition);
void FSMState_MM_Chase_OnUpdate(FSM* controller, CP_Vector* newTargetPosition);