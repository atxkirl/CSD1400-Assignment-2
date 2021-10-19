#pragma once

#include "../CProcessing/inc/cprocessing.h"


typedef enum OBJECT_TYPE
{
	CIRCLE,
	RECTANGLE,
	WALL,
	PLAYER

}OBJECT_TYPE;
typedef enum RENDER_PRIORITY //lowest to highest
{
	PRI_GAME_OBJECT,
	PRI_UI,
}RENDER_PRIORITY;

/*!
@brief struct GameObject that contains transformation and render information
	Every objects have positional values, therefore many subclasses has a reference
	to this struct.
Special scenarios: -
*/
typedef struct
{
	CP_Vector position;
	CP_Vector scale;
	float rotation;
	int isEnabled; //bool for rendering
	char* tag;//tag same as unity tag
	CP_Color color;

	char* text;//to be directly rendered on top of object
	CP_Color textColor;//defaulted black
	CP_Vector textLocalPosition;//w reference to position
	CP_Vector textScale;//defaulted 1,1
	float textRotation;

	enum OBJECT_TYPE type;
	enum RENDER_PRIORITY renderPriority;

} GameObject;
