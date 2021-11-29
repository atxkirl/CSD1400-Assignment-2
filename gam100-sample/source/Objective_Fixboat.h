/*!
@file		Objective_Fixboat.h
@author		Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief		One of the objectives. It is a fix the boat puzzle where
			player has to drag parts and fit into the right slot
*/

#pragma once

/// <summary>
/// Inits the fix boat objective
/// </summary>
void OB_FixBoatInit();
/// <summary>
/// Updates the fix boat objective every frame
/// </summary>
void OB_FixBoatUpdate();
/// <summary>
/// Triggers / activates the fix boat objective
/// </summary>
void OB_FixBoatTrigger();
/// <summary>
/// Untriggers / unactivates the fix boat objective
/// </summary>
void OB_FixBoatUnTrigger();

/// <summary>
/// returns true or false if the fix boat objective is complete
/// </summary>
/// <returns>int</returns>
int OB_IsFixBoatComplete();
/// <summary>
/// returns true of false for objective if it is active or not
/// </summary>
/// <returns>int</returns>
int OB_IsFixBoatActive();
/// <summary>
/// Sets active or not active for the fixboat objective
/// </summary>
/// <param name="iSetter">- 1 or 0 for active</param>
void OB_SetFixBoatActive(int iSetter);