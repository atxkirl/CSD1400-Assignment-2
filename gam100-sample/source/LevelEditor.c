#include "LevelEditor.h"
#include <stdlib.h>
#include <stdio.h>

// For sizing
#define NumGrids 30

#define isEmpty 0
// For obj
#define Wall 1

int iGameObjectsCycle;
int iSize;

typedef struct 
{
	int gGrid[NumGrids][NumGrids];
} Grid;

Grid gGrids;

void LevelEditorInit()
{
	iGameObjectsCycle = Wall; // initialize to 0;
	iSize = CP_System_GetWindowHeight() / NumGrids;

	// set all to empty first
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			gGrids.gGrid[i][j] = isEmpty;
		}
	}
}

void LevelEditorUpdate()
{
	RenderObjects();
	PlaceObject();
}

void LevelEditorExit()
{

}

void RenderObjects()
{
	/* This will fill the background with grey color */
	CP_Graphics_ClearBackground(CP_Color_Create(128, 128, 128, 255));

	// grids
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	for (int i = 0; i <= NumGrids; i++)
	{
		CP_Graphics_DrawLine(0, (float)i * iSize, (float) NumGrids * iSize, (float)i * iSize); // Draw horizontal line
	}

	for (int i = 0; i <= NumGrids; i++)
	{
		CP_Graphics_DrawLine((float)i * iSize, 0, (float)i * iSize, (float) NumGrids * iSize); // Draw Vertical line
	}

	//render obj
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			switch (gGrids.gGrid[i][j])
			{
			case(Wall):
				// Set the colour before rendering the circle.
				CP_Settings_Fill(CP_Color_Create(255, 255, 0, 225)); // r, g, b, a
				CP_Graphics_DrawRect((float)j * iSize, (float)i * iSize, (float)iSize, (float)iSize);
				break;
			default:
				break;
			}
		}
	}
}

void PlaceObject()
{
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		CheckGrid(CP_Input_GetMouseX(), CP_Input_GetMouseY(), iGameObjectsCycle);
	}
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_2))
	{
		CheckGrid(CP_Input_GetMouseX(), CP_Input_GetMouseY(), isEmpty);
	}
}

void CheckGrid(float fMouseX, float fMouseY, int iObjType)
{
	// find the difference to the nearest edge of the grid
	int iModPosX = (int)CP_Input_GetMouseX() % (int)iSize;
	int iModPosY = (int)CP_Input_GetMouseY() % (int)iSize;

	int iCurrentX = (int)(CP_Input_GetMouseX() - iModPosX) / iSize;
	int iCurrentY = (int)(CP_Input_GetMouseY() - iModPosY) / iSize;

	if (gGrids.gGrid[iCurrentY][iCurrentX] != iObjType)
	{
		gGrids.gGrid[iCurrentY][iCurrentX] = iObjType;
	}
}