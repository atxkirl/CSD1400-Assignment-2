/*!
@file            Controls.c
@author          Lim Guan Sheng, Marcus (l.guanshengmarcus)
@course          CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief           This file contains the functions handling the player
                 controls.
*//*______________________________________________________________________*/


#include "Controls.h"
#include "FileParser.h"
#include "SoundManager.h"
#include <stdio.h>
#include <stdlib.h>

/// <summary>
/// Initialises the controls based on .txt
/// </summary>
void Init_Controls()
{
    if(!cControls)
        cControls = malloc(sizeof(Controls));

    char cRightText[7];
    ReadControlsFromFile("Controls/controls.txt", cRightText);

    if (cControls)
    {
        cControls->cUp = cRightText[0];
        cControls->cDown = cRightText[1];
        cControls->cLeft = cRightText[2];
        cControls->cRight = cRightText[3];
        cControls->cInteract = cRightText[4];
    }

    SDM_SetBGVolume((int)cRightText[5]);
    SDM_SetSFXVolume((int)cRightText[6]);
}

/// <summary>
/// Exit
/// </summary>
void Exit_Controls()
{
    if (cControls)
    {
        free(cControls);
        cControls = NULL;
    }
}