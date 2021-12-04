/*
@file	SoundManager.c
@author	Tan Wee Yi (weeyi.t)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@brief	Script that handles the sound that is being used in the game.
*//*______________________________________________________________________*/

#include "cprocessing.h"
#include "SoundManager.h"
#include <time.h>

CP_Sound bg, bg2, foot, wood1, wood2, wood3, electric1, electric2, electric3, hurt1, hurt2, hurt3, drop1;

float volBg, volSfx;
static int bgVolume, sfxVolume;
int playing = 0;
time_t startT;


/// <summary>
/// Inits and loads all the sound to memory.
/// </summary>
/// <param name=""></param>
void SDM_Init(void) {
	bg = CP_Sound_LoadMusic("Assets/Sounds/MainBg.ogg");
	bg2 = CP_Sound_LoadMusic("Assets/Sounds/bg2.ogg");
	foot = CP_Sound_Load("Assets/Sounds/footstep.ogg");
	hurt1 = CP_Sound_Load("Assets/Sounds/hurt1.ogg");
	hurt2 = CP_Sound_Load("Assets/Sounds/hurt2.ogg");
	hurt3 = CP_Sound_Load("Assets/Sounds/hurt3.ogg");
	electric1 = CP_Sound_Load("Assets/Sounds/electric1.ogg");
	electric2 = CP_Sound_Load("Assets/Sounds/electric1.ogg");
	electric3 = CP_Sound_Load("Assets/Sounds/electric1.ogg");
	drop1 = CP_Sound_Load("Assets/Sounds/drop1.ogg");
	wood1 = CP_Sound_Load("Assets/Sounds/wood1.ogg");
}

/// <summary>
/// Handles the track to be played while initializing the sound for background
/// </summary>
/// <param name="name"></param>
void SDM_PlayBgMusic(int name) {
	
	switch (name) {
	case 1:
		CP_Sound_PlayAdvanced(bg , 0.7f, 1.0f, TRUE, CP_SOUND_GROUP_MUSIC);
		break;
	case 2:
		CP_Sound_PlayAdvanced(bg2, 0.7f, 1.0f, TRUE, CP_SOUND_GROUP_MUSIC);
		break;
	}
}

/// <summary>
/// Changes the background music to another track.
/// </summary>
/// <param name="name"></param>
void SDM_ChangeBgMusic(int name) {
	CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
	SDM_PlayBgMusic(name);
}

/// <summary>
/// Pauses the background music 
/// </summary>
/// <param name=""></param>
void SDM_PauseBgMusic(void) {
	CP_Sound_PauseGroup(CP_SOUND_GROUP_MUSIC);
}

/// <summary>
/// Resumes playing the background music
/// </summary>
/// <param name=""></param>
void SDM_ResumeBgMusic(void) {
	CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);
}

/// <summary>
/// Stops the background music
/// </summary>
/// <param name=""></param>
void SDM_StopBgMusic(void) {
	CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
}

/// <summary>
/// Plays the walking effect and also updates a timer so that the footsteps can be timed
/// </summary>
/// <param name=""></param>
void SDM_PlayWEffect(void) {
	float random = CP_Random_RangeFloat(0.5f, 1.0f);
	if (playing == 0) {
		CP_Sound_PlayAdvanced(foot, 0.5f, random, FALSE, CP_SOUND_GROUP_SFX);
		playing = 1;
		startT = clock();
	}
}

/// <summary>
/// Updates the walking effect so that it can be timed properly
/// </summary>
/// <param name=""></param>
void SDM_EffectUpdate(void) {
	time_t Current_T = clock();

	if (difftime(Current_T, startT) >= 400.0f) {
		playing = 0;
	}
}

/// <summary>
/// Plays the sfx that is stored in the sound manager 
/// </summary>
/// <param name="name"></param>
void SDM_PlaySFX(int name) {
	switch (name) {
	case 1:
		CP_Sound_PlayAdvanced(hurt1, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_SFX);
		break;
	case 2:
		CP_Sound_PlayAdvanced(hurt2, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_SFX);
		break;
	case 3:
		CP_Sound_PlayAdvanced(hurt3, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_SFX);
		break;
	case 4:
		CP_Sound_PlayAdvanced(electric1, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_SFX);
		break;
	case 5:
		CP_Sound_PlayAdvanced(electric2, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_SFX);
		break;
	case 6:
		CP_Sound_PlayAdvanced(electric3, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_SFX);
		break;
	case 7:
		CP_Sound_PlayAdvanced(drop1, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_SFX);
		break;
	case 8:
		CP_Sound_PlayAdvanced(wood1, 1.0f, 1.0f, FALSE, CP_SOUND_GROUP_SFX);
		break;
	}
}

/// <summary>
/// Stops all the music that is in the sound system
/// </summary>
/// <param name=""></param>
void SDM_StopAll(void) {
	CP_Sound_StopAll();
}

/// <summary>
/// Frees sound from memory
/// </summary>
/// <param name=""></param>
void SDM_FreeSounds(void) {
	CP_Sound_Free(&bg);
	CP_Sound_Free(&foot);
	CP_Sound_Free(&hurt1);
	CP_Sound_Free(&hurt2);
	CP_Sound_Free(&hurt3);
	CP_Sound_Free(&electric1);
	CP_Sound_Free(&electric2);
	CP_Sound_Free(&electric3);
	CP_Sound_Free(&drop1);
	CP_Sound_Free(&wood1);
	CP_Sound_Free(&bg2);
	
}

/// <summary>
/// Sets the BG volume
/// </summary>
/// <param name="v">Int 1 - 100 for volume </param>
void SDM_SetBGVolume(int v)
{
	bgVolume = v;
	volBg = (1.0f / 100.0f) * bgVolume;
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC, volBg);
}

/// <summary>
/// returns the bg volume value so that it can be updated in an external file
/// </summary>
/// <returns> volume of bg music </returns>
int SDM_GetBGVolume()
{
	return bgVolume;
}

/// <summary>
/// sets the SFX volume.
/// </summary>
/// <param name="v">int 1 - 100 as volume </param>
void SDM_SetSFXVolume(int v)
{
	sfxVolume = v;
	volSfx = (1.0f / 100.0f) * sfxVolume;
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX, volSfx);
}

/// <summary>
/// Returns the SFX volume so that it can be stored elsewhere
/// </summary>
/// <returns> int 1- 100 </returns>
int SDM_GetSFXVolume()
{
	return sfxVolume;
}