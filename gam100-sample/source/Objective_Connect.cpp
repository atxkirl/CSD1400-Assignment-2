#include "Objective_Connect.h"
#include "SystemManager.h"
#include "Colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GameObject* RedConnector_L, * GreenConnector_L, * BlueConnector_L, * YellowConnector_L;
GameObject* RedConnector_R, * GreenConnector_R, * BlueConnector_R, * YellowConnector_R;
GameObject* WireConnector_R , * WireConnector_G , * WireConnector_B , * WireConnector_Y;
GameObject* WireCollider_R, * WireCollider_G , * WireCollider_B , * WireCollider_Y;
GameObject* OBJ_Title, *UI_Background, *cross;
GameObject* hold_Wire = NULL;
int iRedConnected, iBlueConnected, iGreenConnected, iYellowConnected;
float StartingPosX, StartingPosY;
float StartingScaleX, StartingScaleY;

float screenWidth, screenHeight;
CP_Vector initialPos;

void OB_ConnectOnCollision(Collider* left, Collider* right)
{
	if (strcmp(((GameObject*)right->obj)->tag, "Drag") == 0)
	{
		if (strcmp(((GameObject*)left->obj)->tag, "WireCollider_R") == 0)
		{
			initialPos = ((GameObject*)right->obj)->position;
			StartingScaleX = WireConnector_R->scale.x;
			StartingScaleY = WireConnector_R->scale.y;
			hold_Wire = WireConnector_R;
		}

		else if (strcmp(((GameObject*)left->obj)->tag, "WireCollider_G") == 0)
		{
			initialPos = ((GameObject*)right->obj)->position;
			StartingScaleX = WireConnector_G->scale.x;
			StartingScaleY = WireConnector_G->scale.y;
			hold_Wire = WireConnector_G;
		}

		else if (strcmp(((GameObject*)left->obj)->tag, "WireCollider_B") == 0)
		{
			initialPos = ((GameObject*)right->obj)->position;
			StartingScaleX = WireConnector_B->scale.x;
			StartingScaleY = WireConnector_B->scale.y;
			hold_Wire = WireConnector_B;
		}

		else if (strcmp(((GameObject*)left->obj)->tag, "WireCollider_Y") == 0)
		{
			initialPos = ((GameObject*)right->obj)->position;
			StartingScaleX = WireConnector_Y->scale.x;
			StartingScaleY = WireConnector_Y->scale.y;
			hold_Wire = WireConnector_Y;
		}
		else if (strcmp(left->obj->tag, "cross") == 0)
		{
			RedConnector_L->isEnabled = 0;
			GreenConnector_L->isEnabled = 0;
			BlueConnector_L->isEnabled = 0;
			YellowConnector_L->isEnabled = 0;
			RedConnector_R->isEnabled = 0;
			GreenConnector_R->isEnabled = 0;
			BlueConnector_R->isEnabled = 0;
			YellowConnector_R->isEnabled = 0;
			WireConnector_R->isEnabled = 0;
			WireConnector_G->isEnabled = 0;
			WireConnector_B->isEnabled = 0;
			WireConnector_Y->isEnabled = 0;
			WireCollider_R->isEnabled = 0;
			WireCollider_G->isEnabled = 0;
			WireCollider_B->isEnabled = 0;
			WireCollider_Y->isEnabled = 0;
			OBJ_Title->isEnabled = 0;
			UI_Background->isEnabled = 0;
			cross->isEnabled = 0;
			iRedConnected = 0, iBlueConnected = 0, iGreenConnected = 0, iYellowConnected = 0;
		}
	}

	if (strcmp(((GameObject*)right->obj)->tag, "Down") == 0)
	{
		if (hold_Wire)
		{
			if (strcmp(hold_Wire->tag, "WireConnector_R") == 0)
			{
				if(strcmp(((GameObject*)left->obj)->tag, "RedRight") == 0)
					iRedConnected = 1;
			}

			else if (strcmp(hold_Wire->tag, "WireConnector_B") == 0)
			{
				if (strcmp(((GameObject*)left->obj)->tag, "BlueRight") == 0)
				{
					iBlueConnected = 1;
				}
			}

			else if (strcmp(hold_Wire->tag, "WireConnector_G") == 0)
			{
				if (strcmp(((GameObject*)left->obj)->tag, "GreenRight") == 0)
				{
					iGreenConnected = 1;
				}
			}

			else if (strcmp(hold_Wire->tag, "WireConnector_Y") == 0)
			{
				if (strcmp(((GameObject*)left->obj)->tag, "YellowRight") == 0)
				{
					iYellowConnected = 1;
				}
			}
		}
	}

	return;

}

void OB_ConnectInit()
{
	RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
	//Collider* c = NULL;

	//title
	OBJ_Title = GOM_Create2(EMPTY, CP_Vector_Set(screenWidth * 0.5f, 75), 0, CP_Vector_Set(800, 100));
	Renderer* r = RM_AddComponent(OBJ_Title);
	r->color.a = 0;
	r->text = "Connect The Wires!";
	r->renderPriority = PRI_UI;
	r->textScale = CP_Vector_Set(3.0f, 3.0f);

	//UI Background
	UI_Background = GOM_Create(RECTANGLE);
	r = RM_AddComponent(UI_Background);
	r->color = CP_Color_Create(200, 200, 200, 255);
	r->renderPriority = PRI_UI;
	UI_Background->scale = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.7f);
	UI_Background->position = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f);

	//cross
	cross = GOM_Create(RECTANGLE);
	cross->position = CP_Vector_Set(screenWidth * 0.775f, screenHeight * 0.2f);
	cross->scale = CP_Vector_Set(50, 50);
	cross->tag = "cross";
	r = RM_AddComponent(cross);
	RM_LoadImage(r, "Assets/cross.png");
	r->renderPriority = PRI_UI;
	Collider *c = CLM_AddComponent(cross);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	CreateConnectors();
	iRedConnected = 0, iBlueConnected = 0, iGreenConnected = 0, iYellowConnected = 0;
}

void OB_ConnectUpdate()
{
	GameObject* holdPoint = NULL;
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		WireCollider_R->position = CP_Vector_Set(WireConnector_R->position.x + WireConnector_R->scale.x, WireConnector_R->position.y);
		WireCollider_G->position = CP_Vector_Set(WireConnector_G->position.x + WireConnector_G->scale.x, WireConnector_G->position.y);
		WireCollider_B->position = CP_Vector_Set(WireConnector_B->position.x + WireConnector_B->scale.x, WireConnector_B->position.y);
		WireCollider_Y->position = CP_Vector_Set(WireConnector_Y->position.x + WireConnector_Y->scale.x, WireConnector_Y->position.y);
		//Creates a point obj to test collision against button
		holdPoint = GOM_CreateTemp(EMPTY);
		holdPoint->position = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		holdPoint->tag = "Drag";
		Collider* c = CLM_AddComponent(holdPoint);
		c->space = COLSPC_SCREEN;
		CLM_Set(c, COL_POINT, NULL);
	}

	if (CP_Input_MouseDown(MOUSE_BUTTON_1))
	{
		holdPoint = GOM_CreateTemp(EMPTY);
		holdPoint->position = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		holdPoint->tag = "Down";
		Collider* c = CLM_AddComponent(holdPoint);
		c->space = COLSPC_SCREEN;
		CLM_Set(c, COL_POINT, NULL);
	}

	if (hold_Wire)
	{
		//StartingScaleX + CP_Input_GetMouseX() - initialPos.x
		CP_Vector Distance = CP_Vector_Set(CP_Input_GetMouseX() - hold_Wire->position.x, CP_Input_GetMouseY() - hold_Wire->position.y);
		hold_Wire->scale = CP_Vector_Set(CP_Vector_Length(Distance), 0.f);

		float fAngle = CP_Vector_Angle(CP_Vector_Set(1.f, 0.f), Distance);
		if (Distance.y > 0.f)
			fAngle = 360.f - fAngle;
		hold_Wire->rotation = fAngle;
	}

	if (CP_Input_MouseReleased(MOUSE_BUTTON_1))
	{
		if (hold_Wire)
		{
			hold_Wire = NULL; 
			//iRedConnected = 0, BlueConnected = 0, iGreenConnected = 0, iYellowConnected = 0

			if (!iRedConnected)
			{
				WireConnector_R->scale = CP_Vector_Set(StartingScaleX, StartingScaleY);
				WireConnector_R->rotation = 0;
			}
			if (!iGreenConnected)
			{
				WireConnector_G->scale = CP_Vector_Set(StartingScaleX, StartingScaleY);
				WireConnector_G->rotation = 0;
			}
			if (!iBlueConnected)
			{
				WireConnector_B->scale = CP_Vector_Set(StartingScaleX, StartingScaleY);
				WireConnector_B->rotation = 0;
			}
			if (!iYellowConnected)
			{
				WireConnector_Y->scale = CP_Vector_Set(StartingScaleX, StartingScaleY);
				WireConnector_Y->rotation = 0;
			}

			initialPos = CP_Vector_Set(0, 0);
		}
	}

	if (iRedConnected && iBlueConnected && iGreenConnected && iYellowConnected)
	{
		Renderer* r = RM_GetComponent(OBJ_Title);
		r->text = "Complete!";
	}
}

void OB_ConnectTrigger()
{
}

void OB_ConnectUnTrigger()
{
}

void CreateConnectors()
{
	CP_Vector ConnectorScale = CP_Vector_Set(screenWidth * 0.125f, screenHeight * 0.06125f);
	CP_Vector ConnectorPosition_One_Left = CP_Vector_Set(screenWidth * 0.275f, screenHeight * 0.3f);
	CP_Vector ConnectorPositio_Two_Left = CP_Vector_Set(screenWidth * 0.275f, screenHeight * 0.45f);
	CP_Vector ConnectorPosition_Three_Left = CP_Vector_Set(screenWidth * 0.275f, screenHeight * 0.6f);
	CP_Vector ConnectorPosition_Four_Left = CP_Vector_Set(screenWidth * 0.275f, screenHeight * 0.75f);

	CP_Vector ConnectorPosition_One_Right = CP_Vector_Set(screenWidth * 0.725f, screenHeight * 0.3f);
	CP_Vector ConnectorPositio_Two_Right = CP_Vector_Set(screenWidth * 0.725f, screenHeight * 0.45f);
	CP_Vector ConnectorPosition_Three_Right = CP_Vector_Set(screenWidth * 0.725f, screenHeight * 0.6f);
	CP_Vector ConnectorPosition_Four_Right = CP_Vector_Set(screenWidth * 0.725f, screenHeight * 0.75f);

	CP_Vector ConnectorPosition_Left[4];
	ConnectorPosition_Left[0] = ConnectorPosition_One_Left;
	ConnectorPosition_Left[1] = ConnectorPositio_Two_Left;
	ConnectorPosition_Left[2] = ConnectorPosition_Three_Left;
	ConnectorPosition_Left[3] = ConnectorPosition_Four_Left;

	CP_Vector ConnectorPosition_Right[4];
	ConnectorPosition_Right[0] = ConnectorPosition_One_Right;
	ConnectorPosition_Right[1] = ConnectorPositio_Two_Right;
	ConnectorPosition_Right[2] = ConnectorPosition_Three_Right;
	ConnectorPosition_Right[3] = ConnectorPosition_Four_Right;

	///Left
	RedConnector_L = GOM_Create(RECTANGLE);
	Renderer* r = RM_AddComponent(RedConnector_L);
	r->color = CP_Color_Create(255, 0, 0, 255);
	r->renderPriority = PRI_UI;
	RedConnector_L->tag = "RedLeft";
	RedConnector_L->scale = ConnectorScale;
	int iL_FirstPosition = rand() % 4;
	RedConnector_L->position = ConnectorPosition_Left[iL_FirstPosition];

	WireConnector_R = GOM_Create(LINE);
	r = RM_AddComponent(WireConnector_R);
	r->color = CP_Color_Create(0, 0, 0, 255);
	r->renderPriority = PRI_UI;
	WireConnector_R->scale = CP_Vector_Set(screenWidth * 0.025f, screenHeight * 0.1f);
	WireConnector_R->position = RedConnector_L->position; 
	WireConnector_R->position.x += RedConnector_L->scale.x / 2;
	WireConnector_R->tag = "WireConnector_R";
	//WireConnector->tag = "WireConnector";

	WireCollider_R = GOM_Create(RECTANGLE);
	WireCollider_R->position = CP_Vector_Set(WireConnector_R->position.x + WireConnector_R->scale.x, WireConnector_R->position.y);
	WireCollider_R->scale = CP_Vector_Set(50.f, 50.f);
	WireCollider_R->tag = "WireCollider_R";
	Collider* c = CLM_AddComponent(WireCollider_R);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	BlueConnector_L = GOM_Create(RECTANGLE);
	r = RM_AddComponent(BlueConnector_L);
	r->color = CP_Color_Create(0, 0, 255, 255);
	r->renderPriority = PRI_UI;
	BlueConnector_L->tag = "BlueLeft";
	BlueConnector_L->scale = ConnectorScale;
	int iL_SecondPosition = rand() % 4;
	while (iL_SecondPosition == iL_FirstPosition)
		iL_SecondPosition = rand() % 4;
	BlueConnector_L->position = ConnectorPosition_Left[iL_SecondPosition];

	WireConnector_B = GOM_Create(LINE);
	r = RM_AddComponent(WireConnector_B);
	r->color = CP_Color_Create(0, 0, 0, 255);
	r->renderPriority = PRI_UI;
	WireConnector_B->scale = CP_Vector_Set(screenWidth * 0.025f, screenHeight * 0.1f);
	WireConnector_B->position = BlueConnector_L->position;
	WireConnector_B->position.x += BlueConnector_L->scale.x / 2;
	WireConnector_B->tag = "WireConnector_B";
	//WireConnector->tag = "WireConnector";

	WireCollider_B = GOM_Create(RECTANGLE);
	WireCollider_B->position = CP_Vector_Set(WireConnector_B->position.x + WireConnector_B->scale.x, WireConnector_B->position.y);
	WireCollider_B->scale = CP_Vector_Set(50.f, 50.f);
	WireCollider_B->tag = "WireCollider_B";
	c = CLM_AddComponent(WireCollider_B);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	GreenConnector_L = GOM_Create(RECTANGLE);
	r = RM_AddComponent(GreenConnector_L);
	r->color = CP_Color_Create(0, 255, 0, 255);
	r->renderPriority = PRI_UI;
	GreenConnector_L->tag = "GreenLeft";
	GreenConnector_L->scale = ConnectorScale;
	int iL_ThirdPosition = rand() % 4;
	while (iL_ThirdPosition == iL_FirstPosition || iL_ThirdPosition == iL_SecondPosition)
		iL_ThirdPosition = rand() % 4;
	GreenConnector_L->position = ConnectorPosition_Left[iL_ThirdPosition];

	WireConnector_G = GOM_Create(LINE);
	r = RM_AddComponent(WireConnector_G);
	r->color = CP_Color_Create(0, 0, 0, 255);
	r->renderPriority = PRI_UI;
	WireConnector_G->scale = CP_Vector_Set(screenWidth * 0.025f, screenHeight * 0.1f);
	WireConnector_G->position = GreenConnector_L->position;
	WireConnector_G->position.x += GreenConnector_L->scale.x / 2;
	WireConnector_G->tag = "WireConnector_G";
	//WireConnector->tag = "WireConnector";

	WireCollider_G = GOM_Create(RECTANGLE);
	WireCollider_G->position = CP_Vector_Set(WireConnector_G->position.x + WireConnector_G->scale.x, WireConnector_G->position.y);
	WireCollider_G->scale = CP_Vector_Set(50.f, 50.f);
	WireCollider_G->tag = "WireCollider_G";
	c = CLM_AddComponent(WireCollider_G);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	YellowConnector_L = GOM_Create(RECTANGLE);
	r = RM_AddComponent(YellowConnector_L);
	r->color = CP_Color_Create(255, 255, 0, 255);
	r->renderPriority = PRI_UI;
	YellowConnector_L->tag = "YellowLeft";
	YellowConnector_L->scale = ConnectorScale;
	int iL_FourthPosition = rand() % 4;
	while (iL_FourthPosition == iL_FirstPosition || iL_FourthPosition == iL_SecondPosition || iL_FourthPosition == iL_ThirdPosition)
		iL_FourthPosition = rand() % 4;
	YellowConnector_L->position = ConnectorPosition_Left[iL_FourthPosition];

	WireConnector_Y = GOM_Create(LINE);
	r = RM_AddComponent(WireConnector_Y);
	r->color = CP_Color_Create(0, 0, 0, 255);
	r->renderPriority = PRI_UI;
	WireConnector_Y->scale = CP_Vector_Set(screenWidth * 0.025f, screenHeight * 0.1f);
	WireConnector_Y->position = YellowConnector_L->position;
	WireConnector_Y->position.x += YellowConnector_L->scale.x / 2;
	WireConnector_Y->tag = "WireConnector_Y";
	//WireConnector->tag = "WireConnector";

	WireCollider_Y = GOM_Create(RECTANGLE);
	WireCollider_Y->position = CP_Vector_Set(WireConnector_Y->position.x + WireConnector_Y->scale.x, WireConnector_Y->position.y);
	WireCollider_Y->scale = CP_Vector_Set(50.f, 50.f);
	WireCollider_Y->tag = "WireCollider_Y";
	c = CLM_AddComponent(WireCollider_Y);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	///Right
	RedConnector_R = GOM_Create(RECTANGLE);
	r = RM_AddComponent(RedConnector_R);
	r->color = CP_Color_Create(255, 0, 0, 255);
	r->renderPriority = PRI_UI;
	RedConnector_R->tag = "RedRight";
	RedConnector_R->scale = ConnectorScale;
	int iR_FirstPosition = rand() % 4;
	RedConnector_R->position = ConnectorPosition_Right[iR_FirstPosition];
	c = CLM_AddComponent(RedConnector_R);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	BlueConnector_R = GOM_Create(RECTANGLE);
	r = RM_AddComponent(BlueConnector_R);
	r->color = CP_Color_Create(0, 0, 255, 255);
	r->renderPriority = PRI_UI;
	BlueConnector_R->tag = "BlueRight";
	BlueConnector_R->scale = ConnectorScale;
	int iR_SecondPosition = rand() % 4;
	while (iR_SecondPosition == iR_FirstPosition)
		iR_SecondPosition = rand() % 4;
	BlueConnector_R->position = ConnectorPosition_Right[iR_SecondPosition];
	c = CLM_AddComponent(BlueConnector_R);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	GreenConnector_R = GOM_Create(RECTANGLE);
	r = RM_AddComponent(GreenConnector_R);
	r->color = CP_Color_Create(0, 255, 0, 255);
	r->renderPriority = PRI_UI;
	GreenConnector_R->tag = "GreenRight";
	GreenConnector_R->scale = ConnectorScale;
	int iR_ThirdPosition = rand() % 4;
	while (iR_ThirdPosition == iR_FirstPosition || iR_ThirdPosition == iR_SecondPosition)
		iR_ThirdPosition = rand() % 4;
	GreenConnector_R->position = ConnectorPosition_Right[iR_ThirdPosition];
	c = CLM_AddComponent(GreenConnector_R);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;

	YellowConnector_R = GOM_Create(RECTANGLE);
	r = RM_AddComponent(YellowConnector_R);
	r->color = CP_Color_Create(255, 255, 0, 255);
	r->renderPriority = PRI_UI;
	YellowConnector_R->tag = "YellowRight";
	YellowConnector_R->scale = ConnectorScale;
	int iR_FourthPosition = rand() % 4;
	while (iR_FourthPosition == iR_FirstPosition || iR_FourthPosition == iR_SecondPosition || iR_FourthPosition == iR_ThirdPosition)
		iR_FourthPosition = rand() % 4;
	YellowConnector_R->position = ConnectorPosition_Right[iR_FourthPosition];
	c = CLM_AddComponent(YellowConnector_R);
	CLM_Set(c, COL_BOX, OB_ConnectOnCollision);
	c->space = COLSPC_SCREEN;
	c->isTrigger = 1;
}
