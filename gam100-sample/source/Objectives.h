/*!
@file            Objectives.h
@author          Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course          CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief           This file contains the functions for all level objectives.
*//*______________________________________________________________________*/

#pragma once
#define MAX_OBJECTIVES 3

typedef struct Objectives
{
	char cObjective[100];
	int isComplete;
	int isSet;
}Objectives;

/// <summary>
/// Initialises the variables in this file.
/// </summary>
/// <param name="fScreenWidth"></param>
/// <param name="fScreenHeight"></param>
void Objectives_Init(float fScreenWidth, float fScreenHeight);

/// <summary>
/// Update.
/// </summary>
void Objectives_Update();

/// <summary>
/// Exit.
/// </summary>
void Objectives_Exit();

/// <summary>
/// Toggles the player update when doing objectives.
/// </summary>
/// <param name="iSetter"></param>
void Objectives_SetPlayerUpdate(int iSetter);

/// <summary>
/// Render the Objectives Taskbar UI
/// </summary>
void Objectives_RenderUI();

/// <summary>
/// Returns the variable that handles the toggle of player update.
/// </summary>
/// <returns>int</returns>
int Objectives_GetPlayerUpdate();

/// <summary>
/// Checks if player has completed all the objectives of the level.
/// </summary>
/// <returns>int</returns>
int Objectives_GetCompleteAll();

Objectives oObjectiveList[MAX_OBJECTIVES];