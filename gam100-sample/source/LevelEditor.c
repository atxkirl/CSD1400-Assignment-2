#include "LevelEditor.h"
#include <stdlib.h>
#include <stdio.h>
#include "FileParser.h"
#include "SystemManager.h"
#include "LinkedList.h"
#include "Colors.h"

#define LCLICK "LCLICK"
#define LCLICK_SHIFT "LCLICK_SHIFT"
#define LCLICK_CTRL "LCLICK_CTRL"
#define RCLICK "RCLICK"
#define RCLICK_SHIFT "RCLICK_SHIFT"
#define RCLICK_CTRL "RCLICK_CTRL"

// SPACEBAR to auto generate grid
// ENTER to save grid to txt
// wasd to move
// 1 2 to zoom in and out
// 3 to Load Map
// 4 to toggle tile mode
// Q E to change GameObject Type
// Q E + LSHIFT to change GameObject Direction
// LEFT CLICK to set tile, RIGHT CLICK to delete tile
// LEFT CLICK + SHIFT to set direction, RIGHT CLICK + SHIFT to reset direction
// LEFT CLICK + CTRL to set PlayerSpawn, RIGHT CLICK + CTRL to set Objectives

int iSize;
OBJECT_TYPE objType;
OBJECT_DIRECTION objDirection;

float fMoveX;
float fMoveY;
float fScaleBy;
CP_Vector vScale;
CP_Matrix mScale;

Grid gGrids;
Grid gObjectGrids;
int iAutoGenerate;

extern GameObject *GameObjectList;
Renderer* renderImage;

int isTileMode;

void CheckGridInt(int iX, int iY, int iObjType)
{
	if (isTileMode)
	{
		if (gGrids.gGrid[iY][iX]->type != iObjType)
		{
			gGrids.gGrid[iY][iX]->type = iObjType;
		}
	}
	else
	{
		if (gObjectGrids.gGrid[iY][iX]->type != iObjType)
		{
			gObjectGrids.gGrid[iY][iX]->type = iObjType;
		}
	}
}

void AssignDirectionInt(int iX, int iY, int iDirection)
{
	if (isTileMode)
	{
		if (gGrids.gGrid[iY][iX]->oDirection != iDirection)
		{
			gGrids.gGrid[iY][iX]->oDirection = iDirection;
			gGrids.gGrid[iY][iX]->rotation = gGrids.gGrid[iY][iX]->oDirection * 90.f;
		}
	}
	else
	{
		if (gObjectGrids.gGrid[iY][iX]->oDirection != iDirection)
		{
			gObjectGrids.gGrid[iY][iX]->oDirection = iDirection;
			gObjectGrids.gGrid[iY][iX]->rotation = gObjectGrids.gGrid[iY][iX]->oDirection * 90.f;
		}
	}
}

void LevelEditor_OnClickGrid(Collider* l, Collider* r)
{
	if (strcmp(l->obj->tag, "grid") == 0)
	{
		GameObject* tempGO = (GameObject*)l->obj;
		if (strcmp(r->obj->tag, LCLICK) == 0)
		{
			//CheckGrid(CP_Input_GetMouseX() / fScaleBy - fMoveX, CP_Input_GetMouseY() / fScaleBy - fMoveY, objType);
			CheckGridInt((int)tempGO->position.x / iSize, (int)tempGO->position.y / iSize, objType);
			LoadTileImage();
		}
		else if (strcmp(r->obj->tag, LCLICK_SHIFT) == 0)
		{
			//AssignDirection((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, objDirection);
			AssignDirectionInt((int)tempGO->position.x / iSize, (int)tempGO->position.y / iSize, objDirection);
			LoadTileImage();
		}
		else if (strcmp(r->obj->tag, LCLICK_CTRL) == 0)
		{
			//AssignDirection((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, objDirection);
			int iX = (int)tempGO->position.x / iSize;
			int iY = (int)tempGO->position.y / iSize;

			if (isTileMode)
			{
				gGrids.gGrid[iY][iX]->tag = "PlayerSpawn";
				printf("Player Spawn set\n");
			}
			else
			{
				gObjectGrids.gGrid[iY][iX]->tag = "EnemySpawn";
				printf("Enemy Spawn set\n");
			}
		}
		else if (strcmp(r->obj->tag, RCLICK) == 0)
		{
			//CheckGrid((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, FLOOR);
			CheckGridInt((int)tempGO->position.x / iSize, (int)tempGO->position.y / iSize, EMPTY);
			LoadTileImage();
		}
		else if (strcmp(r->obj->tag, RCLICK_SHIFT) == 0)
		{
			//AssignDirection((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, RIGHT);
			AssignDirectionInt((int)tempGO->position.x / iSize, (int)tempGO->position.y / iSize, RIGHT);
			LoadTileImage();
		}
		else if (strcmp(r->obj->tag, RCLICK_CTRL) == 0)
		{
			//AssignDirection((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, objDirection);
			int iX = (int)tempGO->position.x / iSize;
			int iY = (int)tempGO->position.y / iSize;
			gGrids.gGrid[iY][iX]->tag = "Objective";
			printf("Objective set\n");
		}
	}
}

/*!
@brief Initialises the variables
@param void
@return void
*/
void LevelEditorInit()
{
	objType = WALL; // initialize to 0;
	objDirection = RIGHT; // init to right
	iSize = CP_System_GetWindowHeight() / NumGrids;
	//iSize = (int)(WORLD_HEIGHT / NumGrids);

	fMoveX = 0.f;
	fMoveY = 0.f;
	fScaleBy = 1.f;
	vScale = CP_Vector_Set(fScaleBy, fScaleBy);
	mScale = CP_Matrix_Scale(vScale);

	for (int i = 0; i <= NumGrids; i++)
	{
		float fSize = (float)iSize;
		GameObject* go = GOM_Create(LINE);
		go->scale.x = fSize * NumGrids;
		go->position.x = i * fSize - fSize * 0.5f;
		go->position.y = -fSize * 0.5f;
		go->rotation = -90.0f;
		Renderer* r = RM_AddComponent(go);
		r->color = COLOR_GREY;

		go = GOM_Create(LINE);
		go->scale.x = fSize * NumGrids;
		go->position.x = -fSize * 0.5f;
		go->position.y = i * fSize - fSize * 0.5f;
		r = RM_AddComponent(go);
		r->color = COLOR_GREY;
	}

	// set all to FLOOR first
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			//gGrids.gGrid[i][j] = isFLOOR;
			GameObject* go = GOM_Create((i == 0 || i == NumGrids - 1 || j == 0 || j == NumGrids -1)? WATER : FLOOR);
			Renderer* r = RM_AddComponent(go);
			go->position = CP_Vector_Set(j * vScale.x * iSize, i * vScale.y * iSize);
			go->scale = CP_Vector_Set(vScale.x, vScale.y);
			go->oDirection = UP * 90.0f;
			r->color = CP_Color_Create(255, 255, 0, 255);
			gGrids.gGrid[i][j] = go;
			go->tag = "grid";

			gGrids.nGrid[i][j].Curr = (go->type == WATER) ? Visited : NotVisited;
			gGrids.nGrid[i][j].Up = (go->type == WATER) ? Visited : NotVisited;
			gGrids.nGrid[i][j].Down = (go->type == WATER) ? Visited : NotVisited;
			gGrids.nGrid[i][j].Left = (go->type == WATER) ? Visited : NotVisited;
			gGrids.nGrid[i][j].Right = (go->type == WATER) ? Visited : NotVisited;

			Collider* c = CLM_AddComponent(go);
			CLM_Set(c, COL_BOX, LevelEditor_OnClickGrid);
			c->isLockedPos = 1;
			c->layer = COLLAY_WALL;
		}
	}

	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			//gGrids.gGrid[i][j] = isFLOOR;
			GameObject* go = GOM_Create(EMPTY);
			Renderer* r = RM_AddComponent(go);
			go->position = CP_Vector_Set(j * vScale.x * iSize, i * vScale.y * iSize);
			go->scale = CP_Vector_Set(vScale.x, vScale.y);
			go->oDirection = UP * 90.0f;
			r->color = CP_Color_Create(255, 255, 0, 255);
			gObjectGrids.gGrid[i][j] = go;
			go->tag = "object";
			Collider* c = CLM_AddComponent(go);
			CLM_Set(c, COL_BOX, LevelEditor_OnClickGrid);
			c->isLockedPos = 1;
			c->layer = COLLAY_WALL;
		}
	}

	iAutoGenerate = 0;
	isTileMode = 1;

	RM_SetCameraPosition(CP_Vector_Set(WORLD_HEIGHT * 0.5f, WORLD_HEIGHT * 0.5f));

	LoadTileImage();
}

/*!
@brief Update
@param void
@return void
*/
void LevelEditorUpdate()
{
	SM_SystemsPreUpdate();

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

	if (CP_Input_KeyDown(KEY_D))
	{
		fMoveX += 50 * CP_System_GetDt();
	}

	else if (CP_Input_KeyDown(KEY_A))
	{
		fMoveX -= 50 * CP_System_GetDt();
	}

	if (CP_Input_KeyDown(KEY_1))
	{
		fScaleBy += 1 * CP_System_GetDt();
		vScale = CP_Vector_Set(fScaleBy, fScaleBy);
		//mScale = CP_Matrix_Scale(vScale);
		RM_SetCameraZoom(fScaleBy);
	}

	else if (CP_Input_KeyDown(KEY_2))
	{
		fScaleBy -= 1 * CP_System_GetDt();
		vScale = CP_Vector_Set(fScaleBy, fScaleBy);
		//mScale = CP_Matrix_Scale(vScale);
		RM_SetCameraZoom(fScaleBy);
	}

	if (CP_Input_KeyTriggered(KEY_Q))
	{
		if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
		{

			objDirection = (--objDirection < 0) ? DIRECTION_END - 1 : objDirection;

			PrintCurrentDirection(objDirection);
		}
		else
		{
			objType = (--objType < 0) ? TYPE_END - 1 : objType;
			PrintCurrentType(objType);
		}
	}
	else if (CP_Input_KeyTriggered(KEY_E))
	{
		if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
		{
			objDirection = (++objDirection >= DIRECTION_END) ? 0 : objDirection;

			PrintCurrentDirection(objDirection);
		}
		else
		{
			objType = (++objType >= TYPE_END) ? 0 : objType;
			PrintCurrentType(objType);
		}
	}

	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		iAutoGenerate = !iAutoGenerate; 
		
		AutoGenerateGrid();
		LoadTileImage();
	}

	if (CP_Input_MouseDown(MOUSE_BUTTON_1))
	{
		GameObject* clickPoint = GOM_CreateTemp(FLOOR);
		clickPoint->position = RM_MousePositionToWorldSpace(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		clickPoint->tag = LCLICK;
		Collider* c = CLM_AddComponent(clickPoint);
		CLM_Set(c, COL_POINT, NULL);
		c->space = COLSPC_WORLD;

		if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
		{
			clickPoint->tag = LCLICK_SHIFT;
		}

		if (CP_Input_KeyDown(KEY_LEFT_CONTROL))
		{
			clickPoint->tag = LCLICK_CTRL;
		}

		clickPoint->scale = CP_Vector_Set(10.0f, 10.0f);
		RM_AddComponent(clickPoint);
	}
	if (CP_Input_MouseDown(MOUSE_BUTTON_2))
	{
		GameObject* clickPoint = GOM_CreateTemp(FLOOR);
		clickPoint->position = RM_MousePositionToWorldSpace(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		clickPoint->tag = RCLICK;
		Collider* c = CLM_AddComponent(clickPoint);
		CLM_Set(c, COL_POINT, NULL);
		c->space = COLSPC_WORLD;

		if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
			clickPoint->tag = RCLICK_SHIFT;

		if (CP_Input_KeyDown(KEY_LEFT_CONTROL))
		{
			clickPoint->tag = RCLICK_CTRL;
		}
		//GameObject* t = GOM_Create(CIRCLE);
		//t->position = CP_Vector_Set(clickPoint->position.x, clickPoint->position.y);
		//t->scale = CP_Vector_Set(10.0f, 10.0f);
		//RM_AddComponent(t);
	}

	if (CP_Input_KeyTriggered(KEY_3))
	{
		LoadSavedMap();
	}

	if (CP_Input_KeyTriggered(KEY_4))
	{
		isTileMode = !isTileMode;
		printf("%s\n", (isTileMode != 0 ? "Tile Mode!" : "Object Mode!"));
	}

	//PlaceObject();
	RenderObjects();

	SM_SystemsUpdate(0);

	RM_SetCameraPosition(CP_Vector_Set(fMoveX + WORLD_HEIGHT * 0.5f, -fMoveY + WORLD_HEIGHT * 0.5f));

	SM_SystemsLateUpdate();
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
	//CP_Settings_ApplyMatrix(mScale);

	/* This will fill the background with grey color */
	CP_Graphics_ClearBackground(CP_Color_Create(128, 128, 128, 255));

	//// grids
	//CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	//for (int i = 0; i <= NumGrids; i++)
	//{
	//	CP_Graphics_DrawLine(0 + fMoveX,
	//		(float)i * iSize + fMoveY,
	//		(float)NumGrids * iSize + fMoveX,
	//		(float)i * iSize + fMoveY); // Draw horizontal line

	//}

	//for (int i = 0; i <= NumGrids; i++)
	//{
	//	CP_Graphics_DrawLine((float)i * iSize + fMoveX,
	//		0 + fMoveY,
	//		(float)i * iSize + fMoveX,
	//		(float)NumGrids * iSize + fMoveY); // Draw Vertical line
	//}
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
			if (CP_Input_GetMouseX() < NumGrids * iSize && CP_Input_GetMouseY() < NumGrids * iSize)
			{
				if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
				{
					AssignDirection((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, objDirection);
				}
				else
				{
					CheckGrid(CP_Input_GetMouseX() / fScaleBy - fMoveX, CP_Input_GetMouseY() / fScaleBy - fMoveY, objType);
				}

				LoadTileImage();
			}
		}

		if (CP_Input_MouseTriggered(MOUSE_BUTTON_2))
		{
			if (CP_Input_GetMouseX() < NumGrids * iSize && CP_Input_GetMouseY() < NumGrids * iSize)
			{
				if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
				{
					AssignDirection((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, RIGHT);
				}
				else
				{
					CheckGrid((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, FLOOR);
				}
				LoadTileImage();
			}
		}
	}
	else
	{
		AutoGenerateGrid();
		LoadTileImage();
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
	char cFileName[50];
	char** GridObj = malloc(sizeof(char*) * 900);

	for (int i = 0; i < 900; i++)
	{
		if (GridObj)
		{
			GridObj[i] = malloc(sizeof(char) * 70);

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
			if (gGrids.gGrid[i][j]->type !=  EMPTY && iObjNum < 900)
			{
				char ObjType[10];
				char ObjPosX[10];
				char ObjPosY[10];
				char cObjDirection[30];
				if (GridObj)
				{
					if (GridObj[iObjNum])
					{
						sprintf_s(ObjType, 10, "%d", gGrids.gGrid[i][j]->type);
						strcpy_s(GridObj[iObjNum], 70, ObjType); //type
						strcat_s(GridObj[iObjNum], 70, ",");

						sprintf_s(ObjPosX, 10, "%d", j);
						strcat_s(GridObj[iObjNum], 70, ObjPosX); // x
						strcat_s(GridObj[iObjNum], 70, ",");

						sprintf_s(ObjPosY, 10, "%d", i);
						strcat_s(GridObj[iObjNum], 70, ObjPosY); // y
						strcat_s(GridObj[iObjNum], 70, ",");
						printf("%s\n", GridObj[iObjNum]);

						sprintf_s(cObjDirection, 10, "%d", (int)gGrids.gGrid[i][j]->oDirection);
						strcat_s(GridObj[iObjNum], 70, cObjDirection); // dir
						strcat_s(GridObj[iObjNum], 70, ",");

						sprintf_s(cObjDirection, 30, "%s", gGrids.gGrid[i][j]->tag);
						strcat_s(GridObj[iObjNum], 70, cObjDirection); // tag
						strcat_s(GridObj[iObjNum], 70, "\n");

						iObjNum++;
					}
				}
			}
		}
	}

	if (GridObj != NULL)
	{
		char cFileLocation[100] = { "Levels/" };

		printf("Input a file name: ");
		scanf_s("%s", &cFileName, 50);
		strcat_s(cFileName, 50, ".txt");

		strcat_s(cFileLocation, 100, cFileName);
		printf("%s \n", cFileLocation);
		WriteToFile(cFileLocation, GridObj, 900);
	}

	char** GridObjects = malloc(sizeof(char*) * 900);
	for (int i = 0; i < 900; i++)
	{
		if (GridObjects)
		{
			GridObjects[i] = malloc(sizeof(char) * 70);

			if (GridObjects[i])
			{
				GridObjects[i][0] = '\0';
			}
		}
	}

	iObjNum = 0;
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			if (gObjectGrids.gGrid[i][j]->type != EMPTY && iObjNum < 900)
			{
				char ObjType[10];
				char ObjPosX[10];
				char ObjPosY[10];
				char cObjDirection[30];
				if (GridObjects)
				{
					if (GridObjects[iObjNum])
					{
						sprintf_s(ObjType, 10, "%d", gObjectGrids.gGrid[i][j]->type);
						strcpy_s(GridObjects[iObjNum], 70, ObjType); //type
						strcat_s(GridObjects[iObjNum], 70, ",");

						sprintf_s(ObjPosX, 10, "%d", j);
						strcat_s(GridObjects[iObjNum], 70, ObjPosX); // x
						strcat_s(GridObjects[iObjNum], 70, ",");

						sprintf_s(ObjPosY, 10, "%d", i);
						strcat_s(GridObjects[iObjNum], 70, ObjPosY); // y
						strcat_s(GridObjects[iObjNum], 70, ",");
						printf("%s\n", GridObjects[iObjNum]);

						sprintf_s(cObjDirection, 10, "%d", (int)gObjectGrids.gGrid[i][j]->oDirection);
						strcat_s(GridObjects[iObjNum], 70, cObjDirection); // dir
						strcat_s(GridObjects[iObjNum], 70, ",");

						sprintf_s(cObjDirection, 30, "%s", gObjectGrids.gGrid[i][j]->tag);
						strcat_s(GridObjects[iObjNum], 70, cObjDirection); // tag
						strcat_s(GridObjects[iObjNum], 70, "\n");

						iObjNum++;
					}
				}
			}
		}
	}

	if (GridObjects != NULL)
	{
		char cObjectFileLocation[100] = { "Objects/" };
		strcat_s(cObjectFileLocation, 100, cFileName);
		printf("%s \n", cObjectFileLocation);
		WriteToFile(cObjectFileLocation, GridObjects, 900);
	}


	for (int i = 0; i < 900; i++)
	{
		if (GridObj)
		{
			free(GridObj[i]);
		}

		if (GridObjects)
		{
			free(GridObjects[i]);
		}
	}
	free(GridObj);
	free(GridObjects);
}

void AutoGenerateGrid()
{
	////code here is for seed if i need it
	//int iCount;
	//printf("Insert Seed: ");
	//scanf_s("%d", &iCount);
	//srand(iSeed);

	// i referenced prims algorithm.

	/*
	//for (int i = 1; i < NumGrids - 1; i++)
	//{
	//	for (int j = 1; j < NumGrids - 1; j++)
	//	{
	//		int iObjType = FLOOR;
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
	*/

	//Set everything to wall.
	for (int i = 1; i < NumGrids; i++)
	{
		for (int j = 1; j < NumGrids; j++)
		{
			gGrids.nGrid[j][i].Curr = NotVisited;
			gGrids.gGrid[j][i]->type = WATER;
		}
	}

	// select a random cell in the grid
	int positionX = 1 + rand() % (NumGrids - 2), positionY = 1 + rand() % (NumGrids - 2);

	LinkedList* ll_WallList = NULL;
	LL_Add(&ll_WallList, gGrids.gGrid[positionY][positionX]);

	// Select the surrounding walls and add to the list
	AddFrontierCell(positionX, positionY, ll_WallList);

	GameObject* tempGO;
	tempGO = (GameObject*)ll_WallList->curr;

	while (1)
	{
		tempGO->type = FLOOR;
		LL_RemovePtr(&ll_WallList, tempGO);

		if (LL_GetCount(GetHead(ll_WallList)) == 0)
			break;

		//Select the next cell from the list.
		int iNext = rand() % LL_GetCount(GetHead(ll_WallList));
		void* tempNode = LL_Get(ll_WallList, iNext);
		LL_RemovePtr(&ll_WallList, tempNode);

		if (!ll_WallList)
			break;

		ll_WallList->curr = tempNode;
		tempGO = (GameObject*)tempNode;

		/*
		//int DifferenceX = (int)tempGO->position.x - positionX;
		//int DifferenceY = (int)tempGO->position.y - positionY;

		//printf("Position: %d, %d\n", positionX, positionY);
		//printf("TempGO: %d, %d\n", (int)tempGO->position.x, (int)tempGO->position.y);
		//printf("DIFFERENCE: %d, %d\n\n", DifferenceX, DifferenceY);

		//if (DifferenceX  > 0)
		//	positionX += 1;
		//else if (DifferenceX < 0)
		//	positionX -= 1;

		//if (DifferenceY > 0)
		//	positionY += 1;
		//else if (DifferenceY < 0)
		//	positionY -= 1;

		//gGrids.gGrid[positionY][positionX]->type = FLOOR;
		//CheckSurrounding(positionX, positionY, (int)tempGO->position.x, (int)tempGO->position.y);
		*/
		// set the new positions and add the new walls.
 		positionX = (int)tempGO->position.x / iSize;
		positionY = (int)tempGO->position.y / iSize;
		AddFrontierCell(positionX, positionY, ll_WallList);
	}

	for (int i = 1; i < NumGrids - 1; i++)
	{
		for (int j = 1; j < NumGrids - 1; j++)
		{
			int goUp = i - 1;
			int goDown = i + 1;
			int goRight = j + 1;
			int goLeft = j - 1;

			if (gGrids.gGrid[i][j]->type != FLOOR)
				continue;

			if (gGrids.gGrid[goUp][j]->type == WATER)
			{
				if (gGrids.gGrid[i][goLeft]->type == WATER)
				{
					if (gGrids.gGrid[i][goRight]->type == WATER)
					{
						// UP LEFT RIGHT
						gGrids.gGrid[i][j]->type = THREE_CORNER;
						gGrids.gGrid[i][j]->oDirection = RIGHT;
						gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
						continue;
					}

					else if(gGrids.gGrid[goDown][j]->type == WATER)
					{
						// UP LEFT DOWN
						gGrids.gGrid[i][j]->type = THREE_CORNER;
						continue;
					}

					// UP LEFT
					gGrids.gGrid[i][j]->type = CORNER;
					continue;
				}

				else if (gGrids.gGrid[i][goRight]->type == WATER)
				{
					if (gGrids.gGrid[goDown][j]->type == WATER)
					{
						// UP RIGHT DOWN
						gGrids.gGrid[i][j]->type = THREE_CORNER;
						gGrids.gGrid[i][j]->oDirection = DOWN;
						gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
						continue;
					}

					// UP RIGHT
					gGrids.gGrid[i][j]->type = CORNER;
					gGrids.gGrid[i][j]->oDirection = RIGHT;
					gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
					continue;
				}

				else if (gGrids.gGrid[i][goRight]->type == WATER)
				{
					// UP RIGHT
					gGrids.gGrid[i][j]->type = FLOOR_MIDDLE;
					continue;
				}

				else if (gGrids.gGrid[goDown][j]->type == WATER)
				{
					// UP DOWN
					gGrids.gGrid[i][j]->type = FLOOR_MIDDLE;
					gGrids.gGrid[i][j]->oDirection = LEFT;
					gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
					continue;
				}

				else
				{
					// UP
					gGrids.gGrid[i][j]->type = WALL;
					continue;
				}
			}

			else if (gGrids.gGrid[goDown][j]->type == WATER)
			{
				if (gGrids.gGrid[i][goLeft]->type == WATER)
				{
					if (gGrids.gGrid[i][goRight]->type == WATER) // DOWN LEFT RIGHT
					{
						gGrids.gGrid[i][j]->type = THREE_CORNER;
						gGrids.gGrid[i][j]->oDirection = LEFT;
						gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
						continue;
					}

					// DOWN LEFT
					gGrids.gGrid[i][j]->type = CORNER;
					gGrids.gGrid[i][j]->oDirection = LEFT;
					gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
					continue;
				}

				else if (gGrids.gGrid[i][goRight]->type == WATER)
				{
					// DOWN RIGHT
					gGrids.gGrid[i][j]->type = CORNER;
					gGrids.gGrid[i][j]->oDirection = DOWN;
					gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
					continue;
				}

				else
				{ 
					// DOWN
					gGrids.gGrid[i][j]->type = WALL;
					gGrids.gGrid[i][j]->oDirection = DOWN;
					gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
					continue;
				}
			}

			else if (gGrids.gGrid[i][goLeft]->type == WATER)
			{
				if (gGrids.gGrid[i][goRight]->type == WATER)
				{
					gGrids.gGrid[i][j]->type = FLOOR_MIDDLE;
					continue;
				}

				gGrids.gGrid[i][j]->type = WALL;
				gGrids.gGrid[i][j]->oDirection = LEFT;
				gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
				continue;
			}

			else if (gGrids.gGrid[i][goRight]->type == WATER)
			{
				if (gGrids.gGrid[i][goLeft]->type == WATER)
				{
					gGrids.gGrid[i][j]->type = FLOOR_MIDDLE;
					continue;
				}

				gGrids.gGrid[i][j]->type = WALL;
				gGrids.gGrid[i][j]->oDirection = RIGHT;
				gGrids.gGrid[i][j]->rotation = gGrids.gGrid[i][j]->oDirection * 90.f;
				continue;
			}
		}
	}

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
			int iObjType = FLOOR;
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

	LoadTileImage();
	iAutoGenerate = 0;
}

void AddFrontierCell(int x, int y, LinkedList* List)
{
	int offset = 1;
	int bounds = offset + 1;

	if (gGrids.gGrid[y][x]->type != WATER)
		return;

	if (y > bounds)
	{
		if (gGrids.gGrid[y - offset][x]->type == WATER)
			LL_Add(&List, gGrids.gGrid[y - offset][x]); // up
	}

	if (y < NumGrids - bounds)
	{
		if (gGrids.gGrid[y + offset][x]->type == WATER)
			LL_Add(&List, gGrids.gGrid[y + offset][x]); // down
	}

	if (x > bounds)
	{
		if (gGrids.gGrid[y][x - offset]->type == WATER)
			LL_Add(&List, gGrids.gGrid[y][x - offset]); // left
	}

	if (x < NumGrids - bounds)
	{
		if (gGrids.gGrid[y][x + offset]->type == WATER)
			LL_Add(&List, gGrids.gGrid[y][x + offset]); // right
	}
}

void PrintCurrentType(int iObjType)
{
	/*
	FLOOR = 0,
	CIRCLE,
	RECTANGLE,
	WALL,
	PLAYER,
	LINE,
	*/

	switch (iObjType)
	{
	case(EMPTY):
		printf("Object Type: EMPTY\n");
		break;

	case(CIRCLE):
		printf("Object Type: CIRCLE\n");
		break;

	case(RECTANGLE):
		printf("Object Type: RECTANGLE\n");
		break;

	case(WATER):
		printf("Object Type: WATER\n");
		break;

	case(FLOOR):
		printf("Object Type: FLOOR\n");
		break;

	case(WALL):
		printf("Object Type: WALL\n");
		break;

	case(CORNER):
		printf("Object Type: CORNER\n");
		break;

	case(PLAYER):
		printf("Object Type: PLAYER\n");
		break;

	case(LINE):
		printf("Object Type: LINE\n");
		break;

	case(FLOOR_MIDDLE):
		printf("Object Type: FLOOR_MIDDLE\n");
		break;

	case(THREE_CORNER):
		printf("Object Type: THREE_CORNER\n");
		break;

	case(PLAYER_SPAWN):
		printf("Object Type: PLAYER_SPAWN\n");
		break;

	case(ENEMY_SPAWN):
		printf("Object Type: ENEMY_SPAWN\n");
		break;

	case(CORAL):
		printf("Object Type: CORAL\n");
		break;

	case(GRASS):
		printf("Object Type: GRASS\n");
		break;

	case(EXIT):
		printf("Object Type: EXIT\n");
		break;

	case(BOAT):
		printf("Object Type: BOAT\n");
		break;
	default:
		break;
	}
}

void PrintCurrentDirection(int iObjDirection)
{
	switch (iObjDirection)
	{
	case(RIGHT):
		printf("Object Direction: RIGHT\n");
		break;

	case(UP):
		printf("Object Direction: UP\n");
		break;

	case(LEFT):
		printf("Object Direction: LEFT\n");
		break;

	case(DOWN):
		printf("Object Direction: DOWN\n");
		break;

	default:
		break;
	}
}

void AssignDirection(float fMouseX, float fMouseY, int iDirection)
{
	int iModPosX = (int)fMouseX % (int)iSize;
	int iModPosY = (int)fMouseY % (int)iSize;

	int iCurrentX = (int)(fMouseX - iModPosX) / iSize;
	int iCurrentY = (int)(fMouseY - iModPosY) / iSize;

	if (gGrids.gGrid[iCurrentY][iCurrentX]->oDirection != iDirection)
	{
		gGrids.gGrid[iCurrentY][iCurrentX]->oDirection = iDirection;
	}
}

void LoadTileImage()
{
	//render obj
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			gGrids.gGrid[i][j]->scale.x = 16.f;
			gGrids.gGrid[i][j]->scale.y = 16.f;
			renderImage = RM_GetComponent(gGrids.gGrid[i][j]);
			switch (gGrids.gGrid[i][j]->type)
			{
			case(WALL):
				RM_LoadImage(renderImage, "Assets/W.png");
				break;

			case(FLOOR_MIDDLE):
				RM_LoadImage(renderImage, "Assets/sand-tiles/sand-tile-13.png");
				break;

			case(THREE_CORNER):
				RM_LoadImage(renderImage, "Assets/sand-tiles/sand-tile-3c.png");
				break;

			case(FLOOR):
				RM_LoadImage(renderImage, "Assets/F.png");
				break;

			case(CORNER):
				RM_LoadImage(renderImage, "Assets/C.png");
				break;

			case(WATER):
				RM_LoadImage(renderImage, "Assets/sand-tiles/sea-tile-1.png");
				break;

			default:
				break;
			}
		}
	}

	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			gObjectGrids.gGrid[i][j]->scale.x = 16.f;
			gObjectGrids.gGrid[i][j]->scale.y = 16.f;
			renderImage = RM_GetComponent(gObjectGrids.gGrid[i][j]);
			switch (gObjectGrids.gGrid[i][j]->type)
			{
			case(CORAL):
				RM_LoadImage(renderImage, "Assets/redcoral.png");
				break;

			case(GRASS):
				RM_LoadImage(renderImage, "Assets/tempgrass.png");
				break;

			case(BOAT):
				RM_LoadImage(renderImage, "Assets/boat/boat.png");
				break;

			default:
				break;
			}
		}
	}
}

void LoadSavedMap()
{
	char cLevelFileLocation[100] = { "Levels/" };
	char cObjectFileLocation[100] = { "Objects/" };
	char* cInput = malloc(sizeof(char) * 50);
	printf("Open File: ");
	scanf_s("%s", cInput, 50);

	if (cInput)
	{
		strcat_s(cLevelFileLocation, 100, cInput);
		strcat_s(cLevelFileLocation, 100, ".txt");
		strcat_s(cObjectFileLocation, 100, cInput);
		strcat_s(cObjectFileLocation, 100, ".txt");
	}

	Map* objList = new_Map();
	ReadLevelFromFile(cLevelFileLocation, objList);
	Renderer* r;
	for (int i = 0; i < objList->iSize; i++)
	{
		if (objList->fObjList[i])
		{
			int iY = objList->fObjList[i]->iPosY;
			int iX = objList->fObjList[i]->iPosX;
			gGrids.gGrid[iY][iX]->type = objList->fObjList[i]->iType;
			gGrids.gGrid[iY][iX]->tag = objList->fObjList[i]->cTag;
			gGrids.gGrid[iY][iX]->position = CP_Vector_Set(iX * vScale.x * iSize, iY * vScale.y * iSize);
			gGrids.gGrid[iY][iX]->scale = CP_Vector_Set(vScale.x, vScale.y);
			gGrids.gGrid[iY][iX]->oDirection = objList->fObjList[i]->iDir;
			gGrids.gGrid[iY][iX]->rotation = gGrids.gGrid[iY][iX]->oDirection * 90.f;
			r = RM_GetComponent(gGrids.gGrid[iY][iX]);
			//gLoadedGrids.gGrid[iY][iX]->color = CP_Color_Create(255, 255, 255, 255);
			r->color = CP_Color_Create(255, 255, 255, 255);

			switch (gGrids.gGrid[iY][iX]->type)
			{
			case(WALL):
			case(CORNER):
				CLM_AddComponent(gGrids.gGrid[iY][iX]);
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
			gObjectGrids.gGrid[iY][iX]->type = objList2->fObjList[i]->iType;
			gObjectGrids.gGrid[iY][iX]->tag = objList2->fObjList[i]->cTag;
			gObjectGrids.gGrid[iY][iX]->position = CP_Vector_Set(iX * vScale.x * iSize, iY * vScale.y * iSize);
			gObjectGrids.gGrid[iY][iX]->scale = CP_Vector_Set(vScale.x, vScale.y);
			gObjectGrids.gGrid[iY][iX]->oDirection = objList2->fObjList[i]->iDir;
			gObjectGrids.gGrid[iY][iX]->rotation = gObjectGrids.gGrid[iY][iX]->oDirection * 90.f;
			r = RM_GetComponent(gObjectGrids.gGrid[iY][iX]);
			//gLoadedGrids.gGrid[iY][iX]->color = CP_Color_Create(255, 255, 255, 255);
			r->color = CP_Color_Create(255, 255, 255, 255);

			//switch (gObjectGrids.gGrid[iY][iX]->type)
			//{
			//case(CORAL):
			//case(GRASS):
			//	CLM_AddComponent(gObjectGrids.gGrid[iY][iX]);
			//	break;
			//default:
			//	break;
			//}
		}
	}
	LoadTileImage();
	free(cInput);
}