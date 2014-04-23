#include <OgreSceneManager.h>
#include "ParticleManager.h"

ParticleManager::ParticleManager(){
}
ParticleManager::~ParticleManager(){
}

Ogre::ParticleSystem* ParticleManager::Emit(std::string particleSystemName,Ogre::Vector3 origin,
    Ogre::Vector3 direction){
  Ogre::SceneManager* sceneManager=SceneManager::instance()->current_scene->manager;
  Ogre::SceneNode* node=sceneManager->createSceneNode();
  sceneManager->getRootSceneNode()->addChild(node);
  positionAndDirectSceneNode(node,origin,direction);
  return Emit(particleSystemName,node);
}

Ogre::ParticleSystem* ParticleManager::Emit(std::string particleSystemName,Ogre::Vector3 origin,
    Ogre::Vector3 direction,Ogre::SceneNode* parentNode){
  Ogre::SceneNode* node{parentNode->createChildSceneNode()};
  positionAndDirectSceneNode(node,origin,direction);
  return Emit(particleSystemName,node);
}

Ogre::ParticleSystem* ParticleManager::Emit(std::string particleSystemName,Ogre::SceneNode* node){
  Ogre::ParticleSystem* system{lookUpSystem(particleSystemName)};
  if(system->isInScene()){
    Ogre::SceneNode* formerNode = system->getParentSceneNode();
    system->detachFromParent();
    formerNode->getParentSceneNode()->removeAndDestroyChild(formerNode->getName());
  }
  node->attachObject(system);
  unsigned short numberOfEmitters{system->getNumEmitters()};
  for(unsigned short i; i!=numberOfEmitters;++i){
    system->getEmitter(i)->setEnabled(true);
  }
  return system;
}

Ogre::ParticleSystem* ParticleManager::lookUpSystem(std::string particleSystemName){
  auto range=_systems.equal_range(particleSystemName);
  Ogre::ParticleSystem* system{nullptr};
  int howMany{1};
  for(auto systemElement=range.first;systemElement!=range.second;++systemElement){
    if(!(*systemElement).second->getEmitter(0)->getEnabled()){
      system=(*systemElement).second.get();
      break;
    }
    ++howMany;
  }
  if(system==nullptr){
    Ogre::SceneManager* sceneManager=SceneManager::instance()->current_scene->manager;
    system=sceneManager->createParticleSystem(particleSystemName+std::to_string(howMany),particleSystemName);
    _systems.insert(std::make_pair(particleSystemName,std::unique_ptr<Ogre::ParticleSystem>{system}));
  }
  return system;
}

void ParticleManager::positionAndDirectSceneNode(Ogre::SceneNode* node,Ogre::Vector3 origin, Ogre::Vector3 direction){
  node->setPosition(origin);
  node->setDirection(direction);
}

std::tuple<Ogre::ParticleSystem*,Ogre::ParticleSystem*,Ogre::ParticleSystem*> ParticleManager::EmitRocketExplosion(Ogre::Vector3 origin){
  return std::make_tuple(ParticleManager::instance()->Emit("OrangeExplosion",origin,
      Ogre::Vector3{0,1,0}),ParticleManager::instance()->Emit("YellowExplosion",
      origin,Ogre::Vector3{0,1,0}),ParticleManager::instance()->Emit("OrangeExplosionCloud",
      origin,Ogre::Vector3{0,1,0}));
}
std::tuple<Ogre::ParticleSystem*,Ogre::ParticleSystem*,Ogre::ParticleSystem*> ParticleManager::EmitSparks(Ogre::Vector3 origin,Ogre::Vector3 direction){
  return std::make_tuple(ParticleManager::instance()->Emit("JetPackYellowSparks",origin,
      direction),ParticleManager::instance()->Emit("JetPackOrangeSparks",
      origin,direction),ParticleManager::instance()->Emit("JetPackSmoke",
      origin,-direction));
}

Ogre::ParticleSystem* ParticleManager::EmitBloodSpurt(Ogre::Vector3 origin,Ogre::Vector3 direction){
  return ParticleManager::instance()->Emit("BloodSpurt",origin,direction);
}

Ogre::ParticleSystem* ParticleManager::EmitDust(Ogre::Vector3 origin,Ogre::Vector3 direction){
  AudioManager::instance()->playBulletDirtCollision(origin);
  return ParticleManager::instance()->Emit("DustCloud",origin,direction);
}
