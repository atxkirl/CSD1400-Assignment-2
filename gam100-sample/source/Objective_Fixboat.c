#include "Objective_Fixboat.h"
#include "SystemManager.h"
#include "Colors.h"


GameObject* part1;
GameObject* part2;
GameObject* part3;
GameObject* title, *cross;
GameObject* UI;
GameObject* boatfixed1;
GameObject* boatfixed2;
GameObject* boatfixed3;
GameObject* brokenboat;
GameObject* fix1Col, * fix2Col, *fix3Col;
int isFixed1, isFixed2, isFixed3;
GameObject* hold = NULL;
#include <stdio.h>

void OB_FixBoatOnCollision(Collider* left, Collider* right)
{
	if (strcmp(right->obj->tag, "Click") == 0)
	{
		if (strcmp(left->obj->tag, "part1") == 0)
			hold = left->obj;
		else if (strcmp(left->obj->tag, "part2") == 0)
			hold = left->obj;
		else if (strcmp(left->obj->tag, "part3") == 0)
			hold = left->obj;
		else if (strcmp(left->obj->tag, "cross") == 0)
		{
			OB_FixBoatUnTrigger();
		}
		return;
	}
	if (CP_Input_MouseReleased(MOUSE_BUTTON_1))
	{
		if (strcmp(left->obj->tag, "part1") == 0 && strcmp(right->obj->tag, "fix1") == 0)
		{
			SM_DeleteGameObject(left->obj);
			SM_DeleteGameObject(right->obj);
			isFixed1 = 1;
			boatfixed1->isEnabled = 1;
		}
		else if (strcmp(left->obj->tag, "part2") == 0 && strcmp(right->obj->tag, "fix2") == 0)
		{
			SM_DeleteGameObject(left->obj);
			SM_DeleteGameObject(right->obj);
			isFixed2 = 1;
			boatfixed2->isEnabled = 1;
		}
		else if (strcmp(left->obj->tag, "part3") == 0 && strcmp(right->obj->tag, "fix3") == 0)
		{
			SM_DeleteGameObject(left->obj);
			SM_DeleteGameObject(right->obj);
			isFixed3 = 1;
			boatfixed3->isEnabled = 1;
		}

		return;
	}
	
}

void OB_FixBoatInit()
{
	float screenWidth, screenHeight;
	RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
	Collider* c = NULL;

	title = GOM_Create2(EMPTY, CP_Vector_Set(screenWidth * 0.5f, 75), 0, CP_Vector_Set(800, 100));
	Renderer* r = RM_AddComponent(title);
	r->color.a = 0;
	r->text = "Fix the boat!";
	r->renderPriority = PRI_UI;
	r->textScale = CP_Vector_Set(3.0f, 3.0f);

	UI = GOM_Create(RECTANGLE);
	r = RM_AddComponent(UI);
	r->color = CP_Color_Create(200,200,200,255);
	r->renderPriority = PRI_UI;
	UI->scale = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.7f);
	UI->position = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f);

	cross = GOM_Create(RECTANGLE);
	cross->position = CP_Vector_Set(screenWidth * 0.775f, screenHeight * 0.225f);
	cross->scale = CP_Vector_Set(50, 50);
	cross->tag = "cross";
	r = RM_AddComponent(cross);
	RM_LoadImage(r, "Assets/cross.png");
	r->renderPriority = PRI_UI;
	c = CLM_AddComponent(cross);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	CP_Vector boatPos = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.4f);
	CP_Vector boatScale = CP_Vector_Set(screenHeight * 0.4f, screenHeight * 0.4f); //sq
	CP_Vector boatPixelScale = CP_Vector_Set(boatScale.x / 36.0f, boatScale.y / 36.0f);
	CP_Vector boatTopLeft = CP_Vector_Set(boatPos.x - boatScale.x * 0.5f, boatPos.y - boatScale.y * 0.5f);
	CP_Vector part1MatchPos = CP_Vector_Set(boatTopLeft.x + 18.0f * boatPixelScale.x, 
		boatTopLeft.y + 12.0f * boatPixelScale.y);
	CP_Vector part2MatchPos = CP_Vector_Set(boatTopLeft.x + 9.0f * boatPixelScale.x, 
		boatTopLeft.y + 19.0f * boatPixelScale.y);
	CP_Vector part3MatchPos = CP_Vector_Set(boatTopLeft.x + 24.0f * boatPixelScale.x, 
		boatTopLeft.y + 33.0f * boatPixelScale.y);
	CP_Vector partScale = CP_Vector_Set(10.0f * boatPixelScale.x, 2.0f * boatPixelScale.y); //horizontal
	//==1==
	boatfixed1 = GOM_Create2(RECTANGLE, boatPos , 0 , boatScale);
	r = RM_AddComponent(boatfixed1);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatfix1.png");
	fix1Col = GOM_Create2(RECTANGLE, part1MatchPos, 0, CP_Vector_Set(partScale.y, partScale.x));
	fix1Col->tag = "fix1";
	//r = RM_AddComponent(g);
	//r->renderPriority = PRI_UI;
	//r->color = CP_Color_Create(255, 0, 0, 180);
	c = CLM_AddComponent(fix1Col);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;
	//==2==
	boatfixed2 = GOM_Create2(RECTANGLE, boatPos, 0, boatScale);
	r = RM_AddComponent(boatfixed2);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatfix2.png");
	fix2Col = GOM_Create2(RECTANGLE, part2MatchPos, 0, CP_Vector_Set(partScale.x, partScale.y));
	fix2Col->tag = "fix2";
	c = CLM_AddComponent(fix2Col);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;
	//==3==
	boatfixed3 = GOM_Create2(RECTANGLE, boatPos, 0, boatScale);
	r = RM_AddComponent(boatfixed3);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatfix3.png");
	fix3Col = GOM_Create2(RECTANGLE, part3MatchPos, 0, CP_Vector_Set(partScale.x, partScale.y));
	fix3Col->tag = "fix3";
	c = CLM_AddComponent(fix3Col);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	brokenboat = GOM_Create2(RECTANGLE, boatPos, 0, boatScale);
	r = RM_AddComponent(brokenboat);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/brokenboat.png");

	float xlb = 0.325f, xub = 0.675f, ylb = 0.65f, yub = 0.75f;
	//==1==
	part1 = GOM_Create2(RECTANGLE, 
		CP_Vector_Set(screenWidth * CP_Random_RangeFloat(xlb, xub), 
			screenHeight * CP_Random_RangeFloat(ylb, yub)),
		CP_Random_RangeFloat(0.0f, 359.9f), CP_Vector_Set(screenHeight * 0.2f, screenHeight * 0.2f));
	part1->tag = "part1";
	r = RM_AddComponent(part1);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatpart1scaled.png");
	c = CLM_AddComponent(part1);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->useScaleValue = 0; c->width = 50; c->height = 50;
	c->isTrigger = 1;
	//==2==
	part2 = GOM_Create(RECTANGLE);
	part2->position = CP_Vector_Set(screenWidth * CP_Random_RangeFloat(xlb, xub),
		screenHeight * CP_Random_RangeFloat(ylb, yub));
	part2->rotation = CP_Random_RangeFloat(0.0f, 359.9f);
	part2->scale = CP_Vector_Set(screenHeight * 0.2f, screenHeight * 0.2f); //sq
	part2->tag = "part2";
	r = RM_AddComponent(part2);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatpart2scaled.png");
	c = CLM_AddComponent(part2);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->useScaleValue = 0; c->width = 50; c->height = 50;
	c->isTrigger = 1;
	//==3==
	part3 = GOM_Create(RECTANGLE);
	part3->position = CP_Vector_Set(screenWidth * CP_Random_RangeFloat(xlb, xub),
		screenHeight * CP_Random_RangeFloat(ylb, yub));
	part3->rotation = CP_Random_RangeFloat(0.0f, 359.9f);
	part3->scale = CP_Vector_Set(screenHeight * 0.2f, screenHeight * 0.2f); //sq
	part3->tag = "part3";
	r = RM_AddComponent(part3);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatpart3scaled.png");
	c = CLM_AddComponent(part3);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->useScaleValue = 0; c->width = 50; c->height = 50;
	c->isTrigger = 1;
	


	title->isEnabled = UI->isEnabled = cross->isEnabled = 0;
	brokenboat->isEnabled = 0;
	boatfixed1->isEnabled = boatfixed2->isEnabled = boatfixed3->isEnabled = 0;
	part1->isEnabled = part2->isEnabled = part3->isEnabled = 0;
	isFixed1 = isFixed2 = isFixed3 = 0;
	fix1Col->isEnabled = fix2Col->isEnabled = fix3Col->isEnabled = 0;
}

void OB_FixBoatUpdate()
{
	if (CP_Input_MouseDown(MOUSE_BUTTON_1))
	{
		if (hold)
		{
			float posX = CP_Input_GetMouseX();
			float posY = CP_Input_GetMouseY();
			hold->position = CP_Vector_Set(posX, posY);
		}
	}
	if (CP_Input_MouseReleased(MOUSE_BUTTON_1))
	{
		if (hold)
		{
			hold = NULL;
		}
	}

	if (isFixed1 && isFixed2 && isFixed3)
	{
		Renderer* r = RM_GetComponent(title);
		r->text = "Boat fixed!";
	}
}

void OB_FixBoatTrigger()
{
	title->isEnabled = UI->isEnabled = cross->isEnabled = 1;
	brokenboat->isEnabled = 1;
	isFixed1 ? boatfixed1->isEnabled = 1 :( part1->isEnabled = 1, boatfixed1->isEnabled = 0, fix1Col->isEnabled = 1);
	isFixed2 ? boatfixed2->isEnabled = 1 : (part2->isEnabled = 1, boatfixed2->isEnabled = 0, fix2Col->isEnabled = 1);
	isFixed3 ? boatfixed3->isEnabled = 1 : (part3->isEnabled = 1, boatfixed3->isEnabled = 0, fix3Col->isEnabled = 1);
}

void OB_FixBoatUnTrigger()
{
	title->isEnabled = UI->isEnabled = cross->isEnabled = 0;
	brokenboat->isEnabled = 0;
	boatfixed1->isEnabled = boatfixed2->isEnabled = boatfixed3->isEnabled = 0;
	if (part1) part1->isEnabled = 0;
	if (part2) part2->isEnabled = 0;
	if (part3) part3->isEnabled = 0;
	if (fix1Col) fix1Col->isEnabled = 0;
	if (fix2Col) fix2Col->isEnabled = 0;
	if (fix3Col) fix3Col->isEnabled = 0;
}
