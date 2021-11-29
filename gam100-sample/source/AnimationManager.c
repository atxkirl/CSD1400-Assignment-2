/*!
@file		AnimationManager.c
@author		Ow Hong Yu (ow.h)
@course		CSD1400 Software Engineering Project 1
@Team            BananaBoi
@brief		A system that contains the logics of all animations and updates the entities 
			every frame.
*/

#include "SystemManager.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <math.h>
#include "Helpers.h"

LinkedList* animationList;

#define MY_PI 3.141592f

/// <summary>
/// Updates specifically sprite animation every frame
/// </summary>
/// <param name="a">- Animation entity with type sprite</param>
/// <param name="dt">- delta time of frames</param>
void Update_SpriteAnimation(Animation* a, float dt)
{
	a->elapsedTime += CP_System_GetDt();
	float delay = 1.0f / a->fps;
	if (a->elapsedTime >= delay)
	{
		if (a->loopDir > 0)
			a->index = a->index + 1;
		else if (a->loopDir < 0)
			a->index = a->index - 1;

		if (a->index >= a->frameCount && a->isContinuous)
			a->index = 0;
		else if (a->index < 0 && a->isContinuous)
			a->index = a->frameCount - 1;
		else
		{
			if (a->index >= a->frameCount)
				a->index = a->frameCount - 1;
			else if (a->index < 0)
				a->index = 0;
		}
		a->elapsedTime = 0.0f;

		float w = 1.0f / a->splitX;
		float h = 1.0f / a->splitY;
		float l = (a->index % a->splitX) * (w);
		float t = (a->index / a->splitX) * h;

		Renderer* r = RM_GetComponent(a->go);
		if (a->forcedRenderer)
			r = a->forcedRenderer;
		r->startUV = CP_Vector_Set(l,t);
		r->endUV = CP_Vector_Set(l + w, t + h);
	}
}
/// <summary>
/// Updates specifically shaking animation every frame
/// </summary>
/// <param name="a">- Animation entity with type shake</param>
/// <param name="dt">- delta time of frames</param>
void Update_ShakeAnimation(Animation* a, float dt)
{
	a->elapsedTime += CP_System_GetDt();

	float r = a->elapsedTime / a->loopTime;
	if (r < 1.0f)
	{
		float rot = a->rotateAngle * sinf(r * 2.0f * 3.141592f);
		a->go->rotation = a->defaultRotate + rot;
	}
	else
	{
		a->go->rotation = a->defaultRotate;
		a->loopCounter++;
		a->elapsedTime = 0.0f;
	}

	if (a->isContinuous) return;

	if (a->loopCounter >= a->loopCount)
	{
		a->isEnabled = 0;
		a->loopCounter = 0;
	}
}
/// <summary>
/// Updates specifically zooming animation every frame
/// </summary>
/// <param name="a">- Animation entity with type zoom</param>
/// <param name="dt">- delta time of frames</param>
void Update_ZoomAnimation(Animation* a, float dt)
{
	a->elapsedTime += CP_System_GetDt();

	float r = a->elapsedTime / a->scaleToTime;
	if (r > 1.0f)
	{
		if (!a->isFlipped)
			a->go->scale = a->targetScale;
		else
			a->go->scale = a->defaultScale;

		a->elapsedTime = 0.0f;
		a->isEnabled = 0;
		return;
	}
	CP_Vector v = CP_Vector_Subtract(a->targetScale, a->defaultScale);
	v.x = v.x * r;
	v.y = v.y * r;

	if (!a->isFlipped) v = CP_Vector_Add(v, a->defaultScale);
	else v = CP_Vector_Add(CP_Vector_Negate(v), a->targetScale);

	a->go->scale = v;

	
}
/// <summary>
/// Updates specifically walking animation for the player every frame
/// </summary>
/// <param name="a">- Animation entity with type walk</param>
/// <param name="dt">- delta time of frames</param>
void Update_WalkAnimation(Animation* a, float dt)
{
	if (CP_Vector_Distance(a->oldPos, a->go->position) > FLT_EPS)
	{
		a->elapsedTime += dt;
		if (a->elapsedTime > a->loopTime)
		{
			float t = 0.5f;
			CP_Vector pos = a->go->position;
			CP_Vector scale = CP_Vector_Set(20, 20);
			pos.y += a->go->scale.y * 0.5f - scale.y * 0.3f;
			float rot = CP_Random_RangeFloat(0.0f, 360.0f);
			GameObject* p = GOM_Create2(RECTANGLE, pos, rot, scale);
			Renderer* r = RM_AddComponent(p);
			RM_LoadImage(r, "Assets/walkparticle.png");
			Animation* ani = AM_AddComponent(p);
			AM_SetZoom(ani, scale, CP_Vector_Set(2.f, 2.f), t * 2.0f, 0);
			SM_DeleteGameObjectAfter(p, t);
			a->elapsedTime = 0.0f;
		}
		a->oldPos = a->go->position;
	}

}

/*!
@brief This function updates the animation for blinking ( color change )
@param a - Animation to be updated
@param dt - delta time
@return void
*/
void Update_BlinkAnimation(Animation* a, float dt)
{
	a->elapsedTime += CP_System_GetDt();

	float r = a->elapsedTime / a->loopTime;
	Renderer* renderer = RM_GetComponent(a->go);
	if (a->forcedRenderer)
		renderer = a->forcedRenderer;
	CP_Color* c[2] = { NULL, NULL };
	switch (a->index)
	{
	case 0:
		c[0] = &renderer->color;
		break;
	case 1:
		c[0] = &renderer->strokeColor;
		break;
	case 2:
		c[0] = &renderer->color;
		c[1] = &renderer->strokeColor;
		break;
	}


	float ratio = r / 0.5f;
	if (r < 1.0f)
	{
		if (r > 0.5f) //flip
		{
			ratio = (r - 0.5f) / 0.5f;
			ratio = 1.0f - ratio;
		}

		for (int i = 0; i < 2; i++)
		{
			if (c[i] == NULL)
				continue;
			c[i]->r = (unsigned char)((a->targetColor.r - (float)a->defaultColor.r) * ratio + (float)a->defaultColor.r);
			c[i]->g = (unsigned char)((a->targetColor.g - (float)a->defaultColor.g) * ratio + (float)a->defaultColor.g);
			c[i]->b = (unsigned char)((a->targetColor.b - (float)a->defaultColor.b) * ratio + (float)a->defaultColor.b);
			c[i]->a = (unsigned char)((a->targetColor.a - (float)a->defaultColor.a) * ratio + (float)a->defaultColor.a);
		}

	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			if (c[i] == NULL)
				continue;
			c[i]->r = a->defaultColor.r;
			c[i]->g = a->defaultColor.g;
			c[i]->b = a->defaultColor.b;
			c[i]->a = a->defaultColor.a;
		}

		if (a->isContinuous)
			a->elapsedTime = 0.0f;
		else
			a->isEnabled = 0;
	}



}

/*!
@brief This function updates the animation for translation lerp
@param a - Animation to be updated
@param dt - delta time
@return void
*/
void Update_TranslateLerp(Animation* a, float dt)
{
	if (a->type == ANIM_UFDSTATEFLTTRANSIT)
	{
		//special lerp state
		float spd = 10 * dt;
		CP_Vector v = CP_Vector_Subtract(a->targetScale, a->go->position);
		float d = CP_Vector_Length(v);
		
		if (spd > d)
		{
			a->go->position = a->targetScale;
			a->isNextStateFlag = 0;
			a->type = ANIM_UFDSTATEDOWN;
			a->isFlipped = 1;
			a->elapsedTime = 0.0f;
		}
		else
		{
			a->go->position = CP_Vector_Add(a->go->position, CP_Vector_Scale(v, spd / d));
		}

		return;
	}

	float f = a->elapsedTime / a->scaleToTime;
	if (f < 1.0f)
	{
		a->elapsedTime += CP_System_GetDt();
		CP_Vector v = CP_Vector_Subtract(a->targetScale, a->defaultScale);
		if (a->isFlipped)
			f = 1.0f - f;
		a->go->position = CP_Vector_Add(a->defaultScale, CP_Vector_Scale(v, f));
	}
	else
	{
		a->go->position = a->targetScale;

		if (a->type == ANIM_UFDSTATEUP) //reach pos alr, next state is float there
		{
			a->type = ANIM_UFDSTATEFLT;
			a->elapsedTime = 0.0f;
		}
		else if (a->type == ANIM_UFDSTATEDOWN)
		{
			//reach the end of down. off this state
			a->isFlipped = 0;
			a->type = ANIM_UFDSTATEUP;
			a->elapsedTime = 0.0f;
			a->go->isEnabled = 0;
		}
	}

}

/*!
@brief This function updates the animation for translation floating
@param a - Animation to be updated
@param dt - delta time
@return void
*/
void Update_TranslateFloating(Animation* a, float dt)
{
	float f = a->elapsedTime / a->loopTime;

	{
		a->elapsedTime += CP_System_GetDt();
		a->go->position. y = a->oldPos.y + a->rotateAngle * sinf(f * 2.0f * MY_PI);
	}

	if (f >= 1.0f)
	{
		a->elapsedTime -= a->loopTime;
	}

	if (a->type == ANIM_UFDSTATEFLT && a->isNextStateFlag)
	{
		a->isNextStateFlag = 0;
		a->type = ANIM_UFDSTATEFLTTRANSIT;
		a->isFlipped = 1;
		a->elapsedTime = 0.0f;
	}

}

/// <summary>
/// Inits animation manager
/// </summary>
void AM_Init()
{
}

/// <summary>
/// Updates animation manager every frame
/// </summary>
void AM_Update()
{
	//int count = LL_GetCount(animationList);
	//void** arr = LL_ToArray(animationList);
	LinkedList* node = animationList;
	for (; node; node = node->next)
	{
		Animation* a = (Animation*) node->curr;
		if (!a->isEnabled || !a->go->isEnabled)
			continue;
		switch (a->type)
		{
		case ANIM_SHAKE:
			Update_ShakeAnimation(a, CP_System_GetDt());
			break;
		case ANIM_ZOOM:
			Update_ZoomAnimation(a, CP_System_GetDt());
			break;
		case ANIM_WALKSAND:
			Update_WalkAnimation(a, CP_System_GetDt());
			break;
		case ANIM_COLORBLINK:
			Update_BlinkAnimation(a, CP_System_GetDt());
			break;
		case ANIM_UFDSTATEUP:
		case ANIM_UFDSTATEDOWN:
		case ANIM_UFDSTATEFLTTRANSIT:
		case ANIM_TRANSLATELERP:
			Update_TranslateLerp(a, CP_System_GetDt());
			break;
		case ANIM_UFDSTATEFLT:
		case ANIM_TRANSLATEFLOAT:
			Update_TranslateFloating(a, CP_System_GetDt());
			break;
		default:
			Update_SpriteAnimation(a, CP_System_GetDt());
			break;
		}
	}
}

/// <summary>
/// Clears animation manager and clean up memory used
/// </summary>
void AM_Clear()
{
	LinkedList* node = animationList;
	for (; node; node = node->next)
	{
		free(node->curr);
	}
	//animationList = LL_Clear(animationList);
	LL_Clear(&animationList);
}
/// <summary>
/// Adds and creates a animation component and store it in manager's list
/// </summary>
/// <param name="go">- Parent game object of the animation component created</param>
/// <returns></returns>
Animation* AM_AddComponent(GameObject* go)
{
	Animation* ani = malloc(sizeof(Animation));
	if (ani)
	{
		ani->go = go;
		ani->splitX = 1;
		ani->splitY = 1;
		ani->elapsedTime = 0.0f;
		ani->index = 0;
		ani->frameCount = 0;
		ani->fps = 0.0f;
		ani->type = ANIM_SPRITE;

		ani->rotateAngle = 0.0f;
		ani->defaultRotate = 0.0f;
		ani->loopTime = 0.0f;
		ani->loopCount = 0;
		ani->loopCounter = 0;
		ani->isContinuous = 0;
	}
	LL_Add(&animationList, ani);
	return ani;
}

/*!
@brief Gets Animation from go object if go has added animation
@param curr - curr node data
@param arg - gameobject to find
@return animation component that exists
*/
void* FindAnimation(void* curr, void* arg)
{
	GameObject* g = (GameObject*)arg;
	Animation* a = (Animation*)curr;
	if (a->go == g)
		return a;
	return NULL;
}
/// <summary>
/// Returns the animation component of the gameobject if it exists
/// </summary>
/// <param name="go">- Parent game object of animation to be found</param>
/// <returns></returns>
Animation* AM_GetComponent(GameObject* go)
{
	return LL_Find(animationList, FindAnimation, go);
}
/// <summary>
/// Removes the animation component from manager's list
/// </summary>
/// <param name="a">- Animation component to be removed</param>
void AM_Remove(Animation* a)
{
	if (!LL_ContainsPtr(animationList, a))
		return;
	LL_RemovePtr(&animationList, (void*)a);
	free(a);
}
/// <summary>
/// Sets animation to be of sprite animation
/// </summary>
/// <param name="a">- animation to be set</param>
/// <param name="x">- size of x split</param>
/// <param name="y">- size of y split</param>
/// <param name="f">- number of frames</param>
/// <param name="fps">- fps of animation to be played</param>
void AM_SetSprite(Animation* a, int x, int y, int f, float fps, Renderer* r)
{
	a->elapsedTime = 0.0f;
	a->splitX = x;
	a->splitY = y;
	a->frameCount = f;
	a->fps = fps;
	a->type = ANIM_SPRITE;
	a->loopDir = 1;
	a->isContinuous = 1;
	a->forcedRenderer = r;

	float w = 1.0f / a->splitX;
	float h = 1.0f / a->splitY;
	float l = (a->index % a->splitX) * (w);
	float t = (a->index / a->splitX) * h;
	Renderer* tr = RM_GetComponent(a->go);
	if (a->forcedRenderer)
		tr = a->forcedRenderer;
	tr->startUV = CP_Vector_Set(l, t);
	tr->endUV = CP_Vector_Set(l + w, t + h);
}
/// <summary>
/// Set shake animation for component
/// </summary>
/// <param name="a">- animation to be set</param>
/// <param name="rotateAngle">- max angle to rotate</param>
/// <param name="loopTime">- how long for one sin curve</param>
/// <param name="loopCount">- how many loops</param>
/// <param name="isContinuous">- if the shake continues nonstop</param>
void AM_SetShake(Animation* a, float rotateAngle, float loopTime, int loopCount, int isContinuous)
{
	a->elapsedTime = 0.0f;
	a->rotateAngle = rotateAngle;
	a->loopCount = loopCount;
	a->loopTime = loopTime;
	a->isContinuous = isContinuous;
	a->type = ANIM_SHAKE;

	a->defaultRotate = a->go->rotation;
}
/// <summary>
/// Set zoom animation for component
/// </summary>
/// <param name="a">- animation to be set</param>
/// <param name="defaultScale">- defaultscale of obj</param>
/// <param name="targetScale">- target scale value</param>
/// <param name="scaleToTime">- time from default to scale to target</param>
/// <param name="isFlipped">- is scale from default to target or flipped = target to default</param>
void AM_SetZoom(Animation* a, CP_Vector defaultScale, CP_Vector targetScale, float scaleToTime, int isFlipped)
{
	a->elapsedTime = 0.0f;
	a->defaultScale = defaultScale;
	a->targetScale = targetScale;
	a->scaleToTime = scaleToTime;
	a->isFlipped = isFlipped;
	a->type = ANIM_ZOOM;
}
/// <summary>
/// Set a particle generator to generate particles when player walks/moves
/// </summary>
/// <param name="a">- animation to be set</param>
void AM_SetWalk(Animation* a)
{
	a->type = ANIM_WALKSAND;
	a->elapsedTime = 0.0f;
	a->loopTime = 0.5f;
	a->oldPos = a->go->position;
}
/// <summary>
/// Set animation to blinking color
/// </summary>
/// <param name="a">- animation to be set</param>
/// <param name="defaultColor">- default color</param>
/// <param name="targetColor">- target color</param>
/// <param name="loopTime">- time to loop from default to target to default</param>
/// <param name="isContinuous">- will it continue non stop</param>
/// <param name="mode">- 0 for fill, 1 for stroke, 2 for both</param>
void AM_SetBlink(Animation* a, CP_Color defaultColor, CP_Color targetColor, float loopTime, int isContinuous, int mode)
{
	a->type = ANIM_COLORBLINK;
	a->elapsedTime = 0.0f;
	a->loopTime = loopTime;
	a->targetColor = targetColor;
	a->defaultColor = defaultColor;
	a->isContinuous = isContinuous;
	a->index = mode;
	a->forcedRenderer = NULL;
}
/// <summary>
/// Set Animation for object to translate from pa to pb in this time
/// </summary>
/// <param name="a">- animation to be set</param>
/// <param name="pa">- from position</param>
/// <param name="pb">- to position</param>
/// <param name="t">- time taken from pa to pb</param>
/// <param name="isFlipped">- start from to and end at from</param>
void AM_SetLerp(Animation* a, CP_Vector pa, CP_Vector pb, float t, int isFlipped)
{
	a->type = ANIM_TRANSLATELERP;
	a->defaultScale = pa;
	a->targetScale = pb;
	a->scaleToTime = t;
	a->elapsedTime = 0.0f;
	a->isFlipped = isFlipped;
}
/// <summary>
/// Set Animation for object to float about point p (sin curve up and down)
/// </summary>
/// <param name="a">- animation to be set</param>
/// <param name="p">- from position</param>
/// <param name="amplitude">- amplitude of sin curve</param>
/// <param name="t">- time taken to complete one sin</param>
void AM_SetFloat(Animation* a, CP_Vector p, float amplitude, float t)
{
	a->type = ANIM_TRANSLATEFLOAT;
	a->oldPos = p;
	a->rotateAngle = amplitude;
	a->loopTime = t;
	a->elapsedTime = 0.0f;
}
/// <summary>
/// Set Animation that contain states and uses flag for object to (appear) move up, float and down (disappear)
/// treat this like a specified state machine animator
/// </summary>
/// <param name="a">- animation to be set</param>
/// <param name="pa">- from position</param>
/// <param name="pb">- floating position</param>
/// <param name="amplitude">- amplitude of float</param>
/// <param name="t">- time taken to go from start pos to floating pos</param>
/// <param name="ft">- time taken for float to complete one sin curve</param>
void AM_SetUpFloatDownState(Animation* a, CP_Vector pa, CP_Vector pb, float amplitude, float t, float ft)
{
	a->type = ANIM_UFDSTATEUP;
	a->defaultScale = pa;
	a->targetScale = pb;
	a->scaleToTime = t;
	a->rotateAngle = amplitude;
	a->elapsedTime = 0.0f;

	a->oldPos = a->targetScale;
	a->loopTime = ft;
	a->isNextStateFlag = 0;
	a->isFlipped = 0;
}
