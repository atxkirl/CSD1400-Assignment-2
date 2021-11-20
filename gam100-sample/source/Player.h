//Prefix: PLY

#include <stdlib.h>
#include <stdio.h>

// creates player gameobject. Will take in values to input where to generate the player
GameObject* PLY_CreatePlayer(float x, float y);

// controls the player and check for updates
void PLY_Update();

int PLY_IsHidden(void);
int PLY_IsInvincible(void);