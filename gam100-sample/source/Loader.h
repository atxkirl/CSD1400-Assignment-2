/*!
@file            Loader.h
@author          Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course          CSD 1400
@section         C
@brief           This file contains the functions for loading the level
				 and all its objectives.
*//*______________________________________________________________________*/

#pragma once
typedef struct Grid Grid;
typedef union CP_Vector CP_Vector;

Grid* gLoadedGrids;
Grid *gLoadedObjects;

/// <summary>
/// Initialise Loader Variables
/// </summary>
void LoaderInit();

/// <summary>
/// Update
/// </summary>
void LoaderUpdate();

/// <summary>
/// Exit
/// </summary>
void LoaderExit();

/// <summary>
/// Loads the Grid based on the cInput which is the file name.
/// </summary>
/// <param name="cInput"></param>
/// <param name="iLoad"></param>
void LoadGrid(char* cInput, int iLoad);

/// <summary>
/// Load Objectives from .txt based on cInput
/// </summary>
/// <param name="cInput"></param>
void LoadObjectives(char* cInput);

/// <summary>
/// Set the Objective at iIndex to state iSetter
/// </summary>
/// <param name="iIndex"></param>
/// <param name="iSetter"></param>
void SetObjectiveComplete(int iIndex, int iSetter);

/// <summary>
/// Returns the player position
/// </summary>
/// <returns>CP_Vector</returns>
CP_Vector GetPlayerPosition();

/// <summary>
/// Get the enemy position.
/// </summary>
/// <returns>CP_Vector</returns>
CP_Vector GetEnemyPosition();

/// <summary>
/// Get number of objectives loaded.
/// </summary>
/// <returns>int</returns>
int GetNumObjectives();

/// <summary>
/// Get number of boat parts loaded.
/// </summary>
/// <returns>int</returns>
int GetLoadedNumBoatParts();