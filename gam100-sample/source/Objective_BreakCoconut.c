#include "Objective_BreakCoconut.h"
#include "SystemManager.h"


int stage = 0;
const int MAX_STAGE = 3;

GameObject* cross;
GameObject* coconut;
Renderer* r1, *r2, *r3, *r4;

void EnableImage(int _stage)
{
	r1->isEnabled = stage == 0;
	r2->isEnabled = stage == 1;
	r3->isEnabled = stage == 2;
	r4->isEnabled = stage == 3;
}
void OB_BreakCoconutOnCollision(Collider* left, Collider* right)
{
	if (strcmp(right->obj->tag, "Click") == 0)
	{
		if (strcmp(left->obj->tag, "coconut") == 0)
		{
			if (stage < MAX_STAGE)
			{
				stage++;
				EnableImage(stage);
				Animation* a = AM_GetComponent(coconut);
				a->isEnabled = 1;
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

	cross = GOM_Create2(RECTANGLE, CP_Vector_Set(screenWidth * 0.775f, screenHeight * 0.225f), 0 , CP_Vector_Set(50, 50));
	cross->tag = "cross";
	r = RM_AddComponent(cross);
	RM_LoadImage(r, "Assets/cross.png");
	r->renderPriority = PRI_UI;
	c = CLM_AddComponent(cross);
	CLM_Set(c, COL_BOX, OB_BreakCoconutOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	coconut = GOM_Create2(CIRCLE, CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f),
		0, CP_Vector_Set(screenHeight * 0.5f, screenHeight * 0.5f));
	coconut->tag = "coconut";
	r1 = RM_AddComponent(coconut);
	r1->renderPriority = PRI_UI;
	RM_LoadImage(r1, "Assets/coconut/coconut1.png");
	r2 = RM_AddComponent(coconut);
	r2->renderPriority = PRI_UI;
	RM_LoadImage(r2, "Assets/coconut/coconut2.png");
	r3 = RM_AddComponent(coconut);
	r3->renderPriority = PRI_UI;
	RM_LoadImage(r3, "Assets/coconut/coconut3.png");
	r4 = RM_AddComponent(coconut);
	r4->renderPriority = PRI_UI;
	RM_LoadImage(r4, "Assets/coconut/coconut4.png");

	c = CLM_AddComponent(coconut);
	CLM_Set(c, COL_BOX, OB_BreakCoconutOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	Animation* a = AM_AddComponent(coconut);
	AM_SetShake(a, 5.0f, 0.35f, 1, 0);
	
	a->isEnabled = 0;
	r2->isEnabled = r3->isEnabled = r4->isEnabled = 0;
	coconut->isEnabled = 0;
	cross->isEnabled = 0;
	stage = 0;
}
void OB_BreakCoconutUpdate()
{

}
void OB_BreakCoconutTrigger()
{
	EnableImage(stage);
	coconut->isEnabled = 1;
	cross->isEnabled = 1;
}
void OB_BreakCoconutUnTrigger()
{
	coconut->isEnabled = 0;
	cross->isEnabled = 0;
}

int OB_IsBreakCoconutComplete()
{
	return stage == MAX_STAGE;
}
