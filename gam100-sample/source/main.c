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
#include "SceneManager.h"

/* Entry point */
int main(void)
{
    //16:9 dimensions only
    CP_System_SetWindowSize(1280, 720);
    // Set default framerate to 60
    CP_System_SetFrameRate(60.f);

    SceneManager_Initialize();
    SceneManager_ChangeSceneByName("splashscreen");

#ifdef _DEBUG
    CP_System_ShowConsole();
#endif // _DEBUG

    CP_Engine_Run();

    return 0;
}
