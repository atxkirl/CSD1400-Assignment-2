#pragma once
#define MAX_OBJECTIVES 3

typedef struct Objectives
{
	char cObjective[100];
	int isComplete;
	int isSet;
}Objectives;

void Objectives_Init(float fScreenWidth, float fScreenHeight);
void Objectives_Update();
void Objectives_Exit();

void Objectives_SetPlayerUpdate(int iSetter);
void Objectives_RenderUI();
int Objectives_GetPlayerUpdate();
int Objectives_GetCompleteAll();

Objectives oObjectiveList[MAX_OBJECTIVES];