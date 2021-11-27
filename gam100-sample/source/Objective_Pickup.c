#pragma once
#include "Objective_Pickup.h"
#include "SystemManager.h"
#include "Player.h"
#include "Dialogue.h"
#include "Loader.h"
#include "LevelEditor.h"


int iBoatPartsPicked;
#include <stdio.h>

void OB_PickupOnCollision(Collider* left, Collider* right)
{
	if (left->obj->type == BOAT_PARTS && strcmp(right->obj->tag, "player") == 0) {
		left->obj->isEnabled = 0;
		//CLM_Remove(CLM_GetComponent(left->obj));
		iBoatPartsPicked++;
		DM_PrintDialogue("Picked up boat parts!", DIALOGUE_CLOSEBUTTON);
	}
}

void OB_PickupInit()
{
	iBoatPartsPicked = 0;

	for (int i = 0; i < NumGrids; ++i)
	{
		for (int j = 0; j < NumGrids; ++j)
		{
			Collider* c = CLM_AddComponent(gLoadedObjects->gGrid[i][j]);
			CLM_Set(c, COL_BOX, OB_PickupOnCollision);
			c->isTrigger = 1;
		}
	}
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