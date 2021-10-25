
#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "../DetectMemoryLeak.h"
#include "SystemManager.h"
#include "Player.h"

#include <time.h>
#include <stdlib.h>

GameObject* player = NULL;
Renderer* render = NULL;
int p_invincible = 0;
time_t startTime;


/*
@brief Handles invincibility counter to ensure that player will not take damage when invincible.
*/

void counter(void) {
    time_t Current_time = clock();
    if (difftime(Current_time, startTime) >= 5000.0f) {
        p_invincible = 0;
        printf("timer over");
    }
}

int Player_OnCollision(Collider* left, Collider* right) 
{
    while (p_invincible == 0) {
        if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "enemy") == 0) {
            printf("hit");
            startTime = clock();
            p_invincible = 1;
        }
    }
    return CLM_RESPONSE_REMOVENONE;
}

void PLY_CreatePlayer(){

    player = GOM_Create2(RECTANGLE, CP_Vector_Set(50, 90), 0.0f, CP_Vector_Set(50, 50));
    player->tag ="player";
    render = RM_AddComponent(player);
    RM_LoadImage(render, "Assets/bananaboi.png");
    CLM_Set(CLM_AddComponent(player),COL_BOX,Player_OnCollision);
} 

void PLY_Update(){ // handles input from player and checking for flags


    /*if (CP_Input_KeyDown(KEY_W))
    {
        CP_Vector up = CP_Vector_Set(0.0f, -10.0f);
        player->position = CP_Vector_Add(player->position, up);
    }
    if (CP_Input_KeyDown(KEY_A))
    {
        CP_Vector left = CP_Vector_Set(-10.0f, 0.0f);
        player->position = CP_Vector_Add(player->position, left);
    }
    if (CP_Input_KeyDown(KEY_S))
    {
        CP_Vector down = CP_Vector_Set(0.0f, 10.0f);
        player->position = CP_Vector_Add(player->position, down);
    }
    if (CP_Input_KeyDown(KEY_D))
    {
        CP_Vector right = CP_Vector_Set(10.0f, 0.0f);
        player->position = CP_Vector_Add(player->position, right);
    }*/

    float dt = CP_System_GetDt();
    float spd = 200.0f;

    if (CP_Input_KeyDown((enum CP_KEY)KEY_W))
    {
        player->position.y -= spd * dt;
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_S))
    {
        player->position.y += spd * dt;
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_A))
    {
        player->position.x -= spd * dt;
    }
    if (CP_Input_KeyDown((enum CP_KEY)KEY_D))
    {
        player->position.x += spd * dt;
    }

    if (p_invincible == 1)
    {
        counter();
    }

    RM_SetCameraPosition(player->position);
}