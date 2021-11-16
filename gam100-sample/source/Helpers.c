#include "Helpers.h"
#include "RenderManager.h"
#include "LevelEditor.h"

float GetTileScale()
{
	float fWorldHeight = WORLD_HEIGHT;
	float fScale = fWorldHeight / NumGrids * 4.f; //fit 30 grids vertically in the screen

	return fScale;
}