#ifndef __AudioManager_h_
#define __AudioManager_h_

#include "common.h"
#include "Singleton.h"

#define AUDIO_RATE 22050
#define AUDIO_FORMAT AUDIO_S16
#define AUDIO_CHANNELS 2
#define AUDIO_BUFFERS 1024

typedef unsigned int AudioFile;
typedef int AudioChannel;

class AudioManager : public Singleton<AudioManager> {
  public:
  	void initialize();
  	AudioFile    loadAudioFile(const char* file);
    AudioChannel play2DSound(AudioFile, int loops);
    void play3DSound(AudioFile, int loops, Ogre::Vector3 v);
  	void stopChannel(AudioChannel);
  	void playDonk();
  	void playStartSound();
    void playBALLZ();
    void playRifleFire(Ogre::Vector3 v);
    void playBulletDirtCollision(Ogre::Vector3 v);
    void playDeath(Ogre::Vector3 v);
    void playWalkStep(Ogre::Vector3 v);
    void playJetPack(Ogre::Vector3 v);

  private:
  	std::vector<Mix_Chunk*> _loadedFiles;
  	AudioFile _donks[4];
  	AudioFile _startSound;
    AudioFile _ballzSound;
    AudioFile _rifleFire[7];
    AudioFile _bulletDirtCollision[7];
    AudioFile _death;
    AudioFile _footstep[7];
    AudioFile _jetpack;
};

#endif
