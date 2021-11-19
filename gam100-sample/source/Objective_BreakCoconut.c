#include "Objective_BreakCoconut.h"
#include "SystemManager.h"


int obc_stage = 0;
const int obc_MAX_STAGE = 3;
int obc_canHit = 1;
const float obc_delayHit = 0.5f;

GameObject* obc_cross;
GameObject* obc_coconut;
Renderer* r1, *r2, *r3, *r4;

void EnableImage(int _stage)
{
	r1->isEnabled = obc_stage == 0;
	r2->isEnabled = obc_stage == 1;
	r3->isEnabled = obc_stage == 2;
	r4->isEnabled = obc_stage == 3;
}
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
void OB_BreakCoconutInit()
{
	float screenWidth, screenHeight;
	RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
	Renderer* r = NULL;
	Collider* c = NULL;

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
	r2->isEnabled = r3->isEnabled = r4->isEnabled = 0;
	obc_coconut->isEnabled = 0;
	obc_cross->isEnabled = 0;
	obc_stage = 0;
	obc_canHit = 1;
}
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

}
void OB_BreakCoconutTrigger()
{
	EnableImage(obc_stage);
	obc_coconut->isEnabled = 1;
	obc_cross->isEnabled = 1;
}
void OB_BreakCoconutUnTrigger()
{
	obc_stage = 0;
	EnableImage(obc_stage);

	//Animation* a = AM_GetComponent(obc_coconut);
	//AM_SetShake(a, 5.0f, 0.35f, 1, 0);
	//a->isEnabled = 0;

	obc_coconut->isEnabled = 0;
	obc_cross->isEnabled = 0;
	obc_canHit = 1;
}

int OB_IsBreakCoconutComplete()
{
	return obc_stage == obc_MAX_STAGE;
}
