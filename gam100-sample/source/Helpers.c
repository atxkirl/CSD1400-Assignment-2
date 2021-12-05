/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file		Helpers.c
@author		Adrian Tan (t.xingkhiangadrian)
@coauthor	Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team		BananaBoi
@date		06/10/2021
@brief		Contains helpful functions to calculate various things, like random
			values, or the sizes of arrays.
*//*________________________________________________________________________________________*/

#include "Helpers.h"
#include "RenderManager.h"
#include "LevelEditor.h"

float GetTileScale()
{
	float fWorldHeight = WORLD_HEIGHT;
	float fScale = fWorldHeight / NumGrids * 4.f; //fit 30 grids vertically in the screen

	printf("Tile Scale %.2f\n", fScale);
	return fScale;
}