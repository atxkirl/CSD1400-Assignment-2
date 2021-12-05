/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file		MatrixStack.h
@author		Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team       BananaBoi
@brief		Matrix stack mainly used by render manager
*//*________________________________________________________________________________________*/

#pragma once


//prefix: MS

#include "LinkedList.h"
#include "../CProcessing/inc/cprocessing.h"

/// <summary>
/// Inits a matrix stack with a matrix identity
/// </summary>
/// <returns>- head of the newly created stack</returns>
LinkedList* MS_InitIdentity();
/// <summary>
/// Allocates memory for matrix variable
/// </summary>
/// <param name="mat">- matrix to be copied from</param>
/// <returns>- reference of the new matrix variable</returns>
CP_Matrix* MS_MatCopy(CP_Matrix);
/// <summary>
/// Allocates memory for matrix variable
/// </summary>
/// <param name="mat">- matrix to be copied from</param>
/// <returns>- reference of the new matrix variable</returns>
CP_Matrix* MS_MatCopy2(CP_Matrix*);
/// <summary>
/// Copies right matrix to left matrix
/// </summary>
/// <param name="l">- destination matrix to be copied to</param>
/// <param name="r">- source matrix to be copied from</param>
void MS_MatCopy3(CP_Matrix*, CP_Matrix*);
/// <summary>
/// Copies right matrix to left matrix
/// </summary>
/// <param name="l">- destination matrix to be copied to</param>
/// <param name="r">- source matrix to be copied from</param>
void MS_MatCopy4(CP_Matrix*, CP_Matrix);
/// <summary>
/// Adds new matrix w same value to the top of the stack
/// </summary>
/// <param name="ms">- list to have new matrix pushed</param>
void MS_PushMatrix(LinkedList*);
/// <summary>
/// Removes and delete top of the stack
/// </summary>
/// <param name="ms">- list to have matrix popped</param>
/// <returns>LinkedList* - pointer of the list</returns>
LinkedList* MS_PopMatrix(LinkedList*);
/// <summary>
/// Returns reference of the top of the stack
/// </summary>
/// <param name="ms">- list to have matrix peeked</param>
/// <returns>CP_Matrix* - reference of the top of the stack</returns>
CP_Matrix* MS_Top(LinkedList*);
/// <summary>
/// Clears the list and reassign the pointer to null
/// </summary>
/// <param name="ms">- list to be cleared</param>
/// <returns>LinkedList* - nullptr when cleared</returns>
LinkedList* MS_Clear(LinkedList*);
/// <summary>
/// Multiplies rotation to the top of the stack
/// </summary>
/// <param name="ms">- pointer to the matrix stack</param>
/// <param name="deg">- deg of rotation</param>
void MS_Rotate(LinkedList*, float);
/// <summary>
/// Multiplies scale to the top of the stack
/// </summary>
/// <param name="ms">- pointer to the matrix stack</param>
/// <param name="scale">- Vector2 of the scale</param>
void MS_Scale(LinkedList*, CP_Vector);
/// <summary>
/// Multiplies(?) translate to the top of the stack
/// </summary>
/// <param name="ms">- pointer to the matrix stack</param>
/// <param name="translate">- Vector2 of the translation</param>
void MS_Translate(LinkedList*, CP_Vector);
/// <summary>
/// prints the matrix on console
/// </summary>
/// <param name="mat">- matrix to be printed</param>
void MS_Print(CP_Matrix*);