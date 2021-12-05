/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Controls.h
@author     Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course     CSD1400 Software Engineering Project 1
@Team       BananaBoi
@brief      This file contains the functions handling the player
			controls.
*//*________________________________________________________________________________________*/

#pragma once

typedef struct Controls
{
	char cUp;
	char cDown;
	char cLeft;
	char cRight;
	char cInteract;

} Controls;

Controls* cControls;

/// <summary>
/// Initialises the controls based on .txt
/// </summary>
void Init_Controls();

/// <summary>
/// Exit
/// </summary>
void Exit_Controls();