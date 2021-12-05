/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file		Dialogue.c
@author		Adrian Tan (t.xingkhiangadrian)
@course		CSD1400 Software Engineering Project 1
@Team		BananaBoi
@date		07/11/2021
@brief		Contains functions to display and handle dialogue boxes in the game.
*//*________________________________________________________________________________________*/

#include "Dialogue.h"
#include "Colors.h"

/// <summary>
/// Handling function for when dialoguebox close button is clicked.
/// </summary>
/// <param name="left">Collider for mouse click.</param>
/// <param name="right">Collider for close button.</param>
void DM_Handle_CloseButton(Collider* left, Collider* right)
{
	printf("Handle Left = [%-20s] Right = [%-20s]\n", left->obj->tag, right->obj->tag);
	if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
	{
		if (strcmp(((GameObject*)left->obj)->tag, "close") == 0)
		{
			dialogueBox->isEnabled = 0;
			closeButton->isEnabled = 0;
			closeButtonCollider->isEnabled = 0;
		}
	}
	if (strcmp(((GameObject*)right->obj)->tag, "Mouse") == 0)
	{
		if (strcmp(((GameObject*)left->obj)->tag, "close") == 0)
		{
			closeButtonHighlight->isEnabled = 1;
		}
	}
}

/// <summary>
/// Displays a dialogue box with the provided text.
/// </summary>
/// <param name="text =">Text to display.</param>
/// <param name="type =">Type of Dialogue box.</param>
void DM_PrintDialogue(char* text, DialogueType type)
{
	// Don't do anything if there's already a dialogue box open.
	if (dialogueBox->isEnabled == 1)
		return;

	if (type == DIALOGUE_CLOSEBUTTON)
	{
		closeButton->isEnabled = 1;
		closeButtonCollider->isEnabled = 1;
	}

	// Scale the width of the dialogue box by the length of text
	dialogueBoxWidth = (strlen(text) * dialogueTextWidth);
	if (dialogueBoxWidth < dialogueBoxMinWidth)
		dialogueBoxWidth = dialogueBoxMinWidth;

	dialogueBoxRenderer->width = (int)dialogueBoxWidth;
	dialogueBoxRenderer->height = (int)dialogueBoxHeight;
	dialogueBox->scale = CP_Vector_Set(dialogueBoxWidth, dialogueBoxHeight);

	dialogueType = type;
	RM_SetText(dialogueBoxRenderer, text);
	dialogueBox->isEnabled = 1;
}

/// <summary>
/// Displays a dialogue box with the provided text.
/// </summary>
/// <param name="text =">Text to display.</param>
/// <param name="type =">Type of Dialogue box.</param>
/// <param name="color =">Color of the text.</param>
void DM_PrintDialogueColored(char* text, DialogueType type, CP_Color color)
{
	dialogueBoxRenderer->textColor = color;
	DM_PrintDialogue(text, type);
}

/// <summary>
/// Closes any open dialogue boxes.
/// </summary>
void DM_CloseDialogueBox()
{
	dialogueBox->isEnabled = 0;
	closeButton->isEnabled = 0;
}


/// <summary>
/// Initializes dialogue box variables.
/// </summary>
void DM_Init()
{
	// Create and register the dialogue box's GameObject and Renderer.
	dialogueBox = GOM_Create2(RECTANGLE, CP_Vector_Set(CP_System_GetWindowWidth() / 2.f, CP_System_GetWindowHeight() / 2.f), 0.f, CP_Vector_Set(dialogueBoxWidth, dialogueBoxHeight));
	
	dialogueBoxRenderer = RM_AddComponent(dialogueBox);
	dialogueBoxRenderer->color = CP_Color_Create(115, 62, 58, 255);
	dialogueBoxRenderer->textColor = COLOR_LIGHTYELLOW;
	dialogueBoxRenderer->renderPriority = PRI_UI;
	dialogueBoxRenderer->textScale = CP_Vector_Set(dialogueTextScale, dialogueTextScale);
	//RM_LoadImage(dialogueBoxRenderer, "Assets/Backgrounds/background-light.png");

	// Create and register the close button's GameObject, Renderer and Collider.
	closeButton = GOM_Create2(CIRCLE, dialogueBox->position, 0.f, CP_Vector_Set(closeButtonDiameter, closeButtonDiameter));
	closeButton->position.x += (dialogueBoxWidth * 0.5f);
	closeButton->position.y -= (dialogueBoxHeight * 0.5f);
	closeButton->tag = "close";

	closeButtonCollider = CLM_AddComponent(closeButton);
	CLM_Set(closeButtonCollider, COL_CIRCLE, DM_Handle_CloseButton);
	closeButtonCollider->space = COLSPC_SCREEN;

	closeButtonRenderer = RM_AddComponent(closeButton);
	closeButtonRenderer->renderPriority = PRI_UI;
	RM_LoadImage(closeButtonRenderer, "Assets/cross.png");

	closeButtonHighlight = RM_AddComponent(closeButton);
	closeButtonHighlight->renderPriority = PRI_UI;
	RM_LoadImage(closeButtonHighlight, "Assets/crosshighlight.png");
	closeButtonHighlight->isEnabled = 0;
	
	// Disable all dialogue elements first.
	dialogueBox->isEnabled = 0;
	closeButton->isEnabled = 0;
}

/// <summary>
/// Clears any dynamically allocated memory upon overall game shutdown.
/// </summary>
void DM_Clear()
{
	
}

/// <summary>
/// Update loop for dialogue, called every frame.
/// </summary>
void DM_Update()
{
	// Disable highlight at the start of the frame
	closeButtonHighlight->isEnabled = 0;

	// Keep a collider following the mouse cursor if dialogue box is open.
	float mouseX = CP_Input_GetMouseWorldX();
	float mouseY = CP_Input_GetMouseWorldY();
	GameObject* mouse = GOM_CreateTemp(EMPTY);
	mouse->position = CP_Vector_Set(mouseX, mouseY);
	mouse->scale = CP_Vector_Set(10.f, 10.f);
	mouse->tag = "Mouse";
	Collider* mouseCollider = CLM_AddComponent(mouse);
	CLM_Set(mouseCollider, COL_POINT, NULL);
	mouseCollider->space = COLSPC_SCREEN;
	mouseCollider->isTrigger = 1;
}

/// <summary>
/// Late update loop for dialogue, called at the end of every frame.
/// </summary>
void DM_LateUpdate()
{
	if (dialogueType == DIALOGUE_LOOKINGAT)
		dialogueBox->isEnabled = 0;
}