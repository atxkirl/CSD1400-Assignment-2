/*
@file	Dialogue.h
@author	Adrian Tan (t.xingkhiangadrian)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@date	07/11/2021
@brief	Contains functions to display and handle dialogue boxes in the game.
*//*______________________________________________________________________*/

#pragma once

#include <stdio.h>
#include <string.h>
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"

typedef enum
{
	DIALOGUE_LOOKINGAT, // For dialogue that should only appear when the Player is looking directly at an object. (ie, "Press 'E' to pick up.")
	DIALOGUE_CLOSEBUTTON, // For dialogue that should have a close button that can be clicked to close the dialogue box. (ie, a signpost)
	DIALOGUE_PRESSFORNEXT // For dialogue that requires a keypress or mouseclick to close, or move to next line. (ie, NPC talking)
}DialogueType;

// Text
static const float dialogueTextScale = 1.f;
static const float dialogueTextWidth = 10.f; // Hardcoded value for the width of a single text character at 1.f scale... :(
// Box current size.
static float dialogueBoxHeight = 100.f;
static float dialogueBoxWidth = 200.f;
static const float dialogueBoxMinWidth = 200.f;
// Close Button
static const float closeButtonDiameter = 20.f;

DialogueType dialogueType;

// Dialogue Box
GameObject* dialogueBox; // This will be the "singleton" gameobject in the world that represents the dialogue box.
Renderer* dialogueBoxRenderer; // This is the Renderer component attached to the dialogueObject.

// Close Button
GameObject* closeButton;
Renderer* closeButtonRenderer;
Renderer* closeButtonHighlight;
Collider* closeButtonCollider;

/// <summary>
/// Handling function for when dialoguebox close button is clicked.
/// </summary>
/// <param name="left">Collider for mouse click.</param>
/// <param name="right">Collider for close button.</param>
static void DM_Handle_CloseButton(Collider* left, Collider* right);

/// <summary>
/// Displays a dialogue box with the provided text.
/// </summary>
/// <param name="text =">Text to display.</param>
/// <param name="type =">Type of Dialogue box.</param>
void DM_PrintDialogue(char* text, DialogueType type);

/// <summary>
/// Displays a dialogue box with the provided text.
/// </summary>
/// <param name="text =">Text to display.</param>
/// <param name="type =">Type of Dialogue box.</param>
/// <param name="color =">Color of the text.</param>
void DM_PrintDialogueColored(char* text, DialogueType type, CP_Color color);

/// <summary>
/// Closes any open dialogue boxes.
/// </summary>
void DM_CloseDialogueBox();

/// <summary>
/// Initializes dialogue box variables.
/// </summary>
void DM_Init();

/// <summary>
/// Clears any dynamically allocated memory upon overall game shutdown.
/// </summary>
void DM_Clear();

/// <summary>
/// Update loop for dialogue, called every frame.
/// </summary>
void DM_Update();

/// <summary>
/// Late update loop for dialogue, called at the end of every frame.
/// </summary>
void DM_LateUpdate();