/*
@copyright	All content © 2021 DigiPen Institute of Technology Singapore, all rights reserved.
@file	Player.h
@author	Tan Wee Yi (weeyi.t)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@brief	Script that will handle the player functionality, and all the updates that
is required
*//*______________________________________________________________________*/

//Prefix: PLY

#include <stdlib.h>
#include <stdio.h>

/// <summary>
/// Creates the player game object and preps all the required integration to the different systems
/// in the game.
/// </summary>
/// <param name="x">Render x position of the game object.</param>
/// <param name="y">Render y position of the game object.</param>
/// <returns>Returns a pointer to the game object that is created for storage in linked lists </returns>
GameObject* PLY_CreatePlayer(float x, float y);

/// <summary>
/// Function contains the required updates that the player game object needs in order to 
/// function in the game. These include the movement of the player object, the invincibility
/// and all the sound effects that are tied to the player.
/// </summary>
void PLY_Update();

/// <summary>
/// Returns information on whether the player is hidden in a hideable object.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int PLY_IsHidden(void);

/// <summary>
/// Returns information on if the player is invincible or not.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int PLY_IsInvincible(void);

/// <summary>
/// Handles player damage taking, and updates information on when the player has taken damage,
/// to allow the invincibility timer to run.
/// </summary>
/// <param name=""></param>
/// <returns>True/False on if the player has taken damage.</returns>
bool PLY_TakeDamage(void);

/// <summary>
/// handles showing the interact hint for players 
/// </summary>
/// <param name=""></param>
void PLY_ShowInteractHint(void);