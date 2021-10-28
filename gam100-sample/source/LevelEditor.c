#include "LevelEditor.h"
#include <stdlib.h>
#include <stdio.h>
#include "FileParser.h"
#include "SystemManager.h"
#include "LinkedList.h"

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

			gGrids.nGrid[i][j].Curr = go->type == WALL ? Visited : NotVisited;
			gGrids.nGrid[i][j].Up = go->type == WALL ? Visited : NotVisited;
			gGrids.nGrid[i][j].Down = go->type == WALL ? Visited : NotVisited;
			gGrids.nGrid[i][j].Left = go->type == WALL ? Visited : NotVisited;
			gGrids.nGrid[i][j].Right = go->type == WALL ? Visited : NotVisited;
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
		if (CP_Input_GetMouseX() < NumGrids * iSize && CP_Input_GetMouseY() < NumGrids * iSize)
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
	//450, 30
	char **GridObj = malloc(sizeof(char*) * 450);

	for (int i = 0; i < 450; i++)
	{
		if (GridObj)
		{
			GridObj[i] = malloc(sizeof(char) * 30);

			if (GridObj[i])
			{
				GridObj[i][0] = '\0';
			}
		}
	}

	//GridObj = (char*)malloc(900 * sizeof(char));
	//GridObj = "";
	int iObjNum = 0;
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			if (gGrids.gGrid[i][j]->type !=  EMPTY && iObjNum < 450)
			{
				char ObjType[10];
				char ObjPosX[10];
				char ObjPosY[10];
				if (GridObj)
				{
					if (GridObj[iObjNum])
					{
						sprintf_s(ObjType, 10, "%d", gGrids.gGrid[i][j]->type);
						strcpy_s(GridObj[iObjNum], 30, ObjType); //type
						strcat_s(GridObj[iObjNum], 30, ",");

						sprintf_s(ObjPosX, 10, "%d", j);
						strcat_s(GridObj[iObjNum], 30, ObjPosX); // x
						strcat_s(GridObj[iObjNum], 30, ",");

						sprintf_s(ObjPosY, 10, "%d", i);
						strcat_s(GridObj[iObjNum], 30, ObjPosY); // y
						strcat_s(GridObj[iObjNum], 30, "\n");
						printf("%s\n", GridObj[iObjNum]);

						iObjNum++;
					}
				}
			}
		}
	}

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
	//for (int i = 1; i < NumGrids - 1; i++)
	//{
	//	for (int j = 1; j < NumGrids - 1; j++)
	//	{
	//		int iObjType = EMPTY;
	//		int iPercentage = rand() % 10;

	//		if (iPercentage > 6 && iPercentage < 9)
	//		{
	//			iObjType = WALL;
	//		}

	//		else if (iPercentage == 9)
	//		{
	//			iObjType = (rand() % 2) + 1;
	//		}

	//		gGrids.gGrid[j][i]->type = iObjType;
	//	}
	//}
	for (int i = 1; i < NumGrids - 1; i++)
	{
		for (int j = 1; j < NumGrids - 1; j++)
		{
			gGrids.nGrid[j][i].Curr = NotVisited;
			gGrids.gGrid[j][i]->type = WALL;
		}
	}

	int positionX = 1 + rand() % (NumGrids - 2), positionY = 1 + rand() % (NumGrids - 2);

	LinkedList* l = NULL;

	LL_Add(&l, gGrids.gGrid[positionY][positionX]);
	if (positionY - 2 > 1)
	{
		if (gGrids.gGrid[positionY - 2][positionX]->type == WALL)
			LL_Add(&l, gGrids.gGrid[positionY - 2][positionX]); // up
	}

	if (positionY < NumGrids - 3)
	{
		if (gGrids.gGrid[positionY + 2][positionX]->type == WALL)
			LL_Add(&l, gGrids.gGrid[positionY + 2][positionX]); // down
	}

	if (positionX - 2 > 1)
	{
		if (gGrids.gGrid[positionY][positionX - 2]->type == WALL)
			LL_Add(&l, gGrids.gGrid[positionY][positionX - 2]); // left
	}

	if (positionX < NumGrids - 3)
	{
		if (gGrids.gGrid[positionY][positionX + 2]->type == WALL)
			LL_Add(&l, gGrids.gGrid[positionY][positionX + 2]); // right
	}

	GameObject* tempGO;
	tempGO = (GameObject*)l->curr;

	while (LL_GetCount(GetHead(l)) != 0)
	{
		if (tempGO->type == WALL)
		{
			tempGO->type = EMPTY;

			int iNext = rand() % LL_GetCount(GetHead(l));

			void* tempNode = LL_Get(l, iNext);
			LL_RemovePtr(&l, tempNode);
			l = GetHead(l);
			l->curr = tempNode;

			tempGO = (GameObject*)l->curr;
			int DifferenceX = positionX - (int)tempGO->position.x;
			int DifferenceY = positionY - (int)tempGO->position.y;

			gGrids.gGrid[positionY + DifferenceY][positionX + DifferenceX]->type = EMPTY;

			positionX = (int)tempGO->position.x;
			positionY = (int)tempGO->position.y;
		}
	}

	free(tempGO);
	/*while (1)
	{
		gGrids.nGrid[positionY][positionX].Up = gGrids.nGrid[positionY > 0 ? positionY - 1 : positionY][positionX].Curr;
		gGrids.nGrid[positionY][positionX].Down = gGrids.nGrid[positionY < NumGrids ? positionY + 1 : positionY][positionX].Curr;
		gGrids.nGrid[positionY][positionX].Left = gGrids.nGrid[positionY][positionX > 0 ? positionX - 1 : positionX].Curr;
		gGrids.nGrid[positionY][positionX].Right = gGrids.nGrid[positionY][positionX < NumGrids ? positionX + 1 : positionX].Curr;

		if (!(gGrids.nGrid[positionY][positionX].Up == NotVisited
			|| gGrids.nGrid[positionY][positionX].Down == NotVisited
			|| gGrids.nGrid[positionY][positionX].Left == NotVisited
			|| gGrids.nGrid[positionY][positionX].Right == NotVisited))
		{
			break;
		}

		if (gGrids.nGrid[positionY][positionX].Curr == NotVisited)
		{
			int iObjType = EMPTY;
			int iPercentage = rand() % 10;

			if (iPercentage > 6)
			{
				iObjType = WALL;
			}

			gGrids.gGrid[positionY][positionX]->type = iObjType;
			gGrids.nGrid[positionY][positionX].Curr = Visited;
		}


		if (gGrids.nGrid[positionY][positionX].Down == NotVisited)
		{
			positionY += 1;
		}
		else if (gGrids.nGrid[positionY][positionX].Right == NotVisited)
		{
			positionX += 1;
		}
		else if (gGrids.nGrid[positionY][positionX].Up == NotVisited)
		{
			positionY -= 1;
		}
		else if (gGrids.nGrid[positionY][positionX].Left == NotVisited)
		{
			positionX -= 1;
		}
	}*/

	iAutoGenerate = 0;
}
