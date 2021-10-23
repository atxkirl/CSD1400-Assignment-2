#pragma once

#include "../CProcessing/inc/cprocessing.h"


typedef enum OBJECT_TYPE
{
	CIRCLE,
	RECTANGLE,
	WALL,
	PLAYER,
	EMPTY,
	LINE,
	END // LAST 

}OBJECT_TYPE;

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
	char* tag;//tag same as unity tag/name

	//imagine unity tag?
	enum OBJECT_TYPE type;

} GameObject;
