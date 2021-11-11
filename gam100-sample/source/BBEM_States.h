#pragma once
#include "GameObject.h"

void FSMState_BBEM_Roam_OnEnter(GameObject* controller, GameObject* target);
void FSMState_BBEM_Roam_OnExit(GameObject* controller, GameObject* target);
void FSMState_BBEM_Roam_OnUpdate(GameObject* controller, GameObject* target);

void FSMState_BBEM_Chase_OnEnter(GameObject* controller, GameObject* target);
void FSMState_BBEM_Chase_OnExit(GameObject* controller, GameObject* target);
void FSMState_BBEM_Chase_OnUpdate(GameObject* controller, GameObject* target);

void FSMState_BBEM_Search_OnEnter(GameObject* controller, GameObject* target);
void FSMState_BBEM_Search_OnExit(GameObject* controller, GameObject* target);
void FSMState_BBEM_Search_OnUpdate(GameObject* controller, GameObject* target);