#pragma once

#include "../CProcessing/inc/cprocessing.h"


typedef enum OBJECT_TYPE
{
	EMPTY = 0,
	CIRCLE,
	RECTANGLE,
	WALL,
	PLAYER,
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
	int isEnabled; //think as unity go's isenabled bool
	char* tag;//think as unity go's name

	//think as unity go's tag
	enum OBJECT_TYPE type;

} GameObject;
