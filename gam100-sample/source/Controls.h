/*!
@file            Controls.h
@author          Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course          CSD 1400
@section         C
@brief           This file contains the functions handling the player
				 controls.
*//*______________________________________________________________________*/

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