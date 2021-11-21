#include "Objective_Fixboat.h"
#include "SystemManager.h"
#include "Colors.h"


GameObject* ofb_part1;
GameObject* ofb_part2;
GameObject* ofb_part3;
GameObject* ofb_title, *ofb_cross;
GameObject* ofb_UI;
Renderer* ofb_fixed1;
Renderer* ofb_fixed2;
Renderer* ofb_fixed3;
GameObject* ofb_brokenboat;
GameObject* ofb_fix1Col, * ofb_fix2Col, *ofb_fix3Col;
int ofb_isFixed1, ofb_isFixed2, ofb_isFixed3;
GameObject* ofb_hold = NULL;
#include <stdio.h>
int isActive;


void FixBoatEnableImage()
{
	ofb_fixed1->isEnabled = ofb_isFixed1;
	ofb_fixed2->isEnabled = ofb_isFixed2;
	ofb_fixed3->isEnabled = ofb_isFixed3;
}
void FixBoatRandomisePartsTransform()
{
	float xlb = 0.325f, xub = 0.675f, ylb = 0.65f, yub = 0.75f;
	float screenWidth, screenHeight;
	RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);

	if (ofb_part1)
	{
		ofb_part1->position = CP_Vector_Set(screenWidth * CP_Random_RangeFloat(xlb, xub), screenHeight * CP_Random_RangeFloat(ylb, yub));
		ofb_part1->rotation = CP_Random_RangeFloat(0.0f, 360.0f);
		ofb_part1->scale = CP_Vector_Set(screenHeight * 0.2f, screenHeight * 0.2f);
	}

	if (ofb_part2)
	{
		ofb_part2->position = CP_Vector_Set(screenWidth * CP_Random_RangeFloat(xlb, xub),
			screenHeight * CP_Random_RangeFloat(ylb, yub));
		ofb_part2->rotation = CP_Random_RangeFloat(0.0f, 360.0f);
		ofb_part2->scale = CP_Vector_Set(screenHeight * 0.2f, screenHeight * 0.2f); //sq
	}

	if (ofb_part3)
	{
		ofb_part3->position = CP_Vector_Set(screenWidth * CP_Random_RangeFloat(xlb, xub),
			screenHeight * CP_Random_RangeFloat(ylb, yub));
		ofb_part3->rotation = CP_Random_RangeFloat(0.0f, 360.0f);
		ofb_part3->scale = CP_Vector_Set(screenHeight * 0.2f, screenHeight * 0.2f); //sq
	}

}

void OB_FixBoatOnCollision(Collider* left, Collider* right)
{
	if (strcmp(right->obj->tag, "Click") == 0)
	{
		if (strcmp(left->obj->tag, "ofb_part1") == 0)
			ofb_hold = left->obj;
		else if (strcmp(left->obj->tag, "ofb_part2") == 0)
			ofb_hold = left->obj;
		else if (strcmp(left->obj->tag, "ofb_part3") == 0)
			ofb_hold = left->obj;
		else if (strcmp(left->obj->tag, "ofb_cross") == 0)
		{
			OB_FixBoatUnTrigger();
		}
		return;
	}
	if (CP_Input_MouseReleased(MOUSE_BUTTON_1))
	{
		if (strcmp(left->obj->tag, "ofb_part1") == 0 && strcmp(right->obj->tag, "fix1") == 0)
		{
			left->obj->isEnabled = 0;
			right->obj->isEnabled = 0;
			ofb_isFixed1 = 1;
		}
		else if (strcmp(left->obj->tag, "ofb_part2") == 0 && strcmp(right->obj->tag, "fix2") == 0)
		{
			left->obj->isEnabled = 0;
			right->obj->isEnabled = 0;
			ofb_isFixed2 = 1;
		}
		else if (strcmp(left->obj->tag, "ofb_part3") == 0 && strcmp(right->obj->tag, "fix3") == 0)
		{
			left->obj->isEnabled = 0;
			right->obj->isEnabled = 0;
			ofb_isFixed3 = 1;
		}
		FixBoatEnableImage();

		return;
	}
	
}

void OB_FixBoatInit()
{
	float screenWidth, screenHeight;
	RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
	Collider* c = NULL;

	ofb_title = GOM_Create2(RECTANGLE, CP_Vector_Set(screenWidth * 0.5f, 75), 0, CP_Vector_Set(screenWidth * 0.6f, 60));
	Renderer* r = RM_AddComponent(ofb_title);
	r->color.a = 200;
	//r->text = "Fix the boat!";
	RM_SetText(r, "Fix the boat!");
	r->renderPriority = PRI_UI;
	r->textScale = CP_Vector_Set(3.0f, 3.0f);

	ofb_UI = GOM_Create(RECTANGLE);
	r = RM_AddComponent(ofb_UI);
	r->color = CP_Color_Create(200,200,200,255);
	r->renderPriority = PRI_UI;
	ofb_UI->scale = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.7f);
	ofb_UI->position = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f);

	ofb_cross = GOM_Create(RECTANGLE);
	ofb_cross->position = CP_Vector_Set(screenWidth * 0.775f, screenHeight * 0.225f);
	ofb_cross->scale = CP_Vector_Set(50, 50);
	ofb_cross->tag = "ofb_cross";
	r = RM_AddComponent(ofb_cross);
	RM_LoadImage(r, "Assets/cross.png");
	r->renderPriority = PRI_UI;
	c = CLM_AddComponent(ofb_cross);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	CP_Vector boatPos = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.4f);
	CP_Vector boatScale = CP_Vector_Set(screenHeight * 0.4f, screenHeight * 0.4f); //sq
	CP_Vector boatPixelScale = CP_Vector_Set(boatScale.x / 36.0f, boatScale.y / 36.0f);
	CP_Vector boatTopLeft = CP_Vector_Set(boatPos.x - boatScale.x * 0.5f, boatPos.y - boatScale.y * 0.5f);
	CP_Vector ofb_part1MatchPos = CP_Vector_Set(boatTopLeft.x + 18.0f * boatPixelScale.x, 
		boatTopLeft.y + 12.0f * boatPixelScale.y);
	CP_Vector ofb_part2MatchPos = CP_Vector_Set(boatTopLeft.x + 9.0f * boatPixelScale.x, 
		boatTopLeft.y + 19.0f * boatPixelScale.y);
	CP_Vector ofb_part3MatchPos = CP_Vector_Set(boatTopLeft.x + 24.0f * boatPixelScale.x, 
		boatTopLeft.y + 33.0f * boatPixelScale.y);
	CP_Vector partScale = CP_Vector_Set(10.0f * boatPixelScale.x, 2.0f * boatPixelScale.y); //horizontal
	//=====
	ofb_brokenboat = GOM_Create2(RECTANGLE, boatPos, 0, boatScale);
	//==1==
	ofb_fixed1 = RM_AddComponent(ofb_brokenboat);
	ofb_fixed1->renderPriority = PRI_UI;
	RM_LoadImage(ofb_fixed1, "Assets/boat/boatfix1.png");

	ofb_fix1Col = GOM_Create2(RECTANGLE, ofb_part1MatchPos, 0, CP_Vector_Set(partScale.y, partScale.x));
	ofb_fix1Col->tag = "fix1";
	c = CLM_AddComponent(ofb_fix1Col);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;
	//==2==
	ofb_fixed2 = RM_AddComponent(ofb_brokenboat);
	ofb_fixed2->renderPriority = PRI_UI;
	RM_LoadImage(ofb_fixed2, "Assets/boat/boatfix2.png");

	ofb_fix2Col = GOM_Create2(RECTANGLE, ofb_part2MatchPos, 0, CP_Vector_Set(partScale.x, partScale.y));
	ofb_fix2Col->tag = "fix2";
	c = CLM_AddComponent(ofb_fix2Col);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;
	//==3==
	ofb_fixed3 = RM_AddComponent(ofb_brokenboat);
	ofb_fixed3->renderPriority = PRI_UI;
	RM_LoadImage(ofb_fixed3, "Assets/boat/boatfix3.png");

	ofb_fix3Col = GOM_Create2(RECTANGLE, ofb_part3MatchPos, 0, CP_Vector_Set(partScale.x, partScale.y));
	ofb_fix3Col->tag = "fix3";
	c = CLM_AddComponent(ofb_fix3Col);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	ofb_brokenboat = GOM_Create2(RECTANGLE, boatPos, 0, boatScale);
	r = RM_AddComponent(ofb_brokenboat);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/brokenboat.png");


	//==1==
	ofb_part1 = GOM_Create(RECTANGLE);

	ofb_part1->tag = "ofb_part1";
	r = RM_AddComponent(ofb_part1);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatpart1scaled.png");
	c = CLM_AddComponent(ofb_part1);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->useScaleValue = 0; c->width = 50; c->height = 50;
	c->isTrigger = 1;
	//==2==
	ofb_part2 = GOM_Create(RECTANGLE);

	ofb_part2->tag = "ofb_part2";
	r = RM_AddComponent(ofb_part2);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatpart2scaled.png");
	c = CLM_AddComponent(ofb_part2);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->useScaleValue = 0; c->width = 50; c->height = 50;
	c->isTrigger = 1;
	//==3==
	ofb_part3 = GOM_Create(RECTANGLE);

	ofb_part3->tag = "ofb_part3";
	r = RM_AddComponent(ofb_part3);
	r->renderPriority = PRI_UI;
	RM_LoadImage(r, "Assets/boat/boatpart3scaled.png");
	c = CLM_AddComponent(ofb_part3);
	CLM_Set(c, COL_BOX, OB_FixBoatOnCollision);
	c->space = COLSPC_SCREEN;
	c->useScaleValue = 0; c->width = 50; c->height = 50;
	c->isTrigger = 1;
	
	FixBoatRandomisePartsTransform();

	isActive = 0;

	ofb_title->isEnabled = ofb_UI->isEnabled = ofb_cross->isEnabled = 0;
	ofb_brokenboat->isEnabled = 0;
	ofb_isFixed1 = ofb_isFixed2 = ofb_isFixed3 = 0;
	FixBoatEnableImage();
	ofb_part1->isEnabled = ofb_part2->isEnabled = ofb_part3->isEnabled = 0;
	ofb_fix1Col->isEnabled = ofb_fix2Col->isEnabled = ofb_fix3Col->isEnabled = 0;
}

void OB_FixBoatUpdate()
{
	if (CP_Input_MouseDown(MOUSE_BUTTON_1))
	{
		if (ofb_hold)
		{
			float posX = CP_Input_GetMouseX();
			float posY = CP_Input_GetMouseY();
			ofb_hold->position = CP_Vector_Set(posX, posY);
		}
	}
	if (CP_Input_MouseReleased(MOUSE_BUTTON_1))
	{
		if (ofb_hold)
		{
			ofb_hold = NULL;
		}
	}

	if (ofb_isFixed1 && ofb_isFixed2 && ofb_isFixed3)
	{
		Renderer* r = RM_GetComponent(ofb_title);
		//r->text = "Boat fixed!";
		RM_SetText(r, "Boat fixed!");
	}
}

void OB_FixBoatTrigger()
{
	ofb_title->isEnabled = ofb_UI->isEnabled = ofb_cross->isEnabled = 1;
	ofb_brokenboat->isEnabled = 1;
	//ofb_isFixed1 ? ofb_boatfixed1->isEnabled = 1 :( ofb_part1->isEnabled = 1, ofb_boatfixed1->isEnabled = 0, ofb_fix1Col->isEnabled = 1);
	//ofb_isFixed2 ? ofb_boatfixed2->isEnabled = 1 : (ofb_part2->isEnabled = 1, ofb_boatfixed2->isEnabled = 0, ofb_fix2Col->isEnabled = 1);
	//ofb_isFixed3 ? ofb_boatfixed3->isEnabled = 1 : (ofb_part3->isEnabled = 1, ofb_boatfixed3->isEnabled = 0, ofb_fix3Col->isEnabled = 1);
	if (ofb_part1) ofb_part1->isEnabled = 1;
	if (ofb_part2) ofb_part2->isEnabled = 1;
	if (ofb_part3) ofb_part3->isEnabled = 1;
	if (ofb_fix1Col) ofb_fix1Col->isEnabled = 1;
	if (ofb_fix2Col) ofb_fix2Col->isEnabled = 1;
	if (ofb_fix3Col) ofb_fix3Col->isEnabled = 1;

	ofb_isFixed1 = ofb_isFixed2 = ofb_isFixed3 = 0;
	FixBoatEnableImage();
	FixBoatRandomisePartsTransform();
	isActive = 1;

	Renderer* r = RM_GetComponent(ofb_title);
	RM_SetText(r, "Fix the boat!");
}

void OB_FixBoatUnTrigger()
{
	ofb_title->isEnabled = ofb_UI->isEnabled = ofb_cross->isEnabled = 0;
	ofb_brokenboat->isEnabled = 0;
	ofb_isFixed1 = ofb_isFixed2 = ofb_isFixed3 = 0;
	FixBoatEnableImage();
	if (ofb_part1) ofb_part1->isEnabled = 0;
	if (ofb_part2) ofb_part2->isEnabled = 0;
	if (ofb_part3) ofb_part3->isEnabled = 0;
	if (ofb_fix1Col) ofb_fix1Col->isEnabled = 0;
	if (ofb_fix2Col) ofb_fix2Col->isEnabled = 0;
	if (ofb_fix3Col) ofb_fix3Col->isEnabled = 0;
	isActive = 0;
	ofb_hold = NULL;
}

int OB_IsFixBoatComplete()
{
	return ofb_isFixed1 && ofb_isFixed2 && ofb_isFixed3;
}

int OB_IsFixBoatActive()
{
	return isActive;
}

void OB_SetFixBoatActive(int iSetter)
{
	isActive = iSetter;
}