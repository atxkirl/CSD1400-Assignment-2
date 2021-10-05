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
	GAME_OBJECT,
	UI,
}RENDER_PRIORITY;

typedef struct
{
	CP_Vector position;
	CP_Vector scale;

	enum OBJECT_TYPE type;
	enum RENDER_PRIORITY renderPriority;

} GameObject;

GameObject* GameObject_new();