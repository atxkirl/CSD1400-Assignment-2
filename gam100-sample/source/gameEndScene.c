#define BUTTON_WIDTH 60.f
#define BUTTON_HEIGHT 30.f

#include <stdio.h>
#include "cprocessing.h"
#include "Helpers.h"
#include "SceneManager.h"
#include "SystemManager.h"
#include "Colors.h"

void gameEnd_OnCollision(Collider* left, Collider* right) {

    if (strcmp(((GameObject*)right->obj)->tag, "Click") == 0)
    {
        if (strcmp(((GameObject*)left->obj)->tag, "restart") == 0)
            SceneManager_ChangeSceneByName("weiyi");
        else if (strcmp(((GameObject*)left->obj)->tag, "exit") == 0)
            SceneManager_ChangeSceneByName("mainmenu");
    }
}

void gameEnd_init(void)
{
    float xScale = CP_System_GetWindowWidth() / 100.0f, yScale = CP_System_GetWindowHeight()/100.0f;
    SM_SystemsInit();

    Collider* gEnd = NULL;
    Renderer* rEnd = NULL;


    GameObject* eButton = GOM_Create2(RECTANGLE, CP_Vector_Set(50 * xScale, 90 * yScale), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    eButton->tag = "exit";
    gEnd = CLM_AddComponent(eButton);
    CLM_Set(gEnd, COL_BOX, gameEnd_OnCollision);
    gEnd->space = COLSPC_SCREEN;
    rEnd = RM_AddComponent(eButton);
    rEnd->renderPriority = PRI_UI;
    RM_SetText(rEnd, "Main Menu");

    GameObject* rButton = GOM_Create2(RECTANGLE, CP_Vector_Set(50 * xScale, 80 * yScale), 0.0f, CP_Vector_Set(BUTTON_WIDTH, BUTTON_HEIGHT));
    rButton->tag = "restart";
    gEnd = CLM_AddComponent(rButton);
    CLM_Set(gEnd, COL_BOX, gameEnd_OnCollision);
    gEnd->space = COLSPC_SCREEN;
    rEnd = RM_AddComponent(rButton);
    rEnd->renderPriority = PRI_UI;
    RM_SetText(rEnd, "Restart");




}

void gameEnd_update(void)
{
    SM_SystemsPreUpdate();

    SM_SystemsUpdate();

    SM_SystemsLateUpdate();
}

void gameEnd_exit(void)
{
    SM_SystemsExit();
}

void gameEnd_sceneInit(FunctionPtr* init, FunctionPtr* update, FunctionPtr* exit)
{
    *init = gameEnd_init;
    *update = gameEnd_update;
    *exit = gameEnd_exit;
}


