#include "LevelEditor.h"
#include <stdlib.h>
#include <stdio.h>
#include "FileParser.h"
#include "SystemManager.h"

int iSize;
OBJECT_TYPE objType;

float fMoveX;
float fMoveY;
float fScaleBy;
CP_Vector vScale;
CP_Matrix mScale;

Grid gGrids;
int iAutoGenerate;

extern GameObject *GameObjectList;

/*!
@brief Initialises the variables
@param void
@return void
*/
void LevelEditorInit()
{
	objType = WALL; // initialize to 0;
	iSize = CP_System_GetWindowHeight() / NumGrids;

	fMoveX = 0.f;
	fMoveY = 0.f;
	fScaleBy = 1.f;
	vScale = CP_Vector_Set(fScaleBy, fScaleBy);
	mScale = CP_Matrix_Scale(vScale);

	// set all to empty first
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			//gGrids.gGrid[i][j] = isEmpty;
			GameObject* go = GOM_Create((i == 0 || i == NumGrids - 1 || j == 0 || j == NumGrids -1)? WALL : EMPTY);
			Renderer* r = RM_AddComponent(go);
			go->position = CP_Vector_Set(j * vScale.x, i * vScale.y);
			go->scale = CP_Vector_Set(vScale.x, vScale.y);
			r->color = CP_Color_Create(255, 255, 0, 255);
			gGrids.gGrid[i][j] = go;

			gGrids.nGrid[i][j].Curr = (enum GridNodeState) NotVisited;
			gGrids.nGrid[i][j].Next = (enum GridNodeState) NotVisited;
			gGrids.nGrid[i][j].Prev = (enum GridNodeState) NotVisited;
		}
	}

	iAutoGenerate = 0;
}

/*!
@brief Update
@param void
@return void
*/
void LevelEditorUpdate()
{
	if (CP_Input_KeyTriggered(KEY_ENTER))
	{
		SaveGrid();
	}

	// translate the grid
	if (CP_Input_KeyDown(KEY_W))
	{
		fMoveY += 50 * CP_System_GetDt();
	}

	else if (CP_Input_KeyDown(KEY_S))
	{
		fMoveY -= 50 * CP_System_GetDt();
	}

	if (CP_Input_KeyDown(KEY_A))
	{
		fMoveX += 50 * CP_System_GetDt();
	}

	else if (CP_Input_KeyDown(KEY_D))
	{
		fMoveX -= 50 * CP_System_GetDt();
	}

	if (CP_Input_KeyDown(KEY_1))
	{
		fScaleBy += 1 * CP_System_GetDt();
		vScale = CP_Vector_Set(fScaleBy, fScaleBy);
		mScale = CP_Matrix_Scale(vScale);
	}

	else if (CP_Input_KeyDown(KEY_2))
	{
		fScaleBy -= 1 * CP_System_GetDt();
		vScale = CP_Vector_Set(fScaleBy, fScaleBy);
		mScale = CP_Matrix_Scale(vScale);
	}

	if (CP_Input_KeyTriggered(KEY_Q))
	{
		objType++;
		if (objType >= END)
			objType = END - 1;

		printf("Object Type: %d\n", objType);
	}
	else if (CP_Input_KeyTriggered(KEY_E))
	{
		objType--;

		if (objType < 0)
			objType = 0;

		printf("Object Type: %d\n", objType);
	}

	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		iAutoGenerate = !iAutoGenerate;
	}

	PlaceObject();
	RenderObjects();

	//RM_Render();
}

/*!
@brief Exit
@param void
@return void
*/
void LevelEditorExit()
{
	SM_SystemsExit();

	//free(gGrids.gGrid);
}

/*!
@brief Renders the grid and objects.
@param void
@return void
*/
void RenderObjects()
{
	CP_Settings_ApplyMatrix(mScale);

	/* This will fill the background with grey color */
	CP_Graphics_ClearBackground(CP_Color_Create(128, 128, 128, 255));

	// grids
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	for (int i = 0; i <= NumGrids; i++)
	{
		CP_Graphics_DrawLine(0 + fMoveX,
			(float)i * iSize + fMoveY,
			(float)NumGrids * iSize + fMoveX,
			(float)i * iSize + fMoveY); // Draw horizontal line

	}

	for (int i = 0; i <= NumGrids; i++)
	{
		CP_Graphics_DrawLine((float)i * iSize + fMoveX,
			0 + fMoveY,
			(float)i * iSize + fMoveX,
			(float)NumGrids * iSize + fMoveY); // Draw Vertical line


	}

	//render obj
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			switch (gGrids.gGrid[i][j]->type)
			{
			case(WALL):
				CP_Settings_Fill(CP_Color_Create(255, 255, 0, 225)); // r, g, b, a
				CP_Graphics_DrawRect((float)j * iSize + fMoveX, (float)i * iSize + fMoveY, (float)iSize, (float)iSize);
				break;
			case(RECTANGLE):
				CP_Settings_Fill(CP_Color_Create(255, 128, 128, 225)); // r, g, b, a
				CP_Graphics_DrawRect((float)j * iSize + fMoveX, (float)i * iSize + fMoveY, (float)iSize, (float)iSize);
				break;
			case(CIRCLE):
				CP_Settings_Fill(CP_Color_Create(128, 128, 255, 225)); // r, g, b, a
				CP_Graphics_DrawRect((float)j * iSize + fMoveX, (float)i * iSize + fMoveY, (float)iSize, (float)iSize);
				break;
			default:
				break;
			}
		}
	}
}

/*!
@brief Place the object in the grid based on the mouse positions.
@param void
@return void
*/
void PlaceObject()
{
	// Global = (A)Local
	// Local = Global / A
	// In this case CP_InputGetMouse is my global coordinates, so to get the local coordinates need to divide by the scale.
	// Subtracting the fMoveX resets the position back to global.

	if (!iAutoGenerate)
	{
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
		{
			CheckGrid(CP_Input_GetMouseX() / fScaleBy - fMoveX, CP_Input_GetMouseY() / fScaleBy - fMoveY, objType);
		}
		if (CP_Input_MouseTriggered(MOUSE_BUTTON_2))
		{
			CheckGrid((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, EMPTY);
		}
	}
	else
	{
		AutoGenerateGrid();
	}
}

/*!
@brief Check if the spot on the grid is taken or not. 
If it is, it will check if the tile is not the same type first before allocating.

@param void
@return void
*/
void CheckGrid(float fMouseX, float fMouseY, int iObjType)
{
	// find the difference to the nearest edge of the grid
	int iModPosX = (int)fMouseX % (int)iSize;
	int iModPosY = (int)fMouseY % (int)iSize;

	int iCurrentX = (int)(fMouseX - iModPosX) / iSize;
	int iCurrentY = (int)(fMouseY - iModPosY) / iSize;

	if (gGrids.gGrid[iCurrentY][iCurrentX]->type != iObjType)
	{
		gGrids.gGrid[iCurrentY][iCurrentX]->type = iObjType;
	}
}

/*!
@brief Save changes made to the grid.

@param void
@return void
*/
void SaveGrid()
{
	char GridObj[900] = { "" };
	//GridObj = (char*)malloc(900 * sizeof(char));
	//GridObj = "";

	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			if (gGrids.gGrid[i][j]->type !=  EMPTY)
			{
				char ObjType[10];
				char ObjPosX[10];
				char ObjPosY[10];

				sprintf_s(ObjType, 10, "%d", gGrids.gGrid[i][j]->type);
				strcat_s(GridObj, 900, ObjType); //type
				strcat_s(GridObj, 900, ",");

				sprintf_s(ObjPosX, 10, "%d", j);
				strcat_s(GridObj, 900, ObjPosX); // x
				strcat_s(GridObj, 900, ",");

				sprintf_s(ObjPosY, 10, "%d", i);
				strcat_s(GridObj, 900, ObjPosY); // y
				strcat_s(GridObj, 900, "\n");
			}
		}
	}
	printf("%s\n", GridObj);

	if (GridObj != NULL)
	{
		char cFileName[50];
		char cFileLocation[100] = { "Levels/" };

		printf("Input a file name: ");
		scanf_s("%s", &cFileName, 50);
		strcat_s(cFileName, 50, ".txt");

		strcat_s(cFileLocation, 100, cFileName);
		printf("%s \n", cFileLocation);
		WriteToFile(cFileLocation, GridObj);
	}
}

void AutoGenerateGrid()
{
	for (int i = 1; i < NumGrids - 1; i++)
	{
		for (int j = 1; j < NumGrids - 1; j++)
		{
			int iObjType = EMPTY;
			int iPercentage = rand() % 10;

			if (iPercentage > 6 && iPercentage < 9)
			{
				iObjType = WALL;
			}

			else if (iPercentage == 9)
			{
				iObjType = (rand() % 2) + 1;
			}

			gGrids.gGrid[j][i]->type = iObjType;
		}
	}

	iAutoGenerate = 0;
}
