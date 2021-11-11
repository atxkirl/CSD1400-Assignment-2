#pragma once
#define MAX_OBJECTIVES 2

typedef struct Grid Grid;
Grid *gLoadedGrids;

typedef struct Objectives
{
	char cObjective[100];
	int isComplete;
	int isSet;
}Objectives;

Objectives oObjectiveList[MAX_OBJECTIVES];
/*!
@brief Initialises the variables
@param void
@return void
*/
void LoaderInit();

/*!
@brief Update
@param void
@return void
*/
void LoaderUpdate();

/*!
@brief Exit
@param void
@return void
*/
void LoaderExit();

/*!
@brief Loads the Grid based on the cInput which is the file name.

@param char* - File name
@return void
*/
void LoadGrid(char* cInput, int iLoad);

void LoadObjectives(char* cInput);

void SetObjectiveComplete(int iIndex, int iSetter);

void SetPlayerPosition(float fPlayerPosX, float fPlayerPosY);