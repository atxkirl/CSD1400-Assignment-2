#include "../CProcessing/inc/cprocessing.h"
#include "GameObject.h"
// For sizing
#define NumGrids 30

enum GridNodeState
{
	NotVisited = 0,
	Visited,
};

typedef struct GridNode
{
	int Curr;
	int Next;
	int Prev;
} GridNode;

typedef struct Grid
{
	GameObject* gGrid[NumGrids][NumGrids];
	GridNode nGrid[NumGrids][NumGrids];
} Grid;

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

void AutoGenerateGrid();
