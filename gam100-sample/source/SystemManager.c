#include "SystemManager.h"


void SM_SystemsInit()
{
	GOM_Init();
	RM_Init();
	CLM_Init();
}

void SM_SystemsPreUpdate()
{
}

void SM_SystemsUpdate()
{
	CLM_Update();
}

void SM_SystemsLateUpdate()
{
	GOM_ClearTempObjects();
	RM_Render();
}

void SM_SystemsExit()
{
	CLM_Clear();
	RM_Clear();
	GOM_Clear();
}
