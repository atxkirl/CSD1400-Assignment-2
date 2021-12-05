/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file       Scene_Options.c
@author     Lim Guan Sheng, Marcus (l.guanshengmarcus)
@co-author  Ow Hong Yu (ow.h)
@course     CSD1400 Software Engineering Project 1
@Team       BananaBoi
@brief      This file contains the functions for the Scene Options. Player can edit the sounds 
            and controls.
*//*________________________________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "Colors.h"
#include "SystemManager.h"
#include "FileParser.h"
#include "Controls.h"
#include "SoundManager.h"

#define BUTTON_WIDTH 60.f
#define BUTTON_HEIGHT 30.f

GameObject* Options_Title, * Options_Background, * Options_Cross;
GameObject* ButtonL, * ButtonRUp, * ButtonRDown, * ButtonRLeft, * ButtonRRight, * ButtonRInteract, * ButtonSave;
float screenWidth, screenHeight;
char cRightText[7];
char* SaveFile;
int iEditUp, iEditDown, iEditLeft, iEditRight,iEditInteract;


GameObject* options_Sliders[4]; //0 bgm bg, 1 bgm knob, 2 sfx bg, 3 sfx knob
GameObject* Options_HoldSlider;
#define MAX_OPTIONSHIGHLIGHTS 9
Renderer* options_highlight[MAX_OPTIONSHIGHLIGHTS]; //0 bgm, sfx, wsade, save, 9 cross
#define KNOB_HIGHLIGHTCOLOR CP_Color_Create(120,120,120,60);
#define KEYMAP_HIGHLIGHTCOLOR COLOR_YELLOW
#define KEYMAP_STROKEWEIGHT 5.0f

/// <summary>
/// Sets the UI for the buttons.
/// </summary>
void SceneOptionsUI_Buttons();

/// <summary>
/// Assigns the button based on key press.
/// </summary>
void SceneOptions_AssignKeyPress();

/// <summary>
/// Adjusts the slider knob on the sliderBar to value v / 100
/// </summary>
/// <param name="knob - knob of the slider"></param>
/// <param name="sliderBar - bg of the slider bar, also determines the length"></param>
/// <param name="v - value of slider 0 to 100"></param>
void AdjustSlider(GameObject* knob, GameObject* sliderBar, int v);

/// <summary>
/// returns the value of slider knob on sliderBar 0 - 100
/// </summary>
/// <param name="knob - knob of the slider"></param>
/// <param name="sliderBar - bg of the slider bar, also determines the length"></param>
/// <returns>int - value of knob on slider 0 - 100</returns>
int GetSliderValue(GameObject* knob, GameObject* sliderBar);

/// <summary>
/// Handles the collision of the the GameObjects based on the left and right collider parameters.
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void SceneOptions_OnCollision(Collider* left, Collider* right)
{
    //me, other
    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "cross") == 0) {
            SceneManager_ChangeSceneByName("mainmenu");
            SDM_PlaySFX(4);
        }
            
        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonSave") == 0)
        {
            WriteControlsToFile(SaveFile, cRightText, 5);
            SceneManager_ChangeSceneByName("mainmenu");
            SDM_PlaySFX(4);
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonUp") == 0)
        {
            iEditUp = 1;
            iEditDown = 0;
            iEditLeft = 0;
            iEditRight = 0;
            iEditInteract = 0;
            //Will return the correct renderer because it is created first. 
            Renderer* r = RM_GetComponent(ButtonRUp);
            RM_SetText(r, "Enter a key");
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonDown") == 0)
        {
            iEditDown = 1;
            iEditUp = 0;
            iEditLeft = 0;
            iEditRight = 0;
            iEditInteract = 0;
            Renderer* r = RM_GetComponent(ButtonRDown);
            RM_SetText(r, "Enter a key");
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonLeft") == 0)
        {
            iEditLeft = 1;
            iEditUp = 0;
            iEditDown = 0;
            iEditRight = 0;
            iEditInteract = 0;
            Renderer* r = RM_GetComponent(ButtonRLeft);
            RM_SetText(r, "Enter a key");
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonRight") == 0)
        {
            iEditRight = 1;
            iEditUp = 0;
            iEditDown = 0;
            iEditLeft = 0;
            iEditInteract = 0;
            Renderer* r = RM_GetComponent(ButtonRRight);
            RM_SetText(r, "Enter a key");
        }

        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonInteract") == 0)
        {
            iEditInteract = 1;
            iEditUp = 0;
            iEditDown = 0;
            iEditLeft = 0;
            iEditRight = 0;
            Renderer* r = RM_GetComponent(ButtonRInteract);
            RM_SetText(r, "Enter a key");
        }

        else if (strcmp(left->obj->tag, "bgmSlider") == 0)
        {
            Options_HoldSlider = left->obj;
        }
        else if (strcmp(left->obj->tag, "sfxSlider") == 0)
        {
            Options_HoldSlider = left->obj;
        }
    }

    //FOR MOUSE HOVERING OVER
    if (strcmp(((GameObject*)right->obj)->tag, "Mouse") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "ButtonUp") == 0)
        {
            options_highlight[2]->isEnabled = 1;
        }
        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonDown") == 0)
        {
            options_highlight[3]->isEnabled = 1;
        }
        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonLeft") == 0)
        {
            options_highlight[4]->isEnabled = 1;
        }
        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonRight") == 0)
        {
            options_highlight[5]->isEnabled = 1;
        }
        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonInteract") == 0)
        {
            options_highlight[6]->isEnabled = 1;
        }
        else if (strcmp(left->obj->tag, "bgmSlider") == 0)
        {
            options_highlight[0]->isEnabled = 1;
        }
        else if (strcmp(left->obj->tag, "sfxSlider") == 0)
        {
            options_highlight[1]->isEnabled = 1;
        }
        else if (strcmp(((GameObject*)left->obj)->tag, "ButtonSave") == 0)
        {
            options_highlight[7]->isEnabled = 1;
        }
        else if(strcmp(((GameObject*)left->obj)->tag, "cross") == 0)
        {
            options_highlight[8]->isEnabled = 1;
        }
    }
    return;
}

/// <summary>
/// Initialises the variables in this scene.
/// </summary>
void SceneOptions_init(void)
{
    SM_SystemsInit();
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);

    GameObject* bg = GOM_Create2(RECTANGLE,
        CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* bgr = RM_AddComponent(bg);
    bgr->renderPriority = PRI_UI;
    RM_LoadImage(bgr, "Assets/BananaBoi_Title.jpg");

    SaveFile = "Controls/controls.txt";

    Collider* c = NULL;

    //UI Background
    Options_Background = GOM_Create(RECTANGLE);
    Renderer* r = RM_AddComponent(Options_Background);
    r->color = CP_Color_Create(200, 200, 200, 255);
    r->renderPriority = PRI_UI;
    RM_LoadImage(r, "Assets/Backgrounds/objective-light.png");
    Options_Background->scale = CP_Vector_Set(screenWidth * 0.7f, screenHeight * 0.8f);
    Options_Background->position = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f);

    //title
    Options_Title = GOM_Create2(EMPTY, CP_Vector_Set(screenWidth * 0.5f, 135), 0,
        CP_Vector_Set(screenWidth * 0.6f, 180));
    r = RM_AddComponent(Options_Title);
    //r->color.a = 0;
    //r->text = "Options";
    RM_SetText(r, "Options");
    //RM_LoadImage(r, "Assets/Backgrounds/title-light.png");
    r->renderPriority = PRI_UI;
    r->textColor = COLOR_LIGHTYELLOW;
    r->textScale = CP_Vector_Set(3.0f, 3.0f);

    //cross
    Options_Cross = GOM_Create(RECTANGLE);
    Options_Cross->position = CP_Vector_Set(screenWidth * 0.846f, screenHeight * 0.123f);
    Options_Cross->scale = CP_Vector_Set(50, 50);
    Options_Cross->tag = "cross";
    c = CLM_AddComponent(Options_Cross);
    CLM_Set(c, COL_BOX, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;
    // cross unhighlighted
    r = RM_AddComponent(Options_Cross);
    RM_LoadImage(r, "Assets/cross.png");
    r->renderPriority = PRI_UI;
    // cross highlighted
    r = RM_AddComponent(Options_Cross);
    RM_LoadImage(r, "Assets/crosshighlight.png");
    r->renderPriority = PRI_UI;
    options_highlight[8] = r;

    iEditUp = 0;
    iEditDown = 0;
    iEditLeft = 0;
    iEditRight = 0;
    iEditInteract = 0;

    CP_Vector labelStartPos = CP_Vector_Set(screenWidth * 0.365f, screenHeight * 0.275f);
    CP_Vector labelScale = CP_Vector_Set(100, 50);
    CP_Vector labelTextScale = CP_Vector_Set(1.5f, 1.5f);
    CP_Vector startPos = CP_Vector_Set(screenWidth * 0.525f, screenHeight * 0.275f);
    CP_Vector sliderScale = CP_Vector_Set(300, 10);
    CP_Vector knobScale = CP_Vector_Set(30, 30);
    float ygap = 50.0f;
    //music optionss
    GameObject* g = GOM_Create2(RECTANGLE, labelStartPos, 0, labelScale);
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->color.a = 0;
    RM_SetText(r, "Music");
    r->textScale = labelTextScale;
    r->textColor = COLOR_LIGHTYELLOW;
    r->strokeWeight = 0.0f;

    g = GOM_Create2(RECTANGLE, startPos,
        0, sliderScale);
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->color = COLOR_DARKGREY;
    options_Sliders[0] = g;
    
    g = GOM_Create2(CIRCLE, CP_Vector_Set(g->position.x + sliderScale.x * 0.5f, g->position.y),
        0, knobScale);
    g->tag = "bgmSlider";
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    c = CLM_AddComponent(g);
    CLM_Set(c, COL_CIRCLE, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;
    options_Sliders[1] = g;

    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->color = KNOB_HIGHLIGHTCOLOR;
    r->isEnabled = 0;
    options_highlight[0] = r;

    g = GOM_Create2(RECTANGLE, CP_Vector_Set(labelStartPos.x, labelStartPos.y + ygap), 0, labelScale);
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->color.a = 0;
    RM_SetText(r, "SFX");
    r->textColor = COLOR_LIGHTYELLOW;
    r->textScale = labelTextScale;
    r->strokeWeight = 0.0f;

    g = GOM_Create2(RECTANGLE, CP_Vector_Set(startPos.x, startPos.y + ygap),
        0, sliderScale);
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->color = COLOR_DARKGREY;
    options_Sliders[2] = g;

    g = GOM_Create2(CIRCLE, CP_Vector_Set(g->position.x + sliderScale.x * 0.5f, g->position.y),
        0, knobScale);
    g->tag = "sfxSlider";
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    c = CLM_AddComponent(g);
    CLM_Set(c, COL_CIRCLE, SceneOptions_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;
    options_Sliders[3] = g;

    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->color = KNOB_HIGHLIGHTCOLOR;
    r->isEnabled = 0;
    options_highlight[1] = r;
    
    Options_HoldSlider = NULL;

    //Loads and set volume IN SDM
    SceneOptionsUI_Buttons();

    AdjustSlider(options_Sliders[1], options_Sliders[0], SDM_GetBGVolume());
    AdjustSlider(options_Sliders[3], options_Sliders[2], SDM_GetSFXVolume());
}

/// <summary>
/// Update.
/// </summary>
void SceneOptions_update(void)
{
    SM_SystemsPreUpdate();

    //Options_HoldSlider = NULL;
    SceneOptions_AssignKeyPress();

    {
        float mouseX = CP_Input_GetMouseWorldX();
        float mouseY = CP_Input_GetMouseWorldY();

        GameObject* tempMouse = GOM_CreateTemp(EMPTY);
        tempMouse->position = CP_Vector_Set(mouseX, mouseY);
        //tempMouse->scale = CP_Vector_Set(10, 10);
        tempMouse->tag = "Mouse";
        Collider* mc = CLM_AddComponent(tempMouse);
        CLM_Set(mc, COL_POINT, NULL);
        mc->space = COLSPC_SCREEN;
        mc->isTrigger = 1;
        
        //force bgm sfx to always reset highlight, and only reset keymaps if they are not being edited
        int temp[MAX_OPTIONSHIGHLIGHTS] = { 0,0,iEditUp, iEditDown, iEditLeft, iEditRight, iEditInteract };
        for (int i = 0; i < MAX_OPTIONSHIGHLIGHTS; ++i)
            options_highlight[i]->isEnabled = temp[i];
    }

    SM_SystemsUpdate(0);

    //After update collision, holdSlider may be not null
    if (Options_HoldSlider)
    {
        if (CP_Input_MouseReleased(MOUSE_BUTTON_1))
        {
            Options_HoldSlider = NULL;
        }
        else
        {
            float mouseX = CP_Input_GetMouseWorldX();
            //float mouseY = CP_Input_GetMouseWorldY();
            CP_Vector defaultPos = Options_HoldSlider->position;

            CP_Vector bgPos = options_Sliders[0]->position;
            CP_Vector bgScale = options_Sliders[0]->scale;
            char* save = &cRightText[5];
            if (Options_HoldSlider == options_Sliders[3])
            {
                //is sfxslider
                bgPos = options_Sliders[2]->position;
                bgScale = options_Sliders[2]->scale;
                save = &cRightText[6];
            }

            mouseX = CP_Math_ClampFloat(mouseX, bgPos.x - bgScale.x * 0.5f, bgPos.x + bgScale.x * 0.5f);
            Options_HoldSlider->position = CP_Vector_Set(mouseX, defaultPos.y);
            
            int v = (int)(((mouseX - (bgPos.x - bgScale.x * 0.5f)) / bgScale.x) * 100);
            *save = (char)v;

            if (save == &cRightText[5]) //isfor bgm
                SDM_SetBGVolume(v);
            else if (save == &cRightText[6]) //isfor sfx
                SDM_SetSFXVolume(v);
        }
    }

    SM_SystemsLateUpdate();
}

/// <summary>
/// Exit.
/// </summary>
void SceneOptions_exit(void)
{
    SM_SystemsExit();
}

/// <summary>
/// Initialize function for the SceneManager.
/// </summary>
void SceneOptions_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = SceneOptions_init;
    *update = SceneOptions_update;
    *exit = SceneOptions_exit;
}

/// <summary>
/// Handles the UI for the Buttons.
/// </summary>
void SceneOptionsUI_Buttons()
{
    ReadControlsFromFile(SaveFile, cRightText);
    SDM_SetBGVolume((int)cRightText[5]);
    SDM_SetSFXVolume((int)cRightText[6]);


	CP_Vector ButtonScale = CP_Vector_Set(screenWidth * 0.125f, screenHeight * 0.06125f);
    float startY = screenHeight * 0.425f;
    float incY = 50.f;
	CP_Vector Button_One_Left = CP_Vector_Set(screenWidth * 0.4f, startY);
	CP_Vector ConnectorPositio_Two_Left = CP_Vector_Set(screenWidth * 0.4f, startY + incY * 1.0f);
	CP_Vector Button_Three_Left = CP_Vector_Set(screenWidth * 0.4f, startY + incY * 2.0f);
    CP_Vector Button_Four_Left = CP_Vector_Set(screenWidth * 0.4f, startY + incY * 3.0f);
    CP_Vector Button_Five_Left = CP_Vector_Set(screenWidth * 0.4f, startY + incY * 4.0f);

	CP_Vector Button_One_Right = CP_Vector_Set(screenWidth * 0.6f, startY);
	CP_Vector ConnectorPositio_Two_Right = CP_Vector_Set(screenWidth * 0.6f, startY + incY * 1.0f);
	CP_Vector Button_Three_Right = CP_Vector_Set(screenWidth * 0.6f, startY + incY * 2.0f);
    CP_Vector Button_Four_Right = CP_Vector_Set(screenWidth * 0.6f, startY + incY * 3.0f);
    CP_Vector Button_Five_Right = CP_Vector_Set(screenWidth * 0.6f, startY + incY * 4.0f);

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

    r = RM_AddComponent(ButtonRUp);
    r->renderPriority = PRI_UI;
    r->color.a = 0; 
    r->strokeColor = KEYMAP_HIGHLIGHTCOLOR; r->strokeWeight = KEYMAP_STROKEWEIGHT;
    r->isEnabled = 0;
    options_highlight[2] = r;

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

    r = RM_AddComponent(ButtonRDown);
    r->renderPriority = PRI_UI;
    r->color.a = 0;
    r->strokeColor = KEYMAP_HIGHLIGHTCOLOR; r->strokeWeight = KEYMAP_STROKEWEIGHT;
    r->isEnabled = 0;
    options_highlight[3] = r;

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

    r = RM_AddComponent(ButtonRLeft);
    r->renderPriority = PRI_UI;
    r->color.a = 0;
    r->strokeColor = KEYMAP_HIGHLIGHTCOLOR; r->strokeWeight = KEYMAP_STROKEWEIGHT;
    r->isEnabled = 0;
    options_highlight[4] = r;

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

    r = RM_AddComponent(ButtonRRight);
    r->renderPriority = PRI_UI;
    r->color.a = 0;
    r->strokeColor = KEYMAP_HIGHLIGHTCOLOR; r->strokeWeight = KEYMAP_STROKEWEIGHT;
    r->isEnabled = 0;
    options_highlight[5] = r;

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

    r = RM_AddComponent(ButtonRInteract);
    r->renderPriority = PRI_UI;
    r->color.a = 0;
    r->strokeColor = KEYMAP_HIGHLIGHTCOLOR; r->strokeWeight = KEYMAP_STROKEWEIGHT;
    r->isEnabled = 0;
    options_highlight[6] = r;

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

    r = RM_AddComponent(ButtonSave);
    r->renderPriority = PRI_UI;
    r->color = KNOB_HIGHLIGHTCOLOR;
    r->color.a = 100;
    r->isEnabled = 0;
    options_highlight[7] = r;
}

/// <summary>
/// Assigns the button based on key press.
/// </summary>
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

/// <summary>
/// Adjusts the slider knob on the sliderBar to value v / 100
/// </summary>
/// <param name="knob - knob of the slider"></param>
/// <param name="sliderBar - bg of the slider bar, also determines the length"></param>
/// <param name="v - value of slider 0 to 100"></param>
/// <param name="">v - value of slider 0 to 100</param>
void AdjustSlider(GameObject* knob, GameObject* sliderBar, int v)
{
    float percentage = v / 100.0f;
    float minx = sliderBar->position.x - sliderBar->scale.x * 0.5f;
    float len = sliderBar->scale.x;

    knob->position = CP_Vector_Set(minx + percentage * len, sliderBar->position.y);
}

/// <summary>
/// returns the value of slider knob on sliderBar 0 - 100
/// </summary>
/// <param name="knob - knob of the slider"></param>
/// <param name="sliderBar - bg of the slider bar, also determines the length"></param>
/// <returns>int - value of knob on slider 0 - 100</returns>
int GetSliderValue(GameObject* knob, GameObject* sliderBar)
{
    float minx = sliderBar->position.x - sliderBar->scale.x * 0.5f;
    float len = sliderBar->scale.x;
    float v = knob->position.x - minx;
    return (int)(v / len);
}
