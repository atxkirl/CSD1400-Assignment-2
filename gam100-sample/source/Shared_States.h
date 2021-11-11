#pragma once
#include "GameObject.h"

void FSMState_Shared_Idle_OnEnter(GameObject* controller, GameObject* target);
void FSMState_Shared_Idle_OnExit(GameObject* controller, GameObject* target);
void FSMState_Shared_Idle_OnUpdate(GameObject* controller, GameObject* target);