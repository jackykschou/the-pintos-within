#ifndef __ParticleManager_h_
#define __ParticleManager_h_

#include <memory>
#include <map>
#include <OgreParticleSystem.h>
#include <OgreVector3.h>
#include "Singleton.h"

class ParticleManager:public Singleton<ParticleManager>{
  public:
    ParticleManager();
    virtual ~ParticleManager();
    void Emit(Ogre::Vector3 origin, Ogre::Vector3 direction, std::string particleSystemName);
  private:
    std::multimap<std::string,std::unique_ptr<Ogre::ParticleSystem>> _systems;
};
#endif
