#include "MatrixStack.h"
#include <stdio.h> //for printf debug
#include <stdlib.h>

LinkedList* MS_InitIdentity()
{
	CP_Matrix* identity = MS_MatCopy(CP_Matrix_Identity());
	LinkedList* ms = NULL;
	ms = LL_Add(ms, (void*)(identity));
	return ms;
}

CP_Matrix* MS_MatCopy(CP_Matrix mat)
{
	return MS_MatCopy2(&mat);
}
CP_Matrix* MS_MatCopy2(CP_Matrix* mat)
{
	CP_Matrix* ret = malloc(sizeof(CP_Matrix));
	if (!ret)
		return NULL;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			ret->m[i][j] = mat->m[i][j];
	return ret;
}

void MS_MatCopy3(CP_Matrix* l, CP_Matrix* r)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			l->m[i][j] = r->m[i][j];
}

void MS_MatCopy4(CP_Matrix* l, CP_Matrix r)
{
	MS_MatCopy3(l, &r);
}

void MS_PushMatrix(LinkedList* ms)
{
	ms = LL_Add(ms, (void*)(MS_MatCopy2(MS_Top(ms))));
}

LinkedList* MS_PopMatrix(LinkedList* ms)
{
	LinkedList* last = LL_GetLast(ms);
	CP_Matrix* lmat = (CP_Matrix*)last->curr;
	free(lmat);
	ms = LL_RemoveLL(ms, last);
	return ms;
}

CP_Matrix* MS_Top(LinkedList* ms)
{
	return (CP_Matrix*)(LL_GetLast(ms)->curr);
}

LinkedList* MS_Clear(LinkedList* ms)
{
	while (LL_GetCount(ms))
		ms = MS_PopMatrix(ms);
	return NULL;
}

void MS_Rotate(LinkedList* ms, float deg)
{
	CP_Matrix mat = CP_Matrix_Rotate(deg);
	CP_Matrix* top = MS_Top(ms);
	MS_MatCopy4(top, CP_Matrix_Multiply(*top, mat));
}

void MS_Scale(LinkedList* ms, CP_Vector scale)
{
	CP_Matrix mat = CP_Matrix_Scale(scale);
	CP_Matrix* top = MS_Top(ms);
	MS_MatCopy4(top, CP_Matrix_Multiply(*top, mat));
}

void MS_Translate(LinkedList* ms, CP_Vector translate)
{
	CP_Matrix mat = CP_Matrix_Translate(translate);
	CP_Matrix* top = MS_Top(ms);
	MS_MatCopy4(top, CP_Matrix_Multiply(*top, mat));
}

void MS_Print(CP_Matrix* mat)
{
	printf("%6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f\n"
		, mat->m00, mat->m01, mat->m02
		, mat->m10, mat->m11, mat->m12
		, mat->m20, mat->m21, mat->m22);
}
