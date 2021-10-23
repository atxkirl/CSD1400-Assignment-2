#pragma once


//prefix: MS

#include "LinkedList.h"
#include "../CProcessing/inc/cprocessing.h"

LinkedList* MS_InitIdentity();
CP_Matrix* MS_MatCopy(CP_Matrix);
CP_Matrix* MS_MatCopy2(CP_Matrix*);
void MS_MatCopy3(CP_Matrix*, CP_Matrix*);
void MS_MatCopy4(CP_Matrix*, CP_Matrix);
void MS_PushMatrix(LinkedList*);
LinkedList* MS_PopMatrix(LinkedList*);
CP_Matrix* MS_Top(LinkedList*);
LinkedList* MS_Clear(LinkedList*);
void MS_Rotate(LinkedList*, float);
void MS_Scale(LinkedList*, CP_Vector);
void MS_Translate(LinkedList*, CP_Vector);
void MS_Print(CP_Matrix*);