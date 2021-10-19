#include "../CProcessing/inc/cprocessing.h"

/*!
@brief Initialises the variables
@param void
@return void
*/
void LevelEditorInit();

/*!
@brief Update
@param void
@return void
*/
void LevelEditorUpdate();

/*!
@brief Exit
@param void
@return void
*/
void LevelEditorExit();

void PlaceObject();

/*!
@brief Place the object in the grid based on the mouse positions.
@param void
@return void
*/
void CheckGrid(float fMouseX, float fMouseY, int iObjType);

/*!
@brief Check if the spot on the grid is taken or not.
If it is, it will check if the tile is not the same type first before allocating.

@param void
@return void
*/
void RenderObjects();

/*!
@brief Save changes made to the grid.

@param void
@return void
*/
void SaveGrid();

/*!
@brief Loads the Grid based on the cInput which is the file name.

@param char* - File name
@return void
*/
void LoadGrid(char* cInput);