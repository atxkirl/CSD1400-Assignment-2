
#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "../DetectMemoryLeak.h"
#include "SystemManager.h"
#include "Player.h"

#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

GameObject* player = NULL;
Renderer* render = NULL;
time_t startTime;


// Player Parameters

float spd = 200.0f;
int playerhealth;

// Insert all flags that are needed below

bool p_Slowed = false;
bool p_Hideable = false;
bool p_invincible = false;
bool g_objective1 = false, g_objective2 = false , g_objective3 = false, g_objective4 = false, g_objective5 = false;


/*
@brief Handles invincibility counter to ensure that player will not take damage when invincible.
*/

void counter(void) {
    time_t Current_time = clock();
    if (difftime(Current_time, startTime) >= 5000.0f) {
        p_invincible = false;
        printf("timer over");
    }
}

// collider 

void Player_OnCollision(Collider* left, Collider* right)
{
    while (p_invincible == 0) {
        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "enemy") == 0) {
            printf("hit");
            startTime = clock();
            p_invincible = true;
            playerhealth--;
        }

        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj1") == 0) { // placeholders for completing objectives
            g_objective1 = true;
            break;
        }
        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj2") == 0) { 
            g_objective2 = true;
            break;
        }
        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj3") == 0) { 
            g_objective3 = true;
        }
        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj4") == 0) { 
            g_objective4 = true;
        }
        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj5") == 0) { 
            g_objective5 = true;
        }

        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "mud") == 0) { // testing collisions currently not working to slow player down
            spd = 100.0f;
            printf("SLOWED!");
            break;
        }

        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "box") == 0)
        {
            p_Hideable = true;
        }

        else p_Hideable = false;

    }
}

void PLY_CreatePlayer() {

    player = GOM_Create2(RECTANGLE, CP_Vector_Set(50, 90), 0.0f, CP_Vector_Set(50, 50));
    player->tag ="player";
    render = RM_AddComponent(player);
    RM_LoadImage(render, "Assets/bananaboi.png");
    CLM_Set(CLM_AddComponent(player),COL_BOX,Player_OnCollision);

    playerhealth = 3;

    // add hearts generation
} 
    
void PLY_Update() { // handles input from player and checking for flags

    float dt = CP_System_GetDt();
    

    if (CP_Input_KeyDown((enum CP_KEY)KEY_W)) player->position.y -= spd * dt;

    if (CP_Input_KeyDown((enum CP_KEY)KEY_S)) player->position.y += spd * dt;

    if (CP_Input_KeyDown((enum CP_KEY)KEY_A)) player->position.x -= spd * dt;
   
    if (CP_Input_KeyDown((enum CP_KEY)KEY_D)) player->position.x += spd * dt;
  
    if (p_invincible == 1) counter();

    if (playerhealth == 0) SM_DeleteGameObject(player);

    if (p_Hideable == true)
    {
        if (CP_Input_KeyDown((enum CP_KEY)KEY_E)) {
            // add command to hide charcter
        }
    }

    RM_SetCameraPosition(player->position);
}