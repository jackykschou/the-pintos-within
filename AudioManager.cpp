#include "AudioManager.h"

void AudioManager::initialize() {
	SDL_Init(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS)) {
	    throw("Unable to open audio channel.");
	}
}

// returns an audioFileKey that references the loaded resource
AudioFile AudioManager::loadAudioFile(const char *file) {
	_loadedFiles.push_back(Mix_LoadWAV(file));
	return _loadedFiles.size() - 1;
}

// returns a channel integer that can be used to stop the sound
AudioChannel AudioManager::play2DSound(AudioFile file, int loops) {
	return Mix_PlayChannel(-1, _loadedFiles[file], loops);
}

void AudioManager::stopChannel(AudioChannel channel) {
	Mix_HaltChannel(channel);
}