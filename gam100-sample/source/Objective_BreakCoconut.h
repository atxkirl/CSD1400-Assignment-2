/*!
@file		Objective_BreakCoconut.h
@author		Ow Hong Yu (ow.h)
@course		CSD 1400
@section	A
@brief		One of the objectives. It is a click on the coconut a few times
			until it is broken for objective complete
*/


#pragma once

/// <summary>
/// Inits the break coconut objective
/// </summary>
void OB_BreakCoconutInit();
/// <summary>
/// Updates the break coconut objective every frame
/// </summary>
void OB_BreakCoconutUpdate();
/// <summary>
/// Triggers / activates the break coconut objective
/// </summary>
void OB_BreakCoconutTrigger();
/// <summary>
/// Untriggers / de-activates the break coconut objective
/// </summary>
void OB_BreakCoconutUnTrigger();


/// <summary>
/// returns true or false if the objective is complete
/// </summary>
/// <returns>int</returns>
int OB_IsBreakCoconutComplete();
/// <summary>
/// returns true of false for objective if it is active or not
/// </summary>
/// <returns>int</returns>
int OB_IsBreakCoconutActive();
/// <summary>
/// Sets active or not active for the objective
/// </summary>
/// <param name="iSetter">- 1 or 0 for active</param>
void OB_SetBreakCoconutActive(int iSetter);