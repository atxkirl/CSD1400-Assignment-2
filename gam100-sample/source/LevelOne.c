/*!
@file LevelOne.c
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains functions of game init, update, exit
*//*______________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "LevelEditor.h"
#include "FileParser.h"
#include "Loader.h"
#include "Colors.h"
#include "SystemManager.h"
#include "Objectives.h"
#include "Objective_Connect.h"
#include "Player.h"
#include "SceneManager.h"
#include "SoundManager.h"

//GameObject* gLOne = NULL;

float screenWidth, screenHeight;
void LevelOneGridColliderInit();

static GameObject* bananaBoi = NULL;
static AStar_Map map;
FSM* enemy, *enemy2, *enemy3, *enemy4;

static GameObject* howtoplay = NULL;
static GameObject* close = NULL;
static Renderer* pauseCloseHighlight = NULL;
static Renderer* howtoplayCloseHighlight = NULL;
static bool isPaused;
LinkedList* pauseMenus;

static const int minSpawnTimer = 5;
static const int maxSpawnTimer = 15;
static float spawnElapsedTime = 0.f;
static float spawnTimer = 0.f;

static const int maxEnemyCount = 6;
static int enemyCount = 0;

/// <summary>
/// Function declaration for: Initializes the pause menu.
/// </summary>
void InitPause();
/// <summary>
/// Function declaration for: Setter for pause state.
/// </summary>
/// <param name="isPause">New pause state.</param>
void SetPause(int isPause);
/// <summary>
/// Function declaration for: Checks if game is paused or not.
/// </summary>
/// <returns>Pause state of the game.</returns>
int IsPaused();
/// <summary>
/// Function declaration for: Clears memory allocated by pause.
/// </summary>
void ClearPause();
/// <summary>
/// Loop that slowly spawns more and more enemies into the level.
/// </summary>
void SpawnEnemies();

GameObject* GameFpsCounterObj;
/// <summary>
/// Function declaration for: Initializes framerate display object.
/// </summary>
void InitDrawFPS();

/// <summary>
/// Initializes A* Pathfinding for this level.
/// </summary>
void LevelOne_AStarInit(void);

/// <summary>
/// Called whenever collision occurs.
/// </summary>
void LevelOne_OnCollision(Collider* left, Collider* right)
{
    if (strcmp(right->obj->tag, ONCLICK_TAG) == 0)
    {
        if (strcmp(left->obj->tag, "pauseClose") == 0)
        {
            SetPause(0);
        }
        else if (strcmp(left->obj->tag, "ReturnToMainMenu") == 0)
        {
            SceneManager_ChangeSceneByName("mainmenu");
        }
        else if (strcmp(((GameObject*)left->obj)->tag, "howtoplayClose") == 0)
        {
            howtoplay->isEnabled = 0;
            close->isEnabled = 0;
            SetPause(false);
        }
    }
    if (strcmp(right->obj->tag, "Mouse") == 0)
    {
        if (strcmp(left->obj->tag, "pauseClose") == 0)
        {
            pauseCloseHighlight->isEnabled = 1;
        }
        else if (strcmp(left->obj->tag, "howtoplayClose") == 0)
        {
            howtoplayCloseHighlight->isEnabled = 1;
        }
    }
}

/// <summary>
/// Initializes this level.
/// </summary>
void LevelOne_init(void)
{
    SM_SystemsInit();
    //RM_AddComponent(g);
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);

    // Loader
    {
        LoaderInit();
        LoadGrid("level01", 0);

        //Insert spawn x,y here
        CP_Vector PlayerPos = GetPlayerPosition();
        bananaBoi = PLY_CreatePlayer(PlayerPos.x, PlayerPos.y);

        Objectives_Init(screenWidth, screenHeight);
        Objectives_RenderUI();

        // Initialize level colliders
        LevelOneGridColliderInit();
    }
    
    InitPause();
    InitDrawFPS();

    // Scene UI
    {
        // render how to play
        howtoplay = GOM_Create2(RECTANGLE, CP_Vector_Set(0.5f * screenWidth, 0.5f * screenHeight), 0.0f, CP_Vector_Set(screenWidth, screenHeight));
        howtoplay->tag = "credits";
        Renderer* howtoplayRenderer = RM_AddComponent(howtoplay);
        howtoplayRenderer->renderPriority = PRI_UI;
        RM_LoadImage(howtoplayRenderer, "Assets/scenes/howtoplay.png");
        // close button
        close = GOM_Create(RECTANGLE);
        close->position = CP_Vector_Set(screenWidth * 0.975f, screenHeight * 0.045f);
        close->scale = CP_Vector_Set(50, 50);
        close->tag = "howtoplayClose";
        Collider* closeCollider = CLM_AddComponent(close);
        CLM_Set(closeCollider, COL_BOX, LevelOne_OnCollision);
        closeCollider->space = COLSPC_SCREEN;
        // Close button unhighlight
        Renderer* closeRender = RM_AddComponent(close);
        closeRender->renderPriority = PRI_UI;
        RM_LoadImage(closeRender, "Assets/cross.png");
        // Close button highlight
        howtoplayCloseHighlight = RM_AddComponent(close);
        howtoplayCloseHighlight->renderPriority = PRI_UI;
        RM_LoadImage(howtoplayCloseHighlight, "Assets/crosshighlight.png");
    }

    // AStar
    {
        AStar_InitializeMap(&map, NumGrids, NumGrids);

        // Set settings of each node in the level based on the level loaded.
        LevelOne_AStarInit();
    }

    // Enemies
    {
        //enemy = AIM_CreateEnemy("BBEM", "BBEM_Idle", GetEnemyPosition(), bananaBoi, &map);
        //enemy2 = AIM_CreateEnemy("BBEM", "BBEM_Roam", GetEnemyPosition(), bananaBoi, &map);
        //enemy3 = AIM_CreateEnemy("BBEM", "BBEM_Roam", GetEnemyPosition(), bananaBoi, &map);
        //enemy4 = AIM_CreateEnemy("BBEM", "BBEM_Idle", GetEnemyPosition(), bananaBoi, &map);

        AIM_CreateEnemy("BBEM", "BBEM_Idle", GetEnemyPosition(), bananaBoi, &map);
        enemyCount = 1;

        // Set a random timer for next spawner
        spawnTimer = FRAND(minSpawnTimer, maxSpawnTimer);
        spawnElapsedTime = 0.f;
    }

    SDM_Init();
    SDM_PlayBgMusic(2);

    // Set game to be paused at the start.
    SetPause(true);
}

/// <summary>
/// Update loop called every frame.
/// </summary>
void LevelOne_update(void)
{
    SM_SystemsPreUpdate();

    if (!IsPaused())
    {
        if (Objectives_GetPlayerUpdate())
        {
            PLY_Update();
        }
        Objectives_Update();
        SpawnEnemies();
    }
    else
    {
        howtoplayCloseHighlight->isEnabled = 0;
        pauseCloseHighlight->isEnabled = 0;
    }
    SM_SystemsUpdate(IsPaused());

    Renderer* r = RM_GetComponent(GameFpsCounterObj);
    float fps = 1.0f / CP_System_GetDt();
    char t[7] = { 0 };
    sprintf_s(t, 7, "%5.2f", fps);
    RM_SetText(r, t);

    SM_SystemsLateUpdate();
}

/// <summary>
/// Called upon scene exit.
/// </summary>
void LevelOne_exit(void)
{
    ClearPause();
    Objectives_Exit();
    LoaderExit();
    SM_SystemsExit();
    SDM_StopAll();
    SDM_FreeSounds();
}

/// <summary>
/// Called by SceneManager to register function pointers for scene init, update and exit.
/// </summary>
/// <param name="init">Init function for this scene.</param>
/// <param name="update">Update function for this scene</param>
/// <param name="exit">Exit function for this scene</param>
void LevelOne_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = LevelOne_init;
    *update = LevelOne_update;
    *exit = LevelOne_exit;
}

/// <summary>
/// Initializes all colliders within this scene.
/// </summary>
void LevelOneGridColliderInit()
{
    for (int i = 0; i < NumGrids; i++)
    {
        for (int j = 0; j < NumGrids; j++)
        {
            if (gLoadedGrids->gGrid[i][j]->type == WATER || gLoadedGrids->gGrid[i][j]->type == EMPTY)
            {
                CLM_Set(CLM_GetComponent(gLoadedGrids->gGrid[i][j]), COL_BOX, LevelOne_OnCollision);
                CLM_GetComponent(gLoadedGrids->gGrid[i][j])->isLockedPos = 1;
            }
            if (gLoadedObjects->gGrid[i][j]->type == CORAL || gLoadedObjects->gGrid[i][j]->type == GRASS)
            {
                CLM_GetComponent(gLoadedObjects->gGrid[i][j])->OnCollision = LevelOne_OnCollision;
            }
        }
    }
}

/// <summary>
/// Initializes the pause menu.
/// </summary>
void InitPause()
{
    pauseMenus = NULL;
    //just to be safe, can load again
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    GameObject* g = GOM_Create(RECTANGLE);
    g->position = CP_Vector_Set(screenWidth * 0.5f, screenHeight * 0.5f);
    g->scale = CP_Vector_Set(screenHeight * 0.5f, screenHeight * 0.5f);
    Renderer* r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    LL_Add(&pauseMenus, g);
    GameObject* temp = g;

    g = GOM_Create(EMPTY);
    g->position = CP_Vector_Set(temp->position.x, temp->position.y - temp->scale.y * 0.2f);
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->textScale = CP_Vector_Set(3.0f, 3.0f);
    RM_SetText(r, "Pause");
    LL_Add(&pauseMenus, g);

    g = GOM_Create(RECTANGLE);
    g->position = CP_Vector_Set(temp->position.x + temp->scale.x * 0.5f, temp->position.y - temp->scale.y * 0.5f);
    g->scale = CP_Vector_Set(50, 50);
    g->tag = "pauseClose";
    Collider* c = CLM_AddComponent(g);
    CLM_Set(c, COL_BOX, LevelOne_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;
    LL_Add(&pauseMenus, g);
    // Close Button unhighlighted.
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    RM_LoadImage(r, "Assets/cross.png");
    // Close Button highlighted.
    pauseCloseHighlight = RM_AddComponent(g);
    pauseCloseHighlight->renderPriority = PRI_UI;
    RM_LoadImage(pauseCloseHighlight, "Assets/crosshighlight.png");

    g = GOM_Create(RECTANGLE);
    g->position = CP_Vector_Set(temp->position.x, temp->position.y + temp->scale.y * 0.2f);
    g->scale = CP_Vector_Set(220, 50);
    g->tag = "ReturnToMainMenu";
    r = RM_AddComponent(g);
    r->renderPriority = PRI_UI;
    r->color = CP_Color_Create(255, 255, 255, 255);
    r->textScale = CP_Vector_Set(1.5f, 1.5f);
    RM_SetText(r, "Return to main menu");
    c = CLM_AddComponent(g);
    CLM_Set(c, COL_BOX, LevelOne_OnCollision);
    c->space = COLSPC_SCREEN;
    c->isTrigger = 1;
    LL_Add(&pauseMenus, g);

    LinkedList* n = pauseMenus;
    for (; n; n = n->next)
    {
        GameObject* t = (GameObject*)n->curr;
        t->isEnabled = 0;
    }
}
/// <summary>
/// Setter for pause state.
/// </summary>
/// <param name="isPause">New pause state.</param>
void SetPause(int isPause)
{
    isPaused = isPause;
    LinkedList* n = pauseMenus;
    for (; n; n = n->next)
    {
        GameObject* g = (GameObject*)n->curr;
        g->isEnabled = isPause;
    }
}
/// <summary>
/// Checks if game is paused or not.
/// </summary>
/// <returns>Pause state of the game.</returns>
int IsPaused()
{
    if (CP_Input_KeyTriggered((CP_KEY)KEY_ESCAPE) || CP_Input_KeyTriggered((CP_KEY)KEY_P))
    {
        SetPause(1);
    }
    isPaused = false;
    if (pauseMenus)
        isPaused = ((GameObject*)pauseMenus->curr)->isEnabled;
    
    if (isPaused)
    {
        LinkedList* n = pauseMenus; GameObject* button = NULL;
        for (; n; n = n->next)
        {
            GameObject* tn = (GameObject*)n->curr;
            if (strcmp(tn->tag, "ReturnToMainMenu") == 0)
            {
                button = tn;
                break;
            }
        }
        if (button)
        {
            float mouseX = CP_Input_GetMouseX();
            float mouseY = CP_Input_GetMouseY();
            GameObject* tempMouse = GOM_CreateTemp(EMPTY);
            tempMouse->position = CP_Vector_Set(mouseX, mouseY);
            Collider* c = CLM_AddComponent(tempMouse);
            CLM_Set(c, COL_POINT, NULL);
            c->space = COLSPC_SCREEN;
            c->isTrigger = 1;

            Collider* buttonc = CLM_GetComponent(button);
            CP_Vector spd = CP_Vector_Set(200, 100);
            CP_Vector def = CP_Vector_Set(220, 50);
            CP_Vector max = CP_Vector_Set(260, 70);
            if (IsBoxCollidePoint(buttonc, c))
            {
                button->scale.x += spd.x * CP_System_GetDt();
                button->scale.y += spd.y * CP_System_GetDt();
                button->scale.x = min(button->scale.x, max.x);
                button->scale.y = min(button->scale.y, max.y);
            }
            else
            {
                //scale down
                button->scale.x -= spd.x * CP_System_GetDt();
                button->scale.y -= spd.y * CP_System_GetDt();
                button->scale.x = max(button->scale.x, def.x);
                button->scale.y = max(button->scale.y, def.y);
            }
        }

    }

    return isPaused;
}
/// <summary>
/// Clears memory allocated by pause.
/// </summary>
void ClearPause()
{
    LL_Clear(&pauseMenus);
    pauseMenus = NULL;
}

/// <summary>
/// Loop that slowly spawns more and more enemies into the level.
/// </summary>
void SpawnEnemies()
{
    spawnElapsedTime += CP_System_GetDt();
    if (enemyCount >= maxEnemyCount)
        return;
    else if (spawnElapsedTime < spawnTimer)
        return;

    printf("Spawning an Enemy!!\n");
    // Spawn an enemy.
    CP_Vector spawnPos = CP_Vector_Zero();
    AStar_GetTile(&spawnPos, bananaBoi->position, &map, 3, 6);
    AIM_CreateEnemy("BBEM", "BBEM_Idle", spawnPos, bananaBoi, &map);
    ++enemyCount;

    // Reset timer and get new spawn timer.
    spawnTimer = FRAND(minSpawnTimer, maxSpawnTimer);
    spawnElapsedTime -= spawnElapsedTime;
}

/// <summary>
/// Initializes framerate display object.
/// </summary>
void InitDrawFPS()
{
    RM_GetRenderSize(&screenWidth, &screenHeight, PRI_UI);
    CP_Vector padding = CP_Vector_Set(20, 20);
    CP_Vector size = CP_Vector_Set(50, 20);
    GameFpsCounterObj = GOM_Create(EMPTY);
    GameFpsCounterObj->position = CP_Vector_Set(screenWidth - padding.y - size.x * 0.5f, 0 + padding.y + size.y * 0.5f);
    GameFpsCounterObj->scale = size;
    Renderer* r = RM_AddComponent(GameFpsCounterObj);
    r->renderPriority = PRI_UI;
    RM_SetText(r, "FPS:");
    r->textColor = COLOR_GREEN;
    GameFpsCounterObj->isEnabled = 0;

#if _DEBUG
    GameFpsCounterObj->isEnabled = 1;
#endif
}
/// <summary>
/// Renders the framerate counter.
/// </summary>
void RenderFpsCounter()
{
}

/// <summary>
/// Initialises AStar map grid for this level.
/// </summary>
void LevelOne_AStarInit(void)
{
    for (int r = 0; r < map.rows; ++r)
    {
        for (int c = 0; c < map.columns; ++c)
        {
            AStar_Node* temp = &map.map[r][c];
            AStar_InitializeNode(&temp, r, c, gLoadedGrids->gGrid[r][c]->position, NODE_DEFAULT);

            if (gLoadedGrids->gGrid[r][c]->type == WATER || gLoadedGrids->gGrid[r][c]->type == EMPTY)
            {
                temp->type = NODE_WALL;
            }
            else if (gLoadedObjects->gGrid[r][c]->type == CORAL || gLoadedObjects->gGrid[r][c]->type == GRASS)
            {
                temp->type = NODE_HIDEABLE;
            }
        }
    }
}
