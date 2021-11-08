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
Collider* closeButtonCollider;

static void DM_Handle_CloseButton(Collider* left, Collider* right);

static void DM_Handle_PressForNext();

void DM_PrintDialogue(char* text, DialogueType type);

void DM_PrintDialogueColored(char* text, DialogueType type, CP_Color color);

void DM_Init();

void DM_Clear();

void DM_Update();

void DM_LateUpdate();