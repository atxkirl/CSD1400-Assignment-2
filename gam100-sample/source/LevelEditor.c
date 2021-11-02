#include "LevelEditor.h"
#include <stdlib.h>
#include <stdio.h>
#include "FileParser.h"
#include "SystemManager.h"
#include "LinkedList.h"
#include "Colors.h"

#define LCLICK "LCLICK"
#define LCLICK_SHIFT "LCLICK_SHIFT"
#define RCLICK "RCLICK"
#define RCLICK_SHIFT "RCLICK_SHIFT"

int iSize;
OBJECT_TYPE objType;
OBJECT_DIRECTION objDirection;

float fMoveX;
float fMoveY;
float fScaleBy;
CP_Vector vScale;
CP_Matrix mScale;

Grid gGrids;
int iAutoGenerate;

extern GameObject *GameObjectList;
Renderer* renderImage;

void CheckGridInt(int iX, int iY, int iObjType)
{
	if (gGrids.gGrid[iY][iX]->type != iObjType)
	{
		gGrids.gGrid[iY][iX]->type = iObjType;
	}
}
void AssignDirectionInt(int iX, int iY, int iDirection)
{
	if (gGrids.gGrid[iY][iX]->oDirection != iDirection)
	{
		gGrids.gGrid[iY][iX]->oDirection = iDirection;
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
		else if (strcmp(r->obj->tag, RCLICK) == 0)
		{
			//CheckGrid((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, EMPTY);
			CheckGridInt((int)tempGO->position.x / iSize, (int)tempGO->position.y / iSize, EMPTY);
			LoadTileImage();
		}
		else if (strcmp(r->obj->tag, RCLICK_SHIFT) == 0)
		{
			//AssignDirection((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, RIGHT);
			AssignDirectionInt((int)tempGO->position.x / iSize, (int)tempGO->position.y / iSize, RIGHT);
			LoadTileImage();
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

	// set all to empty first
	for (int i = 0; i < NumGrids; i++)
	{
		for (int j = 0; j < NumGrids; j++)
		{
			//gGrids.gGrid[i][j] = isEmpty;
			GameObject* go = GOM_Create((i == 0 || i == NumGrids - 1 || j == 0 || j == NumGrids -1)? WALL : EMPTY);
			Renderer* r = RM_AddComponent(go);
			go->position = CP_Vector_Set(j * vScale.x * iSize, i * vScale.y * iSize);
			go->scale = CP_Vector_Set(vScale.x, vScale.y);
			go->oDirection = RIGHT * 90.0f;
			r->color = CP_Color_Create(255, 255, 0, 255);
			gGrids.gGrid[i][j] = go;
			go->tag = "grid";

			gGrids.nGrid[i][j].Curr = go->type == WALL ? Visited : NotVisited;
			gGrids.nGrid[i][j].Up = go->type == WALL ? Visited : NotVisited;
			gGrids.nGrid[i][j].Down = go->type == WALL ? Visited : NotVisited;
			gGrids.nGrid[i][j].Left = go->type == WALL ? Visited : NotVisited;
			gGrids.nGrid[i][j].Right = go->type == WALL ? Visited : NotVisited;

			Collider* c = CLM_AddComponent(go);
			CLM_Set(c, COL_BOX, LevelEditor_OnClickGrid);
			c->isLockedPos = 1;
			c->layer = COLLAY_WALL;
		}
	}

	iAutoGenerate = 0;

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
			objDirection--;
			if (objDirection <= 0)
				objDirection = 0;

			PrintCurrentDirection(objDirection);
		}
		else
		{
			objType++;
			if (objType >= TYPE_END)
				objType = TYPE_END - 1;
			PrintCurrentType(objType);
		}
	}
	else if (CP_Input_KeyTriggered(KEY_E))
	{
		if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
		{
			objDirection++;
			if (objDirection >= DIRECTION_END)
				objDirection = DIRECTION_END - 1;

			PrintCurrentDirection(objDirection);
		}
		else
		{
			objType--;
			if (objType < 0)
				objType = 0;
			PrintCurrentType(objType);
		}
	}

	if (CP_Input_KeyTriggered(KEY_SPACE))
	{
		iAutoGenerate = !iAutoGenerate; 
		
		AutoGenerateGrid();
		LoadTileImage();
	}

	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		GameObject* clickPoint = GOM_CreateTemp(EMPTY);
		clickPoint->position = RM_MousePositionToWorldSpace(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		clickPoint->tag = LCLICK;
		Collider* c = CLM_AddComponent(clickPoint);
		CLM_Set(c, COL_POINT, NULL);
		c->space = COLSPC_WORLD;

		if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
		{
			clickPoint->tag = LCLICK_SHIFT;
		}

		clickPoint->scale = CP_Vector_Set(10.0f, 10.0f);
		RM_AddComponent(clickPoint);
	}
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_2))
	{
		GameObject* clickPoint = GOM_CreateTemp(EMPTY);
		clickPoint->position = RM_MousePositionToWorldSpace(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		clickPoint->tag = RCLICK;
		Collider* c = CLM_AddComponent(clickPoint);
		CLM_Set(c, COL_POINT, NULL);
		c->space = COLSPC_WORLD;

		if (CP_Input_KeyDown(KEY_LEFT_SHIFT))
			clickPoint->tag = RCLICK_SHIFT;

		//GameObject* t = GOM_Create(CIRCLE);
		//t->position = CP_Vector_Set(clickPoint->position.x, clickPoint->position.y);
		//t->scale = CP_Vector_Set(10.0f, 10.0f);
		//RM_AddComponent(t);
	}

	//PlaceObject();
	RenderObjects();

	SM_SystemsUpdate();

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
					CheckGrid((CP_Input_GetMouseX() - fMoveX) / fScaleBy, (CP_Input_GetMouseY() - fMoveY) / fScaleBy, EMPTY);
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
	*/

	//Set everything to wall.
	for (int i = 1; i < NumGrids; i++)
	{
		for (int j = 1; j < NumGrids; j++)
		{
			gGrids.nGrid[j][i].Curr = NotVisited;
			gGrids.gGrid[j][i]->type = WALL;
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
		tempGO->type = EMPTY;
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

		//gGrids.gGrid[positionY][positionX]->type = EMPTY;
		//CheckSurrounding(positionX, positionY, (int)tempGO->position.x, (int)tempGO->position.y);
		*/
		// set the new positions and add the new walls.
 		positionX = (int)tempGO->position.x / iSize;
		positionY = (int)tempGO->position.y / iSize;
		AddFrontierCell(positionX, positionY, ll_WallList);
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

void AddFrontierCell(int x, int y, LinkedList* List)
{
	int offset = 1;
	int bounds = offset + 1;

	if (gGrids.gGrid[y][x]->type != WALL)
		return;

	if (y > bounds)
	{
		if (gGrids.gGrid[y - offset][x]->type == WALL)
			LL_Add(&List, gGrids.gGrid[y - offset][x]); // up
	}

	if (y < NumGrids - bounds)
	{
		if (gGrids.gGrid[y + offset][x]->type == WALL)
			LL_Add(&List, gGrids.gGrid[y + offset][x]); // down
	}

	if (x > bounds)
	{
		if (gGrids.gGrid[y][x - offset]->type == WALL)
			LL_Add(&List, gGrids.gGrid[y][x - offset]); // left
	}

	if (x < NumGrids - bounds)
	{
		if (gGrids.gGrid[y][x + offset]->type == WALL)
			LL_Add(&List, gGrids.gGrid[y][x + offset]); // right
	}
}

void PrintCurrentType(int iObjType)
{
	/*
	EMPTY = 0,
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

	case(WALL):
		printf("Object Type: WALL\n");
		break;

	case(PLAYER):
		printf("Object Type: PLAYER\n");
		break;

	case(LINE):
		printf("Object Type: LINE\n");
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
		printf("Object Type: UP\n");
		break;

	case(LEFT):
		printf("Object Type: LEFT\n");
		break;

	case(DOWN):
		printf("Object Type: DOWN\n");
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
			switch (gGrids.gGrid[i][j]->type)
			{
			case(WALL):
				gGrids.gGrid[i][j]->scale.x = 16.f;
				gGrids.gGrid[i][j]->scale.y = 16.f;
				renderImage = RM_GetComponent(gGrids.gGrid[i][j]);
				//RM_DeleteImage(renderImage);
				RM_LoadImage(renderImage, "Assets/bananaboi.png");
				//CP_Settings_Fill(CP_Color_Create(255, 0, 128, 225)); // r, g, b, a
				//CP_Graphics_DrawRect((float)j * iSize + fMoveX, (float)i * iSize + fMoveY, (float)iSize, (float)iSize);
				break;
			case(EMPTY):
				gGrids.gGrid[i][j]->scale.x = 16.f;
				gGrids.gGrid[i][j]->scale.y = 16.f;
				renderImage = RM_GetComponent(gGrids.gGrid[i][j]);
				//RM_DeleteImage(renderImage);
				RM_LoadImage(renderImage, "Assets/sand-tiles/sand-tile-0.png");
				//CP_Settings_Fill(CP_Color_Create(255, 128, 0, 225)); // r, g, b, a
				//CP_Graphics_DrawRect((float)j * iSize + fMoveX, (float)i * iSize + fMoveY, (float)iSize, (float)iSize);
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

//void CheckSurrounding(int x, int y, int newX, int newY)
//{
//	int offset = 1;
//	int bounds = offset + 1;
//
//	if (x < NumGrids - bounds && gGrids.gGrid[y][x + bounds]->type == EMPTY)
//	{
//		gGrids.gGrid[y][x + offset]->type = EMPTY; // left
//	}
//
//	else if (x > bounds && gGrids.gGrid[y][x - bounds]->type == EMPTY)
//	{
//		gGrids.gGrid[y][x - offset]->type = EMPTY; // right
//	}
//
//	else if (y < NumGrids - bounds && newY && gGrids.gGrid[y + bounds][x]->type == EMPTY)
//	{
//		gGrids.gGrid[y + offset][x]->type = EMPTY; // up
//	}
//
//	else if (y > bounds && gGrids.gGrid[y - bounds][x]->type == EMPTY)
//	{
//		gGrids.gGrid[y - offset][x]->type = EMPTY; // down
//	}
//}