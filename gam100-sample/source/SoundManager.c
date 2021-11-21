#include "cprocessing.h"
#include "SoundManager.h"

CP_Sound bg, bg2, foot, wood1, wood2, wood3, electric1, electric2, electric3, hurt1, hurt2, hurt3, drop1;

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

void SDM_PauseBgMusic(void) {
	CP_Sound_PauseGroup(CP_SOUND_GROUP_MUSIC);
}

void SDM_ResumeBgMusic(void) {
	CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);
}

void SDM_StopBgMusic(void) {
	CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
}

void SDM_PlayWEffect(void) {
	float random = CP_Random_RangeFloat(0.5f, 1.0f);
	int playing = 0;
	while (playing == 0) {
		CP_Sound_PlayAdvanced(foot, 1.0f, random, FALSE, CP_SOUND_GROUP_SFX);
		playing = 1;
	}
	
}

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

void SDM_StopAll(void) {
	CP_Sound_StopAll();
}

void SDM_StopWEffect(void) {
	CP_Sound_StopGroup(CP_SOUND_GROUP_0);
}

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
	
}