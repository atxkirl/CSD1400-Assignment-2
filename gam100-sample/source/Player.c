
#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "DetectMemoryLeak.h"
#include "SystemManager.h"
#include "Player.h"
#include "FileParser.h"
#include "SoundManager.h"

#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Controls.h"

GameObject* player = NULL;
Renderer* render = NULL;
time_t startTime;


// Player Parameters

float spd;
int playerhealth;
float n_weight = 0;
float weight = 50.0f;

// Insert all flags that are needed below
bool p_Slowed = false;
bool p_Hidden = false;
bool p_Invincible = false;
bool p_Hideable = false;
bool g_objective1 = false, g_objective2 = false , g_objective3 = false, g_objective4 = false, g_objective5 = false;
bool g_object1collect = false, g_object1drop = false, g_object1comp = false;
bool p_walk = false;

/*
@brief Handles invincibility counter to ensure that player will not take damage when invincible.
*/

void counter(void) {
    time_t Current_time = clock();

    // ensures that at least 5 seconds have passsed before player becomes not invincible again. 
    if (difftime(Current_time, startTime) >= 5000.0f) {
        p_Invincible = false;
        printf("timer over");
    }
}

// collider 

void Player_OnCollision(Collider* left, Collider* right)
{
    if (p_Invincible == 0) {
        while (1) {
            // Checks for enemy hits
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "BBEM") == 0) {
                printf("hit");
                startTime = clock(); //holds information on when the player was last hit according to the time
                p_Invincible = true;
                playerhealth--;
                break;
            }

            // Pick-up minigame
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "wood") == 0) {
                //g_objective1 = true;
                n_weight += 1;
                g_object1collect = true;
                break;
            }

            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "wooddrop") == 0) {
                if (g_object1comp == true) {
                    DM_PrintDialogue("You have completed the objective.", DIALOGUE_CLOSEBUTTON);
                }
                else {
                    DM_PrintDialogue("Press E to drop wood.", DIALOGUE_CLOSEBUTTON);
                    g_object1drop = true;
                }
                break;
            }

            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj3") == 0) { // placeholders for completing objectives
                g_objective3 = true;
                break;
            }
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj4") == 0) {
                g_objective4 = true;
                break;
            }
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj5") == 0) {
                g_objective5 = true;
                break;
            }

            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "mud") == 0) { // testing collisions currently not working to slow player down
                n_weight = 1;
                printf("SLOWED!");
                break;
            }

            if (strcmp(left->obj->tag, "player") == 0 && right->obj->type == CORAL)
            {
                p_Hideable = true;
                break;
            }

            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "box") == 0)
            {
                p_Hideable = true;
                break;
            }

            else
            {
                p_Hideable = false;
                break;
            }

        } 
    }
    else {
        while (1) {
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj1") == 0) {  // placeholders for completing objectives
                g_objective1 = true;
                printf("objective 1 complete!");
                break;
            }
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj2") == 0) {
                g_objective2 = true;
                break;
            }
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj3") == 0) {
                g_objective3 = true;
                break;
            }
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj4") == 0) {
                g_objective4 = true;
                break;
            }
            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "obj5") == 0) {
                g_objective5 = true;
                break;
            }

            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "mud") == 0) { // testing collisions currently not working to slow player down
                spd = 100.0f;
                printf("SLOWED!");
                break;
            }

            if (strcmp(left->obj->tag, "player") == 0 && right->obj->type == CORAL)
            {
                p_Hideable = true;
                break;
            }

            if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "box") == 0)
            {
                p_Hideable = true;
                break;
            }

            else
            {
                p_Hideable = false;
                break;
            }
        }
    }

}

GameObject* PLY_CreatePlayer(float x, float y) {

    player = GOM_Create2(RECTANGLE, CP_Vector_Set(x,y), 0.0f, CP_Vector_Set(50, 50)); //makes the player object
    player->tag ="player";
    render = RM_AddComponent(player);
    render->renderPriority = PRI_PLY;
    RM_LoadImage(render, "Assets/bananaboi.png");
    CLM_Set(CLM_AddComponent(player),COL_BOX,Player_OnCollision);
    Animation* a = AM_AddComponent(player);
    AM_SetWalk(a);

    playerhealth = 3;

    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    GameObject* temp = GOM_Create2(RECTANGLE, CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f), 0, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* r = RM_AddComponent(temp);
    RM_LoadImage(r, "Assets/fow.png");
    r->renderPriority = PRI_UI;

    p_Slowed = false;
    p_Hidden = false;
    p_Invincible = false;
    p_Hideable = false;
    g_objective1 = g_objective2 = g_objective3 = g_objective4 = g_objective5 = false;
    g_object1collect = g_object1drop = g_object1comp = false;

    return player;
} 
    
void PLY_Update() { // handles input from player and checking for flags

    float dt = CP_System_GetDt();

    float currentSpd = 200.0f - (n_weight * weight);

    if (p_Hidden == false) {
        //  player controls
        if (CP_Input_KeyDown((CP_KEY)cControls->cUp)) {
            player->position.y -= currentSpd * dt;
        }
        // up
        if (CP_Input_KeyDown((CP_KEY)cControls->cLeft)) player->position.x -= currentSpd * dt; // left

        if (CP_Input_KeyDown((CP_KEY)cControls->cDown)) player->position.y += currentSpd * dt; // down

        if (CP_Input_KeyDown((CP_KEY)cControls->cRight)) player->position.x += currentSpd * dt; // right


        if (render)
            render->color.a = 255;
    }
    else
    {
        if (render)
            render->color.a = 122;
    }
     
    // update and checks for invincibility
    if (p_Invincible == 1) counter();

    //checks for health
    if (playerhealth == 0) SM_DeleteGameObject(player);

    //checks for whether player can hide or not 
    if (p_Hidden == false)
    {
        if (p_Hideable == true)
        {
            if (CP_Input_KeyTriggered((CP_KEY)cControls->cInteract)) {
                //player->isEnabled = 0;
                p_Hidden = true;
            }
        }
    }
   

    else {
        if (CP_Input_KeyTriggered((CP_KEY)cControls->cInteract)) {
            //player->isEnabled = 1;
            p_Hidden = false;
        }

        //if (p_Hidden == true) {

        //}
    }


    //RESET FLAG FOR COLLISION USE LATER
    p_Hideable = false;

    //check if player reach drop off
    if (g_object1drop == true) {
        if (CP_Input_KeyTriggered((CP_KEY)cControls->cInteract)) {
            if (g_object1collect == true) {
                g_object1collect = false;
                g_object1comp = true;
                n_weight -= 1;
                DM_PrintDialogue("You dropped off the wood.", DIALOGUE_CLOSEBUTTON);
                printf("objective 1 complete!");
            }
            else if (g_object1comp == true) {
                DM_PrintDialogue("You have completed the objective.", DIALOGUE_CLOSEBUTTON);
            }
            else {
                DM_PrintDialogue("Boi where your wood, go get it", DIALOGUE_CLOSEBUTTON);
            }
        }
    }


    switch (playerhealth) {
    case 3: {

        break;
    }
    case 2: {
        break;
    }
    case 1:{
        break;
    }
    default: {
        break;
    }
    }

    // returns player back to normal speed when 
    //spd = 200.0f;
    p_Hideable = false;
    RM_SetCameraPosition(player->position);
}

int PLY_IsHidden(void)
{
    return p_Hidden;
}

int PLY_IsInvincible(void)
{
    return p_Invincible;
}