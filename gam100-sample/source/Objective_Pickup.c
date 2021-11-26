#pragma once
#include "Objective_Pickup.h"
#include "SystemManager.h"
#include "Player.h"
#include "Dialogue.h"


int iBoatPartsPicked;
#include <stdio.h>

void OB_PickupOnCollision(Collider* left, Collider* right)
{
	if (left->obj->type == BOAT_PARTS && strcmp(right->obj->tag, "player") == 0) {
		left->obj->isEnabled = 0;
		CLM_RemoveGO(left->obj);
		DM_PrintDialogue("Picked up boat parts!", DIALOGUE_CLOSEBUTTON);
	}
}

void OB_PickupInit()
{
	iBoatPartsPicked = 0;
}

void OB_PickupUpdate()
{
	//SM_SystemsPreUpdate();

	//PLY_Update();

	//SM_SystemsUpdate();

	//SM_SystemsLateUpdate();
}

int GetBoatParts()
{
	return iBoatPartsPicked;
}

void SetBoatParts(int iSetter)
{
	iBoatPartsPicked = iSetter;
}