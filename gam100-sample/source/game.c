/*!
@file game.c
@author TODO
@course TODO
@section TODO
@tutorial TODO
@date TODO
@brief This file contains functions of game init, update, exit
*//*______________________________________________________________________*/

#include <stdio.h>
#include "cprocessing.h"
#include "../LevelEditor.h"

void game_init(void)
{
    //CP_System_Fullscreen();
    CP_System_SetWindowSize(1000, 1000);
    LevelEditorInit();
}

void game_update(void)
{
    LevelEditorUpdate();
}

void game_exit(void)
{

}
