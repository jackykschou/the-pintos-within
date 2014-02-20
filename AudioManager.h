#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "common.h"

class AudioManager : public Singleton<AudioManager> {
  public:
  	void initialize();
};
