#pragma once

#include "../CProcessing/inc/cprocessing.h"


typedef enum OBJECT_TYPE
{
	EMPTY = 0,
	CIRCLE,
	RECTANGLE,
	WATER,
	FLOOR,
	FLOOR_MIDDLE,
	WALL,
	THREE_CORNER,
	CORNER,
	PLAYER,
	LINE,
	PLAYER_SPAWN,
	OBJECT_START,
	ENEMY_SPAWN,
	CORAL,
	GRASS,
	EXIT,
	BOAT,
	BOAT_PARTS,
	TYPE_END // LAST 

}OBJECT_TYPE;

typedef enum OBJECT_DIRECTION
{
	// rotation counterclockwise from X-axis
	UP = 0,
	LEFT,
	DOWN,
	RIGHT,
	DIRECTION_END // LAST
} OBJECT_DIRECTION;

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
	enum OBJECT_DIRECTION oDirection;

} GameObject;
