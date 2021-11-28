
#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "DetectMemoryLeak.h"
#include "SystemManager.h"
#include "Player.h"
#include "FileParser.h"
#include "SoundManager.h"
#include "SceneManager.h"

#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Controls.h"

GameObject* player = NULL;
GameObject* player_fogofwar = NULL;
GameObject* player_invulBubble = NULL;
GameObject* heart1 = NULL;
GameObject* heart2 = NULL;
GameObject* heart3 = NULL;
#define RENDER_COUNT 8
//0 = face back, 1 = face front, 2 = face left, 3 = face right,
//4 = back spritesheet, 5 = front spritesheet, 6 = left spritesheet, 7 = rightspritesheet
Renderer* render[RENDER_COUNT] = { NULL };
time_t startTime;
#define PLY_SPRITE_FPS 3


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

GameObject* ply_interactHint;
#define PLY_HINTOFFSET CP_Vector_Set(25,-30)

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
            //// Checks for enemy hits
            //if (strcmp(left->obj->tag, "player") == 0 && strcmp(right->obj->tag, "BBEM") == 0) {
            //    printf("hit");
            //    startTime = clock(); //holds information on when the player was last hit according to the time
            //    p_Invincible = true;
            //    playerhealth--;
            //    break;
            //}

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

            if (strcmp(left->obj->tag, "player") == 0 && (right->obj->type == CORAL || right->obj->type == GRASS))
            {
                //p_Hideable = true;
                p_Hidden = true;
                AM_GetComponent(player_fogofwar)->loopDir = 1;
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

            if (strcmp(left->obj->tag, "player") == 0 && (right->obj->type == CORAL || right->obj->type == GRASS)) // Auto hide the Player when they're in Tall Grass / Coral.
            {
                //p_Hideable = true;
                AM_GetComponent(player_fogofwar)->loopDir = 1;
                //p_Hideable = true;
                p_Hidden = true;
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

    CLM_Set(CLM_AddComponent(player),COL_BOX,Player_OnCollision);
    Animation* a = AM_AddComponent(player);
    AM_SetWalk(a);

    //Load back image of player
    render[0] = RM_AddComponent(player);
    render[0]->renderPriority = PRI_PLY;
    RM_LoadImage(render[0], "Assets/bananaboi/bananaboi-back.png");

    render[1] = RM_AddComponent(player);
    render[1]->renderPriority = PRI_PLY;
    RM_LoadImage(render[1], "Assets/bananaboi/bananaboi-front.png");
    //todo load the rest
    //...

    //loading right sprite sheet
    render[7] = RM_AddComponent(player);
    render[7]->renderPriority = PRI_PLY;
    RM_LoadImage(render[7], "Assets/bananaboi/bananaboi-side-sprite.png");
    render[7]->isXFlipped = 1;
    a = AM_AddComponent(player);
    AM_SetSprite(a, 6, 1, 6, PLY_SPRITE_FPS, render[7]); //have to add this for all spritesheets. to force the animation to run using this spritesheet

    //Disable all renderers and only enable the front facing one
    for (int i = 0; i < RENDER_COUNT; i++)
    {
        if (render[i]!=NULL)
        render[i]->isEnabled = 0;
    }
    render[1]->isEnabled = 1;

    //reset all booleans
    player->oDirection = DOWN;


    playerhealth = 3;

    float screenWidth, screenHeight;
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    GameObject* temp = GOM_Create2(RECTANGLE, CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f), 0, CP_Vector_Set(screenWidth, screenHeight));
    Renderer* r = RM_AddComponent(temp);
    RM_LoadImage(r, "Assets/fow2.png");
    r->renderPriority = PRI_UI;
    a = AM_AddComponent(temp);
    AM_SetSprite(a, 4, 1, 4, 10.0f, NULL);
    a->isContinuous = 0;
    a->loopDir = -1;
    player_fogofwar = temp;

    float xD = (float)CP_System_GetWindowWidth() / 100, yD = (float)CP_System_GetWindowHeight() / 100;

    heart1 = GOM_Create2(RECTANGLE, CP_Vector_Set(xD * 90, yD * 90), 0, CP_Vector_Set(50.0f, 50.0f));
    heart2 = GOM_Create2(RECTANGLE, CP_Vector_Set(xD * 85, yD * 90), 0, CP_Vector_Set(50.0f, 50.0f));
    heart3 = GOM_Create2(RECTANGLE, CP_Vector_Set(xD * 80, yD * 90), 0, CP_Vector_Set(50.0f, 50.0f));

    r = RM_AddComponent(heart1);
    RM_LoadImage(r, "Assets/heart.png");
    r->renderPriority = PRI_UI;

    r = RM_AddComponent(heart2);
    RM_LoadImage(r, "Assets/heart.png");
    r->renderPriority = PRI_UI;

    r = RM_AddComponent(heart3);
    RM_LoadImage(r, "Assets/heart.png");
    r->renderPriority = PRI_UI;

    // Player Invulnerability Bubble
    player_invulBubble = GOM_Create2(RECTANGLE, CP_Vector_Set(x, y), 0.0f, CP_Vector_Set(50, 50));
    player_invulBubble->tag = "playerBubble";
    r = RM_AddComponent(player_invulBubble);
    r->renderPriority = PRI_PLY;
    RM_LoadImage(r, "Assets/bananaboi/bubble-sprite.png");
    a = AM_AddComponent(player_invulBubble);
    AM_SetSprite(a, 2, 1, 2, 2.f, NULL);
    a->isContinuous = 1;
    a->loopDir = 1;

    p_Slowed = false;
    p_Hidden = false;
    p_Invincible = false;
    p_Hideable = false;
    g_objective1 = g_objective2 = g_objective3 = g_objective4 = g_objective5 = false;
    g_object1collect = g_object1drop = g_object1comp = false;

    ply_interactHint = GOM_Create2(EMPTY, CP_Vector_Add(player->position, PLY_HINTOFFSET), 0, CP_Vector_Set(20, 10));
    r = RM_AddComponent(ply_interactHint);
    r->textScale = CP_Vector_Set(0.75f, 0.75f);
    r->renderPriority = PRI_PLY;
    char tempstr[10] = {0};
    sprintf_s(tempstr, 10, "Press \'%c\'", cControls->cInteract);
    RM_SetText(r, tempstr);
    ply_interactHint->isEnabled = 0;

    return player;
} 
    
void PLY_Update() { // handles input from player and checking for flags
    float dt = CP_System_GetDt();
    float currentSpd = 200.0f - (n_weight * weight);

    AM_GetComponent(player_fogofwar)->loopDir = -1;
    

    if (p_Hidden == false) 
    {
        RM_GetComponent(player)->color.a = 255;
    }
    else
    {
        RM_GetComponent(player)->color.a = 122;
    }

    if (!p_Hideable)
    {
        //  player controls
        if (CP_Input_KeyDown((CP_KEY)cControls->cUp)) {
            player->position.y -= currentSpd * dt;
            SDM_PlayWEffect();
        }// up

        if (CP_Input_KeyDown((CP_KEY)cControls->cLeft)) {
            player->position.x -= currentSpd * dt;
            SDM_PlayWEffect();
        }  // left

        if (CP_Input_KeyDown((CP_KEY)cControls->cDown)) {
            player->position.y += currentSpd * dt; 
            SDM_PlayWEffect();

            player->oDirection = DOWN;
        } // down

        if (CP_Input_KeyDown((CP_KEY)cControls->cRight)) {
            player->position.x += currentSpd * dt;
            SDM_PlayWEffect();

            player->oDirection = RIGHT;
        } // right
    }
     
    // update and checks for invincibility
    if (p_Invincible == 1) 
    {
        counter();
        player_invulBubble->isEnabled = 1;
        player_invulBubble->position = player->position;
    }
    else
    {
        player_invulBubble->isEnabled = 0;
    }

    //checks for health
    if (playerhealth == 0) SM_DeleteGameObject(player);

    //checks for whether player can hide or not 
    if (p_Hidden == false)
    {
        if (p_Hideable == true)
        {
            p_Hidden = true;

            if (CP_Input_KeyTriggered((CP_KEY)cControls->cInteract)) 
            {
                //player->isEnabled = 0;
                p_Hidden = true;
            }
        }
    }
    else 
    {
        if (CP_Input_KeyTriggered((CP_KEY)cControls->cInteract)) 
        {
            //player->isEnabled = 1;
            p_Hidden = false;
        }
    }


    //RESET FLAG FOR COLLISION USE LATER
    p_Hideable = false;
    //REST FLAG
    p_Hidden = false;

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
        heart1->isEnabled = true;
        heart2->isEnabled = true;
        heart3->isEnabled = true;
        break;
    }
    case 2: {
        heart1->isEnabled = true;
        heart2->isEnabled = true;
        heart3->isEnabled = false;
        break;
    }
    case 1:{
        heart1->isEnabled = true;
        heart2->isEnabled = false;
        heart3->isEnabled = false;
        break;
    }
    default: {
        break;
    }
    }

    // returns player back to normal speed when 
    //spd = 200.0f;
    RM_SetCameraPosition(player->position);
    SDM_EffectUpdate();

    //Update player intereaction hint. has to hve after positional update so it wont look 1 frame lagg
    ply_interactHint->isEnabled = 0;
    ply_interactHint->position = CP_Vector_Add(player->position, PLY_HINTOFFSET);

    //set the correct sprite for player
    //first reset all 
    for (int i = 0; i < RENDER_COUNT; i++)
    {
        if (render[i]!=NULL)
        render[i]->isEnabled = 0;
    }


    if (player->oDirection == DOWN)
    {
        render[1]->isEnabled = 1;
    }
    else if (player->oDirection == RIGHT)
    {
        render[7]->isEnabled = 1;
    }

    if (playerhealth <= 0)
    {
        SceneManager_ChangeSceneByName("gameEnd");
    }
}

int PLY_IsHidden(void)
{
    return p_Hidden;
}

int PLY_IsInvincible(void)
{
    return p_Invincible;
}

bool PLY_TakeDamage(void)
{
    if (!p_Invincible)
    {
        startTime = clock(); //holds information on when the player was last hit according to the time
        p_Invincible = true;
        playerhealth--;
        SDM_PlaySFX(2);
        return true;
    }
    else
        return false;
}


void PLY_ShowInteractHint(void)
{
    ply_interactHint->isEnabled = 1;
}