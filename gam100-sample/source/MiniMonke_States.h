#pragma once
#include "GameObject.h"

void FSMState_MM_Roam_OnEnter(GameObject* controller, GameObject* target);
void FSMState_MM_Roam_OnExit(GameObject* controller, GameObject* target);
void FSMState_MM_Roam_OnUpdate(GameObject* controller, GameObject* target);

void FSMState_MM_Chase_OnEnter(GameObject* controller, GameObject* target);
void FSMState_MM_Chase_OnExit(GameObject* controller, GameObject* target);
void FSMState_MM_Chase_OnUpdate(GameObject* controller, GameObject* target);