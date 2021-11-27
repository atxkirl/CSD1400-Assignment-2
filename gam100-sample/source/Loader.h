#pragma once
typedef struct Grid Grid;
typedef union CP_Vector CP_Vector;

Grid* gLoadedGrids;
Grid *gLoadedObjects;
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

CP_Vector SetPlayerPosition();

CP_Vector SetEnemyPosition();

int GetNumObjectives();

int GetLoadedNumBoatParts();