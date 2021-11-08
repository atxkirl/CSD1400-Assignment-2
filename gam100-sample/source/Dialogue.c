#include "Dialogue.h"

void DM_Handle_CloseButton(Collider* left, Collider* right)
{
	printf("Clicked!\n");
	if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
	{
		if (strcmp(((GameObject*)left->obj)->tag, "close") == 0)
		{
			dialogueBox->isEnabled = 0;
			closeButton->isEnabled = 0;
			closeButtonCollider->isEnabled = 0;
		}
	}
}

void DM_Handle_PressForNext()
{
	
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
	dialogueBoxRenderer->text = text;
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

void DM_Init()
{
	// Create and register the dialogue box's GameObject and Renderer.
	dialogueBox = GOM_Create2(RECTANGLE, CP_Vector_Set(CP_System_GetWindowWidth() / 2.f, CP_System_GetWindowHeight() / 2.f), 0.f, CP_Vector_Set(dialogueBoxWidth, dialogueBoxHeight));
	
	dialogueBoxRenderer = RM_AddComponent(dialogueBox);
	dialogueBoxRenderer->textColor = CP_Color_Create(255, 255, 255, 255);
	dialogueBoxRenderer->renderPriority = PRI_UI;
	dialogueBoxRenderer->textScale = CP_Vector_Set(dialogueTextScale, dialogueTextScale);
	//RM_LoadImage(dialogueBoxRenderer, "Assets/testbuttonbackground.jpg");

	// Create and register the close button's GameObject, Renderer and Collider.
	closeButton = GOM_Create2(CIRCLE, dialogueBox->position, 0.f, CP_Vector_Set(closeButtonDiameter, closeButtonDiameter));
	closeButton->position.x += (dialogueBoxWidth * 0.5f);
	closeButton->position.y -= (dialogueBoxHeight * 0.5f);
	closeButton->tag = "close";

	closeButtonRenderer = RM_AddComponent(closeButton);
	closeButtonRenderer->renderPriority = PRI_UI;
	closeButtonRenderer->color = CP_Color_Create(255, 0, 0, 255);
	
	closeButtonCollider = CLM_AddComponent(closeButton);
	CLM_Set(closeButtonCollider, COL_CIRCLE, DM_Handle_CloseButton);
	
	// Disable all dialogue elements first.
	dialogueBox->isEnabled = 0;
	closeButton->isEnabled = 0;
}

void DM_Clear()
{
	
}

void DM_Update()
{
	GameObject* clickPoint = NULL;
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1))
	{
		//Creates a point obj to test collision against button
		clickPoint = GOM_CreateTemp(EMPTY);//param doesnt matter
		clickPoint->position = CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY());
		clickPoint->tag = "Click";
		CLM_Set(CLM_AddComponent(clickPoint), COL_POINT, NULL);
	}
}

void DM_LateUpdate()
{
	if (dialogueType == DIALOGUE_LOOKINGAT)
		dialogueBox->isEnabled = 0;
}