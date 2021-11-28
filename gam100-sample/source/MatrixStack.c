/*!
@file		MatrixStack.c
@author		Ow Hong Yu (ow.h)
@course		CSD 1400
@section	A
@brief		Matrix stack mainly used by render manager
*/

#include "MatrixStack.h"
#include <stdio.h> //for printf debug
#include <stdlib.h>

/// <summary>
/// Inits a matrix stack with a matrix identity
/// </summary>
/// <returns>- head of the newly created stack</returns>
LinkedList* MS_InitIdentity()
{
	CP_Matrix* identity = MS_MatCopy(CP_Matrix_Identity());
	LinkedList* ms = NULL;
	LL_Add(&ms, (void*)(identity));
	return ms;
}
/// <summary>
/// Allocates memory for matrix variable
/// </summary>
/// <param name="mat">- matrix to be copied from</param>
/// <returns>- reference of the new matrix variable</returns>
CP_Matrix* MS_MatCopy(CP_Matrix mat)
{
	return MS_MatCopy2(&mat);
}
/// <summary>
/// Allocates memory for matrix variable
/// </summary>
/// <param name="mat">- matrix to be copied from</param>
/// <returns>- reference of the new matrix variable</returns>
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
/// <summary>
/// Copies right matrix to left matrix
/// </summary>
/// <param name="l">- destination matrix to be copied to</param>
/// <param name="r">- source matrix to be copied from</param>
void MS_MatCopy3(CP_Matrix* l, CP_Matrix* r)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			l->m[i][j] = r->m[i][j];
}
/// <summary>
/// Copies right matrix to left matrix
/// </summary>
/// <param name="l">- destination matrix to be copied to</param>
/// <param name="r">- source matrix to be copied from</param>
void MS_MatCopy4(CP_Matrix* l, CP_Matrix r)
{
	MS_MatCopy3(l, &r);
}
/// <summary>
/// Adds new matrix w same value to the top of the stack
/// </summary>
/// <param name="ms">- list to have new matrix pushed</param>
void MS_PushMatrix(LinkedList* ms)
{
	LL_Add(&ms, (void*)(MS_MatCopy2(MS_Top(ms))));
}
/// <summary>
/// Removes and delete top of the stack
/// </summary>
/// <param name="ms">- list to have matrix popped</param>
/// <returns>LinkedList* - pointer of the list</returns>
LinkedList* MS_PopMatrix(LinkedList* ms)
{
	CP_Matrix* lmat = (CP_Matrix*)LL_GetLast(ms);
	LL_RemovePtr(&ms, lmat);
	free(lmat);
	return ms;
}
/// <summary>
/// Returns reference of the top of the stack
/// </summary>
/// <param name="ms">- list to have matrix peeked</param>
/// <returns>CP_Matrix* - reference of the top of the stack</returns>
CP_Matrix* MS_Top(LinkedList* ms)
{
	return (CP_Matrix*)(LL_GetLast(ms));
}
/// <summary>
/// Clears the list and reassign the pointer to null
/// </summary>
/// <param name="ms">- list to be cleared</param>
/// <returns>LinkedList* - nullptr when cleared</returns>
LinkedList* MS_Clear(LinkedList* ms)
{
	while (LL_GetCount(ms))
		ms = MS_PopMatrix(ms);
	return NULL;
}
/// <summary>
/// Multiplies rotation to the top of the stack
/// </summary>
/// <param name="ms">- pointer to the matrix stack</param>
/// <param name="deg">- deg of rotation</param>
void MS_Rotate(LinkedList* ms, float deg)
{
	CP_Matrix mat = CP_Matrix_Rotate(deg);
	CP_Matrix* top = MS_Top(ms);
	MS_MatCopy4(top, CP_Matrix_Multiply(*top, mat));
}
/// <summary>
/// Multiplies scale to the top of the stack
/// </summary>
/// <param name="ms">- pointer to the matrix stack</param>
/// <param name="scale">- Vector2 of the scale</param>
void MS_Scale(LinkedList* ms, CP_Vector scale)
{
	CP_Matrix mat = CP_Matrix_Scale(scale);
	CP_Matrix* top = MS_Top(ms);
	MS_MatCopy4(top, CP_Matrix_Multiply(*top, mat));
}
/// <summary>
/// Multiplies(?) translate to the top of the stack
/// </summary>
/// <param name="ms">- pointer to the matrix stack</param>
/// <param name="translate">- Vector2 of the translation</param>
void MS_Translate(LinkedList* ms, CP_Vector translate)
{
	CP_Matrix mat = CP_Matrix_Translate(translate);
	CP_Matrix* top = MS_Top(ms);
	MS_MatCopy4(top, CP_Matrix_Multiply(*top, mat));
}

/// <summary>
/// prints the matrix on console
/// </summary>
/// <param name="mat">- matrix to be printed</param>
void MS_Print(CP_Matrix* mat)
{
	printf("%6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f\n%6.2f %6.2f %6.2f\n"
		, mat->m00, mat->m01, mat->m02
		, mat->m10, mat->m11, mat->m12
		, mat->m20, mat->m21, mat->m22);
}
