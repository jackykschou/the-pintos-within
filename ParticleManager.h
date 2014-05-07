#ifndef __ParticleManager_h_
#define __ParticleManager_h_

#include <memory>
#include <map>
#include <tuple>
#include <OgreParticleSystem.h>
#include <OgreParticleEmitter.h>
#include <OgreVector3.h>
#include "Singleton.h"
#include "SceneManager.h"

class ParticleManager:public Singleton<ParticleManager>{
  public:
    ParticleManager();
    virtual ~ParticleManager();
    Ogre::ParticleSystem* Emit(std::string particleSystemName,Ogre::Vector3 origin,
        Ogre::Vector3 direction, float duration = -1);
    Ogre::ParticleSystem* Emit(std::string particleSystemName,Ogre::Vector3 origin,
        Ogre::Vector3 direction,Ogre::SceneNode* parentNode, float duration = -1);
    Ogre::ParticleSystem* Emit(std::string particleSystemName,Ogre::SceneNode* node, float duration = -1);
    std::tuple<Ogre::ParticleSystem*,Ogre::ParticleSystem*,Ogre::ParticleSystem*> EmitRocketExplosion(Ogre::Vector3 origin, float radius);
    std::tuple<Ogre::ParticleSystem*,Ogre::ParticleSystem*,Ogre::ParticleSystem*> EmitSparks(Ogre::Vector3 origin,Ogre::Vector3 direction);
    Ogre::ParticleSystem* EmitBloodSpurt(Ogre::Vector3 origin,Ogre::Vector3 direction);
    Ogre::ParticleSystem* EmitDust(Ogre::Vector3 origin,Ogre::Vector3 direction);
  private:
    std::multimap<std::string,std::unique_ptr<Ogre::ParticleSystem>> _systems;
    Ogre::ParticleSystem* lookUpSystem(std::string particleSystemName);
    void positionAndDirectSceneNode(Ogre::SceneNode* node,Ogre::Vector3 origin, Ogre::Vector3 direction);
};
#endif
