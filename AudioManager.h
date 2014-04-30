#ifndef __AudioManager_h_
#define __AudioManager_h_

#include "common.h"
#include "Singleton.h"

#define AUDIO_RATE 22050
#define AUDIO_FORMAT AUDIO_S16
#define AUDIO_CHANNELS 2
#define AUDIO_BUFFERS 1024

#define WEAPON_SIZE 5
#define SOUNDS_NUM_PER_WEAPON 5

#define DEFAULT_CHUNK_VOLUME 128

typedef unsigned int AudioFile;
typedef int AudioChannel;

class AudioManager : public Singleton<AudioManager> {
  public:
  	void initialize();
  	AudioFile    loadAudioFile(const char* file);
    AudioChannel play2DSound(AudioFile, int loops);
    void play3DSound(AudioFile, int loops, Ogre::Vector3 v, uint8_t sound_volume = DEFAULT_CHUNK_VOLUME);
  	void stopChannel(AudioChannel);
  	void playDonk();
  	void playStartSound();
    void playBALLZ();
    void playWeaponFire(Ogre::Vector3 v, uint32_t weapon_id, uint32_t sound_volume = DEFAULT_CHUNK_VOLUME);
    void playBulletDirtCollision(Ogre::Vector3 v);
    void playDeath(Ogre::Vector3 v);
    void playWalkStep(Ogre::Vector3 v);
    void playJetPack(Ogre::Vector3 v);
    void playReload(Ogre::Vector3 v, uint32_t);
    void playBlootSplat(Ogre::Vector3 v);
    void playBlasterCharge(Ogre::Vector3 v, uint32_t sound_volume = DEFAULT_CHUNK_VOLUME);
    void playOutOfAmmo(Ogre::Vector3 v);
    void playPickWeapon(Ogre::Vector3 v);

  private:
  	std::vector<Mix_Chunk*> _loadedFiles;
  	AudioFile _donks[4];
  	AudioFile _startSound;
    AudioFile _ballzSound;
    AudioFile _rifleFire[WEAPON_SIZE][SOUNDS_NUM_PER_WEAPON];
    AudioFile _bulletDirtCollision[7];
    AudioFile _death;
    AudioFile _footstep[7];
    AudioFile _jetpack;
    AudioFile _reloads[WEAPON_SIZE];
    AudioFile _blood_splats[3];
    AudioFile _blaster_charge;
    AudioFile _out_of_ammo;
    AudioFile _pick_weapon;
};

#endif
