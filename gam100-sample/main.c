/*---------------------------------------------------------
 * file:	main.c
 * author:	[NAME]
 * email:	[DIGIPEN EMAIL ADDRESS]
* 
 * brief:	Main entry point for the sample project
			* of the CProcessing library
* 
 * documentation link:
 * https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
* 
 * Copyright © 2020 DigiPen, All rights reserved.
* ---------------------------------------------------------*/

#include <stdio.h>
#include "cprocessing.h"
#include "source/SceneManager.h"

/* Entry point */
int main(void)
{
    CP_System_SetWindowSize(800, 600);

    SceneManager_Initialize();
    SceneManager_ChangeScene(currentScene);

    CP_System_ShowConsole();
    CP_Engine_Run();

    return 0;
}
