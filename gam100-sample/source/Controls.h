#pragma once

typedef struct Controls
{
	char cUp;
	char cDown;
	char cLeft;
	char cRight;
	char cInteract;

} Controls;

Controls* cControls;

void Init_Controls();
void Exit_Controls();