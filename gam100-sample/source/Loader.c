#include <stdlib.h>
#include <stdio.h>
#include "..\CProcessing\inc\cprocessing.h"
#include "SystemManager.h"
#include "FileParser.h"
#include"Loader.h"
#include "Colors.h"
#include "LevelEditor.h"

#define UIHEIGHT 50.f
#define UIWIDTH 300.f

int iSize;
Grid gLoadedGrids;

void LoaderInit()
{
	iSize = CP_System_GetWindowHeight() / NumGrids;

	for (int i = 0; i < MAX_OBJECTIVES; i++)
	{
		oObjectiveList[i].isComplete = 0;
	}

	// set all to empty first
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			//gGrids.gGrid[i][j] = isEmpty;
			GameObject* go = GOM_Create(EMPTY);
			Renderer* r = RM_AddComponent(go);
			go->position = CP_Vector_Set((float)j , (float)i);
			go->scale = CP_Vector_Set(1.f, 1.f);
			r->color = CP_Color_Create(255, 255, 0, 255);
			gLoadedGrids.gGrid[i][j] = go;
		}
	}
}

void LoaderUpdate()
{
	LoaderRender();
}

void LoaderExit()
{
	SM_SystemsExit();
}

void LoaderRender()
{
	// grids
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	for (int i = 0; i <= NumGrids; i++)
	{
		CP_Graphics_DrawLine(0,
			(float)i * iSize,
			(float)NumGrids * iSize,
			(float)i * iSize); // Draw horizontal line

	}

	for (int i = 0; i <= NumGrids; i++)
	{
		CP_Graphics_DrawLine((float)i * iSize,
			0,
			(float)i * iSize,
			(float)NumGrids * iSize); // Draw Vertical line


	}

	//render obj
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			switch (gLoadedGrids.gGrid[i][j]->type)
			{
			case(WALL):
				CP_Settings_Fill(CP_Color_Create(255, 255, 0, 225)); // r, g, b, a
				CP_Graphics_DrawRect((float)j * iSize , (float)i * iSize, (float)iSize, (float)iSize);
				break;
			case(RECTANGLE):
				CP_Settings_Fill(CP_Color_Create(255, 128, 128, 225)); // r, g, b, a
				CP_Graphics_DrawRect((float)j * iSize, (float)i * iSize, (float)iSize, (float)iSize);
				break;
			default:
				break;
			}
		}
	}

	CP_Settings_Fill(COLOR_WHITE); // r, g, b, a
	CP_Graphics_DrawRect(50.f, 10.f, UIWIDTH, UIHEIGHT);


	CP_Settings_Fill(COLOR_BLACK); // r, g, b, a
	for (int i = 0; i < MAX_OBJECTIVES; i++)
	{
		if (!oObjectiveList[i].isComplete)
		{
			CP_Font_DrawText(oObjectiveList[i].cObjective, 115.f, 30.f + i * 20.f);
		}
		else
		{
			CP_Font_DrawText("DONE", 75.f, 30.f + i * 20.f);
		}
	}
}


/*!
@brief Loads the Grid based on the cInput which is the file name.

@param char* - File name
@return void
*/
void LoadGrid(char* cInput, int iLoad)
{
	char cFileLocation[100] = { "Levels/" };

	strcat_s(cFileLocation, 100, cInput);
	strcat_s(cFileLocation, 100, ".txt");

	Map* objList = new_Map();
	ReadLevelFromFile(cFileLocation, objList);

	float fWorldHeight = WORLD_HEIGHT;
	float fCellSize = fWorldHeight / NumGrids; //fit 30 grids vertically in the screen

	for (int i = 0; i < objList->iSize; i++)
	{
		int iY = objList->fObjList[i]->iPosY;
		int iX = objList->fObjList[i]->iPosX;
		gLoadedGrids.gGrid[iY][iX]->type = objList->fObjList[i]->iType;
		gLoadedGrids.gGrid[iY][iX]->position = CP_Vector_Set(iX * fCellSize + fCellSize * 0.5f, iY * fCellSize + fCellSize * 0.5f);
		gLoadedGrids.gGrid[iY][iX]->scale = CP_Vector_Set(fCellSize, fCellSize);
		Renderer* r = RM_GetComponent(gLoadedGrids.gGrid[iY][iX]);
		//gLoadedGrids.gGrid[iY][iX]->color = CP_Color_Create(255, 255, 255, 255);
		r->color = CP_Color_Create(255, 255, 255, 255);
	}
}

void LoadObjectives(char* cInput)
{
	char cFileLocation[100] = { "Objectives/" };

	strcat_s(cFileLocation, 100, cInput);
	strcat_s(cFileLocation, 100, ".txt");

	char** cObjectiveList;
	cObjectiveList = malloc(sizeof(char*) * MAX_OBJECTIVES);

	if (cObjectiveList)
	{
		for (int i = 0; i < MAX_OBJECTIVES; i++)
		{
			cObjectiveList[i] = malloc(sizeof(char) * 100);
			if (cObjectiveList[i])
				cObjectiveList[i][0] = '\0';
		}
	}

	ReadObjectivesFromFile(cFileLocation, cObjectiveList);

	//testing printing the objectives
	if (cObjectiveList)
	{
		for (int i = 0; i < MAX_OBJECTIVES; i++)
		{
			if (cObjectiveList[i])
			{
				if (cObjectiveList[i][0] != '\0')
				{
					strcpy_s(oObjectiveList[i].cObjective, 100, cObjectiveList[i]);
				}
			}
		}
	}
}

void SetObjectiveComplete(int iIndex, int iSetter)
{
	oObjectiveList[iIndex - 1].isComplete = iSetter;
}