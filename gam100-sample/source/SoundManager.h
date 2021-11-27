//command Grp : SDM 

void SDM_Init(void);

void SDM_PlayBgMusic(int name);

void SDM_PauseBgMusic(void);

void SDM_ResumeBgMusic(void);

void SDM_StopBgMusic(void);

void SDM_PlayWEffect(void);

void SDM_StopWEffect(void);

void SDM_PlaySFX(int name);

void SDM_FreeSounds(void);

void SDM_StopAll(void);

void SDM_EffectUpdate(void);

void walk_timer(void);

/*!
@brief Sets the BGM volume
@param int - the volume 0-100 to be set
@return void
*/
void SDM_SetBGVolume(int);
/*!
@brief Gets the BGM volume
@return int - volume of BGM 0 - 100
*/
int SDM_GetBGVolume();
/*!
@brief Sets the SFX volume
@param int - the volume 0-100 to be set
@return void
*/
void SDM_SetSFXVolume(int);
/*!
@brief Gets the SFX volume
@return int - volume of SFX 0 - 100
*/
int SDM_GetSFXVolume();