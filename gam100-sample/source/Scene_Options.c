#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "Colors.h"
#include "SystemManager.h"
#include "FileParser.h"
#include "Scene_Options.h"

#define BUTTON_WIDTH 60.f
#define BUTTON_HEIGHT 30.f

GameObject* Options_Title, * Options_Background, * Options_Cross;
GameObject* ButtonL, * ButtonRUp, * ButtonRDown, * ButtonRLeft, * ButtonRRight, * ButtonRInteract, * ButtonSave;
float screenWidth, screenHeight;
char cRightText[5];
char* SaveFile;
int iEditUp, iEditDown, iEditLeft, iEditRight,iEditInteract;

void SceneOptionsUI_Buttons();
void SceneOptions_AssignKeyPress();

void SceneOptions_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "cross") == 0)
            SceneManager_ChangeSceneByName("mainmenu");
        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonSave") == 0)
        {
            WriteControlsToFile(SaveFile, cRightText, 5);
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonUp") == 0)
        {
            iEditUp = 1;
            iEditDown = 0;
            iEditLeft = 0;
            iEditRight = 0;
            iEditInteract = 0;
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonDown") == 0)
        {
            iEditDown = 1;
            iEditUp = 0;
            iEditLeft = 0;
            iEditRight = 0;
            iEditInteract = 0;
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonLeft") == 0)
        {
            iEditLeft = 1;
            iEditUp = 0;
            iEditDown = 0;
            iEditRight = 0;
            iEditInteract = 0;
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonRight") == 0)
        {
            iEditRight = 1;
            iEditUp = 0;
            iEditDown = 0;
            iEditLeft = 0;
            iEditInteract = 0;
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonInteract") == 0)
        {
            iEditInteract = 1;
            iEditUp = 0;
            iEditDown = 0;
            iEditLeft = 0;
            iEditRight = 0;
        }

    }
    return;
}

void SceneOptions_init(void)
{
    SM_SystemsInit();
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);

    SaveFile = "Controls/controls.txt";

    Collider* c = NULL;
    //title
    Options_Title = GOM_Create2(EMPTY, CP_Vector_Set(screenWidth * 0.5f, 75), 0, CP_Vector_Set(800, 100));
    Renderer* r = RM_AddComponent(Options_Title);
    r->color.a = 0;
    //r->text = "Options";
    RM_SetText(r, "Options");
    r->renderPriority = PRI_UI;
    r->textScale = CP_Vector_Set(3.0f, 3.0f);

    //UI Background
    Options_Background = GOM_Create(RECTANGLE);
    r = RM_AddComponent(Options_Background);
    r->color = CP_Color_Create(200, 200, 200, 255);
    r->renderPriority = PRI_UI;
    Options_Background->scale = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.7f);
    Options_Background->position = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f);

    //cross
    Options_Cross = GOM_Create(RECTANGLE);
    Options_Cross->position = CP_Vector_Set(screenWidth * 0.775f, screenHeight * 0.2f);
    Options_Cross->scale = CP_Vector_Set(50, 50);
    Options_Cross->tag = "cross";
    r = RM_AddComponent(Options_Cross);
    RM_LoadImage(r, "Assets/cross.png");
    r->renderPriority = PRI_UI;
    c = CLM_AddComponent(Options_Cross);
    CLM_Set(c, COL_BOX, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;

    iEditUp = 0;
    iEditDown = 0;
    iEditLeft = 0;
    iEditRight = 0;
    iEditInteract = 0;

    SceneOptionsUI_Buttons();
}

void SceneOptions_update(void)
{
    SM_SystemsPreUpdate();

    SceneOptions_AssignKeyPress();

    SM_SystemsUpdate();

    SM_SystemsLateUpdate();
}

void SceneOptions_exit(void)
{
    SM_SystemsExit();
}

void SceneOptions_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = SceneOptions_init;
    *update = SceneOptions_update;
    *exit = SceneOptions_exit;
}

void SceneOptionsUI_Buttons()
{
    ReadControlsFromFile(SaveFile, cRightText);

	CP_Vector ButtonScale = CP_Vector_Set(screenWidth * 0.125f, screenHeight * 0.06125f);
	CP_Vector Button_One_Left = CP_Vector_Set(screenWidth * 0.4f, screenHeight * 0.3f);
	CP_Vector ConnectorPositio_Two_Left = CP_Vector_Set(screenWidth * 0.4f, screenHeight * 0.4f);
	CP_Vector Button_Three_Left = CP_Vector_Set(screenWidth * 0.4f, screenHeight * 0.5f);
    CP_Vector Button_Four_Left = CP_Vector_Set(screenWidth * 0.4f, screenHeight * 0.6f);
    CP_Vector Button_Five_Left = CP_Vector_Set(screenWidth * 0.4f, screenHeight * 0.7f);

	CP_Vector Button_One_Right = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.3f);
	CP_Vector ConnectorPositio_Two_Right = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.4f);
	CP_Vector Button_Three_Right = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.5f);
    CP_Vector Button_Four_Right = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.6f);
    CP_Vector Button_Five_Right = CP_Vector_Set(screenWidth * 0.6f, screenHeight * 0.7f);

	CP_Vector Button_Left[5];
	Button_Left[0] = Button_One_Left;
	Button_Left[1] = ConnectorPositio_Two_Left;
	Button_Left[2] = Button_Three_Left;
    Button_Left[3] = Button_Four_Left;
    Button_Left[4] = Button_Five_Left;

	CP_Vector Button_Right[5];
	Button_Right[0] = Button_One_Right;
	Button_Right[1] = ConnectorPositio_Two_Right;
	Button_Right[2] = Button_Three_Right;
    Button_Right[3] = Button_Four_Right;
    Button_Right[4] = Button_Five_Right;

    char* cTextLeft[5] = { "UP", "DOWN", "LEFT", "RIGHT", "INTERACT" };
    Renderer* r;
    for (int i = 0; i < 5; i++)
    {
        ButtonL = GOM_Create(RECTANGLE);
        r = RM_AddComponent(ButtonL);
        r->color = COLOR_WHITE;
        r->renderPriority = PRI_UI;
        ButtonL->scale = ButtonScale;
        ButtonL->position = Button_Left[i];
        //r->text = cTextLeft[i];
        RM_SetText(r, cTextLeft[i]);
    }

    ButtonRUp = GOM_Create(RECTANGLE);
    r = RM_AddComponent(ButtonRUp);
    r->color = COLOR_WHITE;
    r->renderPriority = PRI_UI;
    ButtonRUp->tag = "ButtonUp";
    ButtonRUp->scale = ButtonScale;
    ButtonRUp->position = Button_Right[0];
    char temp[2] = { cRightText[0] , '\0' };
    RM_SetText(r, temp);
    Collider * c = CLM_AddComponent(ButtonRUp);
    CLM_Set(c, COL_BOX, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;

    ButtonRDown = GOM_Create(RECTANGLE);
    r = RM_AddComponent(ButtonRDown);
    r->color = COLOR_WHITE;
    r->renderPriority = PRI_UI;
    ButtonRDown->tag = "ButtonDown";
    ButtonRDown->scale = ButtonScale;
    ButtonRDown->position = Button_Right[1];
    temp[0] = cRightText[1];
    RM_SetText(r, temp);
    c = CLM_AddComponent(ButtonRDown);
    CLM_Set(c, COL_BOX, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;

    ButtonRLeft = GOM_Create(RECTANGLE);
    r = RM_AddComponent(ButtonRLeft);
    r->color = COLOR_WHITE;
    r->renderPriority = PRI_UI;
    ButtonRLeft->tag = "ButtonLeft";
    ButtonRLeft->scale = ButtonScale;
    ButtonRLeft->position = Button_Right[2];
    temp[0] = cRightText[2];
    RM_SetText(r, temp);
    c = CLM_AddComponent(ButtonRLeft);
    CLM_Set(c, COL_BOX, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;

    ButtonRRight = GOM_Create(RECTANGLE);
    r = RM_AddComponent(ButtonRRight);
    r->color = COLOR_WHITE;
    r->renderPriority = PRI_UI;
    ButtonRRight->tag = "ButtonRight";
    ButtonRRight->scale = ButtonScale;
    ButtonRRight->position = Button_Right[3];
    temp[0] = cRightText[3];
    RM_SetText(r, temp);
    c = CLM_AddComponent(ButtonRRight);
    CLM_Set(c, COL_BOX, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;

    ButtonRInteract = GOM_Create(RECTANGLE);
    r = RM_AddComponent(ButtonRInteract);
    r->color = COLOR_WHITE;
    r->renderPriority = PRI_UI;
    ButtonRInteract->tag = "ButtonInteract";
    ButtonRInteract->scale = ButtonScale;
    ButtonRInteract->position = Button_Right[4];
    temp[0] = cRightText[4];
    RM_SetText(r, temp);
    c = CLM_AddComponent(ButtonRInteract);
    CLM_Set(c, COL_BOX, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;

    ButtonSave = GOM_Create(RECTANGLE);
    r = RM_AddComponent(ButtonSave);
    r->color = COLOR_WHITE;
    r->renderPriority = PRI_UI;
    ButtonSave->tag = "ButtonSave";
    ButtonSave->scale = ButtonScale;
    ButtonSave->position = CP_Vector_Set(screenWidth * 0.725f, screenHeight * 0.8f);
    RM_SetText(r, "Save");
    c = CLM_AddComponent(ButtonSave);
    CLM_Set(c, COL_BOX, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;
}

void SceneOptions_AssignKeyPress()
{
    if (!iEditUp && !iEditDown && !iEditLeft && !iEditRight && !iEditInteract)
        return;

    char cKeyToAssign;

    for (int i = KEY_A; i < KEY_Z + 1; i++)
    {
        if(CP_Input_KeyTriggered((CP_KEY)i))
        {
            printf("Key: %c\n", (char)i);

            cKeyToAssign = (char)i;

            if (iEditUp)
            {
                cRightText[0] = cKeyToAssign;
                Renderer* r = RM_GetComponent(ButtonRUp);
                char temp[2] = { cRightText[0], '\0' };
                RM_SetText(r, temp);
            }
            else if (iEditDown)
            {
                cRightText[1] = cKeyToAssign;
                Renderer* r = RM_GetComponent(ButtonRDown);
                char temp[2] = { cRightText[1], '\0' };
                RM_SetText(r, temp);
            }
            else if (iEditLeft)
            {
                cRightText[2] = cKeyToAssign;
                Renderer* r = RM_GetComponent(ButtonRLeft);
                char temp[2] = { cRightText[2], '\0' };
                RM_SetText(r, temp);
            }
            else if (iEditRight)
            {
                cRightText[3] = cKeyToAssign;
                Renderer* r = RM_GetComponent(ButtonRRight);
                char temp[2] = { cRightText[3], '\0' };
                RM_SetText(r, temp);
            }
            else if (iEditInteract)
            {
                cRightText[4] = cKeyToAssign;
                Renderer* r = RM_GetComponent(ButtonRInteract);
                char temp[2] = { cRightText[4], '\0' };
                RM_SetText(r, temp);
            }

            iEditUp = 0;
            iEditDown = 0;
            iEditLeft = 0;
            iEditRight = 0;
            iEditInteract = 0;
            return;
        }
    }
}

void Options_LoadControls(char* controls)
{
    ReadControlsFromFile("Controls/controls.txt", cRightText);
    for (int i = 0; i < (int)(sizeof(cRightText) / sizeof(*cRightText)); ++i)
    {
        controls[i] = cRightText[i];
    }
}
