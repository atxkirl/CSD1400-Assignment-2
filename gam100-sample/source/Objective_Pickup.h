/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file		Objective_Pickup.h
@author		Koh Xin Yun (xinyun.k)
@co-author	Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course		CSD1400 Software Engineering Project 1
@Team		BananaBoi
@brief		This file contains the declarations for functions in Objective Pickup.
*//*________________________________________________________________________________________*/

#pragma once

/// <summary>
/// Initialises variables in this scene.
/// </summary>
void OB_PickupInit();

/// <summary>
/// Update.
/// </summary>
void OB_PickupUpdate();

/// <summary>
/// returns the number of boat parts
/// </summary>
/// <returns>int</returns>
int GetBoatParts();

/// <summary>
/// Sets the number of boat parts
/// </summary>
/// <param name="iSetter">- 1 or 0 for active</param>
void SetBoatParts(int iSetter);