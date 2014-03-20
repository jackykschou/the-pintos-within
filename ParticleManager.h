#ifndef __ParticleManager_h_
#define __ParticleManager_h_

#include <memory>
#include <map>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreVector3.h>
#include "Singleton.h"

class ParticleManager:public Singleton<ParticleManager>{
  public:
    ParticleManager();
    virtual ~ParticleManager();
    Ogre::ParticleSystem* Emit(std::string particleSystemName,Ogre::Vector3 origin,
        Ogre::Vector3 direction);
    Ogre::ParticleSystem* Emit(std::string particleSystemName,Ogre::Vector3 origin,
        Ogre::Vector3 direction,Ogre::SceneNode* parentNode);
    Ogre::ParticleSystem* Emit(std::string particleSystemName,Ogre::SceneNode* node);
  private:
    std::multimap<std::string,std::unique_ptr<Ogre::ParticleSystem>> _systems;
    Ogre::ParticleSystem* lookUpSystem(std::string particleSystemName);
    void positionAndDirectSceneNode(Ogre::SceneNode* node,Ogre::Vector3 origin, Ogre::Vector3 direction);
};
#endif
