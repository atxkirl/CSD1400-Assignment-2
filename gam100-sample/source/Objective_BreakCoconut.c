/*!
@file		Objective_BreakCoconut.c
@author		Ow Hong Yu (ow.h)
@course		CSD 1400
@section	A
@brief		One of the objectives. It is a click on the coconut a few times
			until it is broken for objective complete
*/

#include "Objective_BreakCoconut.h"
#include "SystemManager.h"


int obc_stage = 0;
const int obc_MAX_STAGE = 3;
int obc_canHit = 1;
const float obc_delayHit = 0.5f;

GameObject* obc_cross;
GameObject* obc_UI, * obc_title;
GameObject* obc_coconut;
Renderer* r1, *r2, *r3, *r4;
int isActive;

/// <summary>
/// Enables or disable images based on current stage of breaking coconut
/// </summary>
/// <param name="_stage"></param>
void EnableImage(int _stage)
{
	r1->isEnabled = _stage == 0;
	r2->isEnabled = _stage == 1;
	r3->isEnabled = _stage == 2;
	r4->isEnabled = _stage == 3;
}
/// <summary>
/// On collision response for break coconut objective objects
/// </summary>
/// <param name="left">- the collider of the owner of this response</param>
/// <param name="right">- the collider of the other object</param>
void OB_BreakCoconutOnCollision(Collider* left, Collider* right)
{
	if (strcmp(right->obj->tag, "Click") == 0)
	{
		if (strcmp(left->obj->tag, "obc_coconut") == 0)
		{
			if (obc_stage < obc_MAX_STAGE && obc_canHit)
			{
				obc_stage++;
				EnableImage(obc_stage);
				Animation* a = AM_GetComponent(obc_coconut);
				a->isEnabled = 1;
				obc_canHit = 0;
			}
		}
		else if (strcmp(left->obj->tag, "cross") == 0)
		{
			OB_BreakCoconutUnTrigger();
		}
	}
}
/// <summary>
/// Inits the break coconut objective
/// </summary>
void OB_BreakCoconutInit()
{
	float screenWidth, screenHeight;
	RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
	Renderer* r = NULL;
	Collider* c = NULL;

	obc_title = GOM_Create2(RECTANGLE, CP_Vector_Set(screenWidth * 0.5f, 75), 0, CP_Vector_Set(screenWidth * 0.6f, 60));
	r = RM_AddComponent(obc_title);
	r->color.a = 200;
	RM_SetText(r, "Break the coconut!");
	r->renderPriority = PRI_UI;
	r->textScale = CP_Vector_Set(3.0f, 3.0f);

	obc_UI = GOM_Create(RECTANGLE);
	r = RM_AddComponent(obc_UI);
	r->color = CP_Color_Create(200, 200, 200, 255);
	r->renderPriority = PRI_UI;
	obc_UI->scale = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.7f);
	obc_UI->position = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f);

	obc_cross = GOM_Create2(RECTANGLE, CP_Vector_Set(screenWidth * 0.775f, screenHeight * 0.225f), 0 , CP_Vector_Set(50, 50));
	obc_cross->tag = "cross";
	r = RM_AddComponent(obc_cross);
	RM_LoadImage(r, "Assets/cross.png");
	r->renderPriority = PRI_UI;
	c = CLM_AddComponent(obc_cross);
	CLM_Set(c, COL_BOX, OB_BreakCoconutOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	obc_coconut = GOM_Create2(CIRCLE, CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f),
		0, CP_Vector_Set(screenHeight * 0.5f, screenHeight * 0.5f));
	obc_coconut->tag = "obc_coconut";
	r1 = RM_AddComponent(obc_coconut);
	r1->renderPriority = PRI_UI;
	RM_LoadImage(r1, "Assets/coconut/coconut1.png");
	r2 = RM_AddComponent(obc_coconut);
	r2->renderPriority = PRI_UI;
	RM_LoadImage(r2, "Assets/coconut/coconut2.png");
	r3 = RM_AddComponent(obc_coconut);
	r3->renderPriority = PRI_UI;
	RM_LoadImage(r3, "Assets/coconut/coconut3.png");
	r4 = RM_AddComponent(obc_coconut);
	r4->renderPriority = PRI_UI;
	RM_LoadImage(r4, "Assets/coconut/coconut4.png");

	c = CLM_AddComponent(obc_coconut);
	CLM_Set(c, COL_BOX, OB_BreakCoconutOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	Animation* a = AM_AddComponent(obc_coconut);
	AM_SetShake(a, 5.0f, 0.35f, 1, 0);
	
	a->isEnabled = 0;
	obc_stage = 0;
	OB_BreakCoconutUnTrigger();
}
/// <summary>
/// Updates the break coconut objective every frame
/// </summary>
void OB_BreakCoconutUpdate()
{	
	static float et = 0.0f;
	if (!obc_canHit)
	{
		et += CP_System_GetDt();
		if (et > obc_delayHit)
		{
			obc_canHit = 1;
			et = 0.0f;
		}
	}

	if (OB_IsBreakCoconutComplete())
	{
		Renderer* r = RM_GetComponent(obc_title);
		RM_SetText(r, "Coconut broken!");
	}
}
/// <summary>
/// Triggers / activates the break coconut objective
/// </summary>
void OB_BreakCoconutTrigger()
{
	EnableImage(obc_stage);
	obc_UI->isEnabled = obc_title->isEnabled = 1;
	obc_coconut->isEnabled = 1;
	obc_cross->isEnabled = 1;
	isActive = 1;

	Renderer* r = RM_GetComponent(obc_title);
	RM_SetText(r, "Break the coconut!");
}
/// <summary>
/// Untriggers / de-activates the break coconut objective
/// </summary>
void OB_BreakCoconutUnTrigger()
{
	obc_stage = 0;
	EnableImage(obc_stage);

	//Animation* a = AM_GetComponent(obc_coconut);
	//AM_SetShake(a, 5.0f, 0.35f, 1, 0);
	//a->isEnabled = 0;
	obc_UI->isEnabled = obc_title->isEnabled = 0;
	obc_coconut->isEnabled = 0;
	obc_cross->isEnabled = 0;
	obc_canHit = 1;
	isActive = 0;
}
/// <summary>
/// returns true or false if the objective is complete
/// </summary>
/// <returns>int</returns>
int OB_IsBreakCoconutComplete()
{
	return obc_stage == obc_MAX_STAGE;
}
/// <summary>
/// returns true of false for objective if it is active or not
/// </summary>
/// <returns>int</returns>
int OB_IsBreakCoconutActive()
{
	return isActive;
}
/// <summary>
/// Sets active or not active for the objective
/// </summary>
/// <param name="iSetter">- 1 or 0 for active</param>
void OB_SetBreakCoconutActive(int iSetter)
{
	isActive = iSetter;
}
