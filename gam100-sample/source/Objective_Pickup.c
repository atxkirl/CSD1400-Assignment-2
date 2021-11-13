#pragma once
#include "Objective_Pickup.h"
#include "SystemManager.h"
#include "Player.h"
#include "Dialogue.h"


GameObject* pickup1 = NULL;
GameObject* pickup2 = NULL;
GameObject* drop1 = NULL;
GameObject* drop2 = NULL;
Collider* k;
Renderer* rd = NULL;
#include <stdio.h>

void OB_PickupOnCollision(Collider* left, Collider* right)
{
	if (strcmp(left->obj->tag, "wood") == 0 && strcmp(right->obj->tag, "player") == 0) {
		SM_DeleteGameObject(pickup1);
		printf("pick up wood\n");

		DM_PrintDialogue("You got dem wood.", DIALOGUE_CLOSEBUTTON);
	}

	if (strcmp(left->obj->tag, "fuel") == 0 && strcmp(right->obj->tag, "player") == 0) {
		SM_DeleteGameObject(pickup2);
		printf("pick up fuel\n");
	}

	if (strcmp(left->obj->tag, "wooddrop") == 0 && strcmp(right->obj->tag, "player") == 0) {
		//SM_DeleteGameObject(drop1);
		//printf("drop off wood\n");
	}

	if (strcmp(left->obj->tag, "fueldrop") == 0 && strcmp(right->obj->tag, "player") == 0) {
		//SM_DeleteGameObject(drop2);
		//printf("drop off fuel\n");
	}
}

void OB_PickupInit()
{
	//SM_SystemsInit();

	//PLY_CreatePlayer(90, 90);

	pickup1 = GOM_Create2(RECTANGLE, CP_Vector_Set(130, 130), 0.0f, CP_Vector_Set(30, 30));
	pickup1->tag = "wood";
	k = CLM_AddComponent(pickup1);
	CLM_Set(k, COL_BOX, OB_PickupOnCollision);
	rd = RM_AddComponent(pickup1);
	rd->text = "item1";
	//RM_LoadImage(rd, "Assets/bananaboi.png");

	pickup2 = GOM_Create2(RECTANGLE, CP_Vector_Set(0, 0), 0.0f, CP_Vector_Set(30, 30));
	pickup2->tag = "fuel";
	k = CLM_AddComponent(pickup2);
	CLM_Set(k, COL_BOX, OB_PickupOnCollision);
	rd = RM_AddComponent(pickup2);
	rd->text = "item2";
	//RM_LoadImage(rd, "Assets/bananaboi.png");

	drop1 = GOM_Create2(RECTANGLE, CP_Vector_Set(500, 500), 0.0f, CP_Vector_Set(50, 50));
	drop1->tag = "wooddrop";
	k = CLM_AddComponent(drop1);
	CLM_Set(k, COL_BOX, OB_PickupOnCollision);
	k->isLockedPos = 1;
	rd = RM_AddComponent(drop1);
	rd->text = "drop1";
	RM_LoadImage(rd, "Assets/sand-tiles/test-sea-sprite.png");
	Animation* a = AM_AddComponent(drop1);
	AM_SetSprite(a, 2, 1, 2, 2);

	drop2 = GOM_Create2(RECTANGLE, CP_Vector_Set(550, 550), 0.0f, CP_Vector_Set(30, 30));
	drop2->tag = "fueldrop";
	k = CLM_AddComponent(drop2);
	CLM_Set(k, COL_BOX, OB_PickupOnCollision);
	rd = RM_AddComponent(drop2);
	rd->text = "drop2";
	//RM_LoadImage(rd, "Assets/bananaboi.png");
}

void OB_PickupUpdate()
{
	//SM_SystemsPreUpdate();

	//PLY_Update();

	//SM_SystemsUpdate();

	//SM_SystemsLateUpdate();
}
