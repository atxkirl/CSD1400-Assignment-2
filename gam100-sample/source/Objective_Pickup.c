/*!
@file            Objective_Pickup.c
@author          Koh Xin Yun (xinyun.k)
@co-author		 Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course          CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief           This file contains functions for Objective Pickup.
*//*______________________________________________________________________*/

#pragma once
#include "Objective_Pickup.h"
#include "SystemManager.h"
#include "Player.h"
#include "Dialogue.h"
#include "Loader.h"
#include "LevelEditor.h"


int iBoatPartsPicked;
#include <stdio.h>

/// <summary>
/// Handles the collision of the the GameObjects based on the left and right collider parameters.
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void OB_PickupOnCollision(Collider* left, Collider* right)
{
	if (left->obj->type == BOAT_PARTS && strcmp(right->obj->tag, "player") == 0) {
		left->obj->isEnabled = 0;
		//CLM_Remove(CLM_GetComponent(left->obj));
		iBoatPartsPicked++;
		DM_PrintDialogue("Picked up boat parts!", DIALOGUE_CLOSEBUTTON);
	}
}

/// <summary>
/// Initialises variables in this scene.
/// </summary>
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

/// <summary>
/// Update.
/// </summary>
void OB_PickupUpdate()
{
	//SM_SystemsPreUpdate();

	//PLY_Update();

	//SM_SystemsUpdate();

	//SM_SystemsLateUpdate();
}

/// <summary>
/// returns true of false for objective if it is picked
/// </summary>
/// <returns>int</returns>
int GetBoatParts()
{
	return iBoatPartsPicked;
}

/// <summary>
/// Sets active or not active for the objective
/// </summary>
/// <param name="iSetter">- 1 or 0 for active</param>
void SetBoatParts(int iSetter)
{
	iBoatPartsPicked = iSetter;
}