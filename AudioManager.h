#ifndef __AudioManager_h_
#define __AudioManager_h_

#include "common.h"
#include "Singleton.h"

#define AUDIO_RATE 22050
#define AUDIO_FORMAT AUDIO_S16
#define AUDIO_CHANNELS 2
#define AUDIO_BUFFERS 4096

typedef unsigned int AudioFile;
typedef int AudioChannel;

class AudioManager : public Singleton<AudioManager> {
  public:
  	void initialize();
  	AudioFile    loadAudioFile(const char* file);
  	AudioChannel play2DSound(AudioFile, int loops);
  	void stopChannel(AudioChannel);
  	AudioChannel playDonk();
  	AudioChannel playStartSound();
  	AudioChannel playBALLZ();

  private:
  	std::vector<Mix_Chunk*> _loadedFiles;
  	AudioFile _donks[4];
  	AudioFile _startSound;
};

#endif
