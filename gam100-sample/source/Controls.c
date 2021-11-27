#include "Controls.h"
#include "FileParser.h"
#include "SoundManager.h"
#include <stdio.h>
#include <stdlib.h>

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

void Exit_Controls()
{
    if (cControls)
    {
        free(cControls);
        cControls = NULL;
    }
}