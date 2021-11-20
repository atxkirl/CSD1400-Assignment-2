#include <stdio.h>
#include <stdlib.h>
#include"Loader.h"
#include "..\CProcessing\inc\cprocessing.h"
#include "SystemManager.h"
#include "FileParser.h"
#include "Colors.h"
#include "LevelEditor.h"
#include "Objectives.h"

#define UIHEIGHT 50.f
#define UIWIDTH 300.f

int iSize;
float fPlayerPositionX;
float fPlayerPositionY;
int iNumObjectives;

void LoaderInit()
{
	iSize = CP_System_GetWindowHeight() / NumGrids;
	iNumObjectives = 0;

	for (int i = 0; i < MAX_OBJECTIVES; i++)
	{
		oObjectiveList[i].isComplete = 0;
		oObjectiveList[i].isSet = 0;
	}

	gLoadedGrids = malloc(sizeof(Grid));

	if (gLoadedGrids)
	{// set all to empty first
		for (int i = 0; i < NumGrids; i++)
		{
			for (int j = 0; j < NumGrids; j++)
			{
				//gGrids.gGrid[i][j] = isEmpty;
				GameObject* go = GOM_Create(EMPTY);
				Renderer* r = RM_AddComponent(go);
				go->position = CP_Vector_Set((float)j, (float)i);
				go->scale = CP_Vector_Set(1.f, 1.f);
				r->color = CP_Color_Create(255, 255, 0, 255);
				gLoadedGrids->gGrid[i][j] = go;
			}
		}
	}

	gLoadedObjects = malloc(sizeof(Grid));
	if (gLoadedObjects)
	{// set all to empty first
		for (int i = 0; i < NumGrids; i++)
		{
			for (int j = 0; j < NumGrids; j++)
			{
				//gGrids.gGrid[i][j] = isEmpty;
				GameObject* go = GOM_Create(EMPTY);
				Renderer* r = RM_AddComponent(go);
				go->position = CP_Vector_Set((float)j, (float)i);
				go->scale = CP_Vector_Set(1.f, 1.f);
				r->color = CP_Color_Create(255, 255, 0, 255);
				gLoadedObjects->gGrid[i][j] = go;
			}
		}
	}

	fPlayerPositionX = 0.f;
	fPlayerPositionY = 0.f;
}

void LoaderUpdate()
{

}

void LoaderExit()
{
	SM_SystemsExit();

	free(gLoadedGrids);
	free(gLoadedObjects);
}
/*!
@brief Loads the Grid based on the cInput which is the file name.

@param char* - File name
@return void
*/
void LoadGrid(char* cInput, int iLoad)
{
	char cLevelFileLocation[100] = { "Levels/" };
	char cObjectFileLocation[100] = { "Objects/" };

	strcat_s(cLevelFileLocation, 100, cInput);
	strcat_s(cLevelFileLocation, 100, ".txt");
	strcat_s(cObjectFileLocation, 100, cInput);
	strcat_s(cObjectFileLocation, 100, ".txt");

	float fWorldHeight = WORLD_HEIGHT;
	float fScale = fWorldHeight / NumGrids * 4.f; //fit 30 grids vertically in the screen

	Renderer* r;
	Map* objList = new_Map();
	ReadLevelFromFile(cLevelFileLocation, objList);
	for (int i = 0; i < objList->iSize; i++)
	{
		if (objList->fObjList[i])
		{
			int iY = objList->fObjList[i]->iPosY;
			int iX = objList->fObjList[i]->iPosX;
			gLoadedGrids->gGrid[iY][iX]->type = objList->fObjList[i]->iType;
			gLoadedGrids->gGrid[iY][iX]->position = CP_Vector_Set(iX * fScale - fScale, iY * fScale - fScale);
			gLoadedGrids->gGrid[iY][iX]->scale = CP_Vector_Set(fScale, fScale);
			gLoadedGrids->gGrid[iY][iX]->oDirection = objList->fObjList[i]->iDir;
			gLoadedGrids->gGrid[iY][iX]->rotation = gLoadedGrids->gGrid[iY][iX]->oDirection * 90.f;
			gLoadedGrids->gGrid[iY][iX]->tag = objList->fObjList[i]->cTag;

			if (strcmp(objList->fObjList[i]->cTag, "PlayerSpawn") == 0)
			{
				fPlayerPositionX = gLoadedGrids->gGrid[iY][iX]->position.x;
				fPlayerPositionY = gLoadedGrids->gGrid[iY][iX]->position.y;
				printf("Spawned\n");
			}

			r = RM_GetComponent(gLoadedGrids->gGrid[iY][iX]);
			//gLoadedGrids.gGrid[iY][iX]->color = CP_Color_Create(255, 255, 255, 255);
			r->color = CP_Color_Create(255, 255, 255, 255);

			switch (gLoadedGrids->gGrid[iY][iX]->type)
			{
			case(WALL):
				RM_LoadImage(r, "Assets/sand-tiles/sand-tile-1.png");
				break;
			case(CORNER):
				RM_LoadImage(r, "Assets/sand-tiles/sand-tile-5.png");
				break;
			case(FLOOR):
				RM_LoadImage(r, "Assets/sand-tiles/sand-tile-0.png");
				break;
			case(FLOOR_MIDDLE):
				RM_LoadImage(r, "Assets/sand-tiles/sand-tile-13.png");
				break;
			case(THREE_CORNER):
				RM_LoadImage(r, "Assets/sand-tiles/sand-tile-3c.png");
				break;
			case(WATER):
				CLM_AddComponent(gLoadedGrids->gGrid[iY][iX]);
				RM_LoadImage(r, "Assets/sand-tiles/sea-tile-1.png");
				break;
			default:
				break;
			}
		}
	}

	Map* objList2 = new_Map();
	ReadLevelFromFile(cObjectFileLocation, objList2);
	for (int i = 0; i < objList2->iSize; i++)
	{
		if (objList2->fObjList[i])
		{
			int iY = objList2->fObjList[i]->iPosY;
			int iX = objList2->fObjList[i]->iPosX;
			gLoadedObjects->gGrid[iY][iX]->type = objList2->fObjList[i]->iType;
			gLoadedObjects->gGrid[iY][iX]->position = CP_Vector_Set(iX * fScale - fScale, iY * fScale - fScale);
			gLoadedObjects->gGrid[iY][iX]->scale = CP_Vector_Set(fScale, fScale);
			gLoadedObjects->gGrid[iY][iX]->oDirection = objList2->fObjList[i]->iDir;
			gLoadedObjects->gGrid[iY][iX]->rotation = gLoadedObjects->gGrid[iY][iX]->oDirection * 90.f;
			gLoadedObjects->gGrid[iY][iX]->tag = objList2->fObjList[i]->cTag;

			r = RM_GetComponent(gLoadedObjects->gGrid[iY][iX]);
			//gLoadedObjects.gGrid[iY][iX]->color = CP_Color_Create(255, 255, 255, 255);
			r->color = CP_Color_Create(255, 255, 255, 255);

			switch (gLoadedObjects->gGrid[iY][iX]->type)
			{
			case(CORAL):
				RM_LoadImage(r, "Assets/redcoral.png");
				break;

			case(GRASS):
				RM_LoadImage(r, "Assets/tempgrass.png");
				break;
			default:
				break;
			}
		}
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

	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			if (strcmp(gLoadedGrids->gGrid[i][j]->tag, "Objective") == 0)
			{
				int iRandom = rand() % MAX_OBJECTIVES;
				snprintf(gLoadedGrids->gGrid[i][j]->tag, 30, "Objective%d", iRandom + 1);
				printf("%s\n", gLoadedGrids->gGrid[i][j]->tag);
				++iNumObjectives;
			}
		}
	}
}

void SetObjectiveComplete(int iIndex, int iSetter)
{
	oObjectiveList[iIndex - 1].isComplete = iSetter;
}

CP_Vector SetPlayerPosition()
{
	return CP_Vector_Set(fPlayerPositionX, fPlayerPositionY);
}

int GetNumObjectives()
{
	return iNumObjectives;
}