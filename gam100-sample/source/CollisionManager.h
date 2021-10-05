#pragma once

//PREFIX: CLM
#include "../CProcessing/inc/cprocessing.h"
#include "LinkedList.h"

typedef struct
{
	CP_Vector velocity;
	int isEnabled;
	void (*OnCollision)(Collider);

} Collider;

LinkedList* objects = NULL;

void CLM_Init();
void CLM_Add(Collider* );
int CLM_Remove(Collider*);
void CLM_Clear();
int CLM_GetIndex(Collider*);