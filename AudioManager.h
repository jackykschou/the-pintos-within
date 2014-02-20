#ifndef __AudioManager_h_
#define __AudioManager_h_

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "common.h"

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

  private:
  	std::vector<Mix_Chunk*> _loadedFiles;
};

#endif
