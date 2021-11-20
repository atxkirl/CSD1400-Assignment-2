#include "Controls.h"
#include "FileParser.h"
#include <stdio.h>
#include <stdlib.h>

void Init_Controls()
{
    if(!cControls)
        cControls = malloc(sizeof(Controls));

    char cRightText[5];
    ReadControlsFromFile("Controls/controls.txt", cRightText);

    if (cControls)
    {
        cControls->cUp = cRightText[0];
        cControls->cDown = cRightText[1];
        cControls->cLeft = cRightText[2];
        cControls->cRight = cRightText[3];
        cControls->cInteract = cRightText[4];
    }
}

void Exit_Controls()
{
    if (cControls)
    {
        free(cControls);
        cControls = NULL;
    }
}