#include "AudioManager.h"

void AudioManager::initialize() {
	SDL_Init(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS)) {
	    throw("Unable to open audio channel.");
	}
	_donks[0] = AudioManager::instance()->loadAudioFile("media/sounds/Donk1.wav");
	_donks[1] = AudioManager::instance()->loadAudioFile("media/sounds/Donk2.wav");
	_donks[2] = AudioManager::instance()->loadAudioFile("media/sounds/Donk3.wav");
	_donks[3] = AudioManager::instance()->loadAudioFile("media/sounds/Donk4.wav");
}

// returns an audioFileKey that references the loaded resource
AudioFile AudioManager::loadAudioFile(const char *file) {
	Mix_Chunk* c = Mix_LoadWAV(file);
	if (!c) {
		std::cout << "Failed to load file " << file << std::endl;
		printf("Error Mix_LoadWAV: %s\n", Mix_GetError());
		return -1;
	}
	_loadedFiles.push_back(c);
	return _loadedFiles.size() - 1;
}

// returns a channel integer that can be used to stop the sound
AudioChannel AudioManager::play2DSound(AudioFile file, int loops)
{
	return Mix_PlayChannel(-1, _loadedFiles[file], loops);
}

void AudioManager::stopChannel(AudioChannel channel) {
	Mix_HaltChannel(channel);
}

AudioChannel AudioManager::playDonk() {
	return play2DSound(_donks[rand() % 4], 0);
}
