#include "AudioManager.h"
#include "GameState.h"
#include "PlayerCharacter.h"
void AudioManager::initialize() {
	SDL_Init(SDL_INIT_AUDIO);
	if (Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS, AUDIO_BUFFERS)) {
	    throw("Unable to open audio channel.");
	}
	_donks[0] = AudioManager::instance()->loadAudioFile("media/sounds/Donk1.wav");
	_donks[1] = AudioManager::instance()->loadAudioFile("media/sounds/Donk2.wav");
	_donks[2] = AudioManager::instance()->loadAudioFile("media/sounds/Donk3.wav");
	_donks[3] = AudioManager::instance()->loadAudioFile("media/sounds/Donk4.wav");
	_startSound = AudioManager::instance()->loadAudioFile("media/sounds/Balls.wav");
	_ballzSound = AudioManager::instance()->loadAudioFile("media/sounds/BallBlasterIntro.wav");
	_rifleFire[0] = AudioManager::instance()->loadAudioFile("media/sounds/RifleFire1.wav");
	_rifleFire[1] = AudioManager::instance()->loadAudioFile("media/sounds/RifleFire2.wav");
	_rifleFire[2] = AudioManager::instance()->loadAudioFile("media/sounds/RifleFire3.wav");
	_rifleFire[3] = AudioManager::instance()->loadAudioFile("media/sounds/RifleFire4.wav");
	_rifleFire[4] = AudioManager::instance()->loadAudioFile("media/sounds/RifleFire5.wav");
	_rifleFire[5] = AudioManager::instance()->loadAudioFile("media/sounds/RifleFire6.wav");
	_rifleFire[6] = AudioManager::instance()->loadAudioFile("media/sounds/RifleFire7.wav");
	_bulletDirtCollision[0] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision1.wav");
	_bulletDirtCollision[1] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision2.wav");
	_bulletDirtCollision[2] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision3.wav");
	_bulletDirtCollision[3] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision4.wav");
	_bulletDirtCollision[4] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision5.wav");
	_bulletDirtCollision[5] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision6.wav");
	_bulletDirtCollision[6] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision7.wav");
	_death = AudioManager::instance()->loadAudioFile("media/sounds/Death.wav");
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

void AudioManager::play3DSound(AudioFile file, int loops, Ogre::Vector3 v)
{
	if(GameState::instance()->player == NULL)
		return;
	
	Ogre::Vector3 curPos = Ogre::Vector3(GameState::instance()->player->tr->posX, GameState::instance()->player->tr->posY, GameState::instance()->player->tr->posZ);
	
	//max dist = 100m 
	//min dist = 1m  64
	int maxDist = 500;
	float distance = abs((v-curPos).length());
	int volume = 64;
	if (distance > 1)
		volume -= (int)(64.0*distance/maxDist);
	// LOG("Distance: " << distance);
	if (volume > 0) {
		// LOG("Volume: " << volume);
		int chan = Mix_PlayChannel(-1, _loadedFiles[file], loops);
		Mix_Volume(chan, volume);
	}

	//return Mix_PlayChannel(-1, _loadedFiles[file], loops);
}

void AudioManager::stopChannel(AudioChannel channel) {
	Mix_HaltChannel(channel);
}

void AudioManager::playDonk() {
	play2DSound(_donks[rand() % 4], 0);
}

void AudioManager::playStartSound() {
	play2DSound(_startSound, 0);
}

void AudioManager::playBALLZ() {
	play2DSound(_ballzSound, 0);
}

void AudioManager::playRifleFire(Ogre::Vector3 v) {
	play3DSound(_rifleFire[rand() % 7], 0, v);
}

void AudioManager::playBulletDirtCollision(Ogre::Vector3 v) {
	play3DSound(_bulletDirtCollision[rand() % 7], 0, v);
}

void AudioManager::playDeath(Ogre::Vector3 v) {
	play3DSound(_death, 0, v);
}
