#pragma once

#include "../CProcessing/inc/cprocessing.h"


enum OBJECT_TYPE
{
	CIRCLE,
	RECTANGLE,
};
enum RENDER_PRIORITY //lowest to highest
{
	GAME_OBJECT,
	UI,
};

typedef struct Object
{
	CP_Vector position;
	CP_Vector scale;

	enum OBJECT_TYPE type;
	enum RENDER_PRIORITY renderPriority;

} GameObject;

GameObject* GameObject_new();