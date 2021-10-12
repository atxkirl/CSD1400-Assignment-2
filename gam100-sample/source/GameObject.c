#include "GameObject.h"
#include <stdlib.h>

GameObject* GameObject_new() {

	GameObject* go = malloc(sizeof(GameObject));

	if (go)
	{
		go->position = CP_Vector_Set(0.0f, 0.0f);
		go->scale = CP_Vector_Set(1.0f, 1.0f);
		go->type = CIRCLE;
		go->renderPriority = GAME_OBJECT;
		go->isEnabled = 1;
	}

	return go;
}