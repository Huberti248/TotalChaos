#include "AudioManager.h"

AudioManager* AudioManager::instance = NULL;

AudioManager* AudioManager::Instance() {
	if (instance == NULL) {
		instance = new AudioManager();
	}

	return instance;
}

void AudioManager::Release() {
	for (int i = 0; i < instance->music.size(); ++i) {
		Mix_FreeMusic(instance->music[i].second);
		instance->music[i].second = NULL;
	}
	instance->music.clear();

	for (int i = 0; i < instance->sfx.size(); ++i) {
		Mix_FreeChunk(instance->sfx[i].second);
		instance->sfx[i].second = NULL;
	}
	instance->sfx.clear();

	delete instance;
	instance = NULL;
}

AudioManager::AudioManager() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else {
		AudioInit();
	}
}

AudioManager::~AudioManager() {
	Mix_Quit();
}

void AudioManager::PlayMusic(MusicAudio name, int loops) {
	for (int i = 0; i < music.size(); ++i) {
		if (name == music[i].first) {
			Mix_PlayMusic(music[i].second, loops);
			return;
		}
	}
}

void AudioManager::PauseMusic() {
	if (Mix_PlayingMusic() != 0) {
		Mix_PauseMusic();
	}
}

void AudioManager::ResumeMusic() {
	if (Mix_PlayingMusic() != 0) {
		Mix_ResumeMusic();
	}
}

void AudioManager::StopMusic() {
	if (Mix_PlayingMusic() != 0) {
		Mix_HaltMusic();
	}
}

void AudioManager::PlaySFX(SFXAudio name, int loops, int channel) {
	for (int i = 0; i < sfx.size(); ++i) {
		if (name == sfx[i].first) {
			Mix_PlayChannel(channel, sfx[i].second, loops);
			return;
		}
	}
}

void AudioManager::AudioInit() {

	music.push_back(std::make_pair<MusicAudio, Mix_Music*>(
		MusicAudio::Background, 
		Mix_LoadMUS("res/music/ost1.ogg")
		)
	);

	sfx.push_back(std::make_pair<SFXAudio, Mix_Chunk*>(
		SFXAudio::UI,
		Mix_LoadWAV("res/sound\ fx/ui.ogg")
		)
	);

	sfx.push_back(std::make_pair<SFXAudio, Mix_Chunk*>(
		SFXAudio::EnemyDeath,
		Mix_LoadWAV("res/sound\ fx/explosion.ogg")
		)
	);

	sfx.push_back(std::make_pair<SFXAudio, Mix_Chunk*>(
		SFXAudio::PlayerFire,
		Mix_LoadWAV("res/sound\ fx/gun_sound_3.ogg")
		)
	);

	sfx.push_back(std::make_pair<SFXAudio, Mix_Chunk*>(
		SFXAudio::EnemyFire,
		Mix_LoadWAV("res/sound\ fx/gun_sound_1.ogg")
		)
	);

	sfx.push_back(std::make_pair<SFXAudio, Mix_Chunk*>(
		SFXAudio::PlayerHit,
		Mix_LoadWAV("res/sound\ fx/hit.ogg")
		)
	);

	Mix_Volume(-1, VOLUME);
	Mix_AllocateChannels(MAX_CHANNELS);
}