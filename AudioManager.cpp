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

	_rifleFire[0][0] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire2.wav");
	_rifleFire[0][1] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire2.wav");
	_rifleFire[0][2] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire3.wav");
	_rifleFire[0][3] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire3.wav");
	_rifleFire[0][4] = AudioManager::instance()->loadAudioFile("media/sounds/PistolFire3.wav");

	_rifleFire[1][0] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire2.wav");
	_rifleFire[1][1] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire2.wav");
	_rifleFire[1][2] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire2.wav");
	_rifleFire[1][3] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire2.wav");
	_rifleFire[1][4] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunFire2.wav");

	_rifleFire[2][0] = AudioManager::instance()->loadAudioFile("media/sounds/AssaultRifeFire2.wav");
	_rifleFire[2][1] = AudioManager::instance()->loadAudioFile("media/sounds/AssaultRifeFire2.wav");
	_rifleFire[2][2] = AudioManager::instance()->loadAudioFile("media/sounds/AssaultRifeFire2.wav");
	_rifleFire[2][3] = AudioManager::instance()->loadAudioFile("media/sounds/AssaultRifeFire2.wav");
	_rifleFire[2][4] = AudioManager::instance()->loadAudioFile("media/sounds/AssaultRifeFire2.wav");

	_rifleFire[3][0] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][1] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][2] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][3] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");
	_rifleFire[3][4] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterFire1.wav");

	_rifleFire[4][0] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][1] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][2] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][3] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");
	_rifleFire[4][4] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");

	_bulletDirtCollision[0] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision1.wav");
	_bulletDirtCollision[1] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision2.wav");
	_bulletDirtCollision[2] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision3.wav");
	_bulletDirtCollision[3] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision3.wav");
	_bulletDirtCollision[4] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision1.wav");
	_bulletDirtCollision[5] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision2.wav");
	_bulletDirtCollision[6] = AudioManager::instance()->loadAudioFile("media/sounds/Bullet_Dirt_Collision2.wav");

	_footstep[0] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep1.wav");
	_footstep[1] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep2.wav");
	_footstep[2] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep3.wav");
	_footstep[3] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep4.wav");
	_footstep[4] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep5.wav");
	_footstep[5] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep6.wav");
	_footstep[6] = AudioManager::instance()->loadAudioFile("media/sounds/Footstep7.wav");

	_jetpack = AudioManager::instance()->loadAudioFile("media/sounds/Jetpack1.wav");

	_death = AudioManager::instance()->loadAudioFile("media/sounds/Death1.wav");

	_reloads[0] = AudioManager::instance()->loadAudioFile("media/sounds/PistolReload.wav");
	_reloads[1] = AudioManager::instance()->loadAudioFile("media/sounds/ShotGunReload.wav");
	_reloads[2] = AudioManager::instance()->loadAudioFile("media/sounds/AssaultReload.wav");
	_reloads[3] = AudioManager::instance()->loadAudioFile("media/sounds/BlasterReload.wav");
	_reloads[4] = AudioManager::instance()->loadAudioFile("media/sounds/Knife.wav");

	_blood_splats[0] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat1.wav");
	_blood_splats[1] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat2.wav");
	_blood_splats[2] = AudioManager::instance()->loadAudioFile("media/sounds/BloodSplat2.wav");

	_blaster_charge = AudioManager::instance()->loadAudioFile("media/sounds/BlasterCharge.wav");
	_out_of_ammo = AudioManager::instance()->loadAudioFile("media/sounds/OutOfAmmo.wav");
	_pick_weapon = AudioManager::instance()->loadAudioFile("media/sounds/PickWeapon.wav");
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

void AudioManager::play3DSound(AudioFile file, int loops, Ogre::Vector3 v, uint8_t sound_volume)
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

	_loadedFiles[file]->volume = sound_volume;

	if (volume > 0) 
	{
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

void AudioManager::playWeaponFire(Ogre::Vector3 v, uint32_t weapon_id, uint32_t sound_volume) {
	play3DSound(_rifleFire[weapon_id][RAND_RANGE(0, SOUNDS_NUM_PER_WEAPON)], 0, v, sound_volume);
}

void AudioManager::playBulletDirtCollision(Ogre::Vector3 v) {
	play3DSound(_bulletDirtCollision[RAND_RANGE(0, 7)], 0, v);
}

void AudioManager::playWalkStep(Ogre::Vector3 v) {
	play3DSound(_footstep[RAND_RANGE(0, 7)], 0, v);
}

void AudioManager::playJetPack(Ogre::Vector3 v) {
	play3DSound(_jetpack, 0, v);
}

void AudioManager::playReload(Ogre::Vector3 v, uint32_t weapon_id) 
{
	play3DSound(_reloads[weapon_id], 0, v);
}

void AudioManager::playDeath(Ogre::Vector3 v) {
	play3DSound(_death, 0, v);
}

void AudioManager::playBlootSplat(Ogre::Vector3 v)
{
	play3DSound(_blood_splats[RAND_RANGE(0, 3)], 0, v);
}

void AudioManager::playBlasterCharge(Ogre::Vector3 v, uint32_t sound_volume)
{
	play3DSound(_blaster_charge, 0, v, sound_volume);
}

void AudioManager::playOutOfAmmo(Ogre::Vector3 v)
{
	play3DSound(_out_of_ammo, 0, v);
}

void AudioManager::playPickWeapon(Ogre::Vector3 v)
{
	play3DSound(_pick_weapon, 0, v);
}