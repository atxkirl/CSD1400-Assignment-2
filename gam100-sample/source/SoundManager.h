/*
@file	SoundManager.c
@author	Tan Wee Yi (weeyi.t)
@course	CSD1400 Software Engineering Project 1
@Team	BananaBoi
@brief	Script that handles the sound that is being used in the game.
*//*______________________________________________________________________*/

//command Grp : SDM 

/// <summary>
/// Inits and loads all the sound to memory.
/// </summary>
/// <param name=""></param>
void SDM_Init(void);

/// <summary>
/// Handles the track to be played while initializing the sound for background
/// </summary>
/// <param name="name"></param>
void SDM_PlayBgMusic(int name);

/// <summary>
/// Changes the background music to another track.
/// </summary>
/// <param name="name"></param>
void SDM_ChangeBgMusic(int name);

/// <summary>
/// Plays the walking effect and also updates a timer so that the footsteps can be timed
/// </summary>
/// <param name=""></param>
void SDM_PauseBgMusic(void);

/// <summary>
/// Resumes playing the background music
/// </summary>
/// <param name=""></param>
void SDM_ResumeBgMusic(void);

/// <summary>
/// Stops the background music
/// </summary>
/// <param name=""></param>
void SDM_StopBgMusic(void);

/// <summary>
/// Plays the walking effect and also updates a timer so that the footsteps can be timed
/// </summary>
/// <param name=""></param>
void SDM_PlayWEffect(void);

/// <summary>
/// Plays the sfx that is stored in the sound manager 
/// </summary>
/// <param name="name"></param>
void SDM_PlaySFX(int name);

/// <summary>
/// Updates the walking effect so that it can be timed properly
/// </summary>
/// <param name=""></param>
void SDM_EffectUpdate(void);

/// <summary>
/// Frees sound from memory
/// </summary>
/// <param name=""></param>
void SDM_FreeSounds(void);

/// <summary>
/// Stops all the music that is in the sound system
/// </summary>
/// <param name=""></param>
void SDM_StopAll(void);

/// <summary>
/// Sets the BG volume
/// </summary>
/// <param name="v">Int 1 - 100 for volume </param>
void SDM_SetBGVolume(int);

/// <summary>
/// returns the bg volume value so that it can be updated in an external file
/// </summary>
/// <returns> volume of bg music </returns>
int SDM_GetBGVolume();

/// <summary>
/// sets the SFX volume.
/// </summary>
/// <param name="v">int 1 - 100 as volume </param>
void SDM_SetSFXVolume(int);

/// <summary>
/// Returns the SFX volume so that it can be stored elsewhere
/// </summary>
/// <returns> int 1- 100 </returns>
int SDM_GetSFXVolume();