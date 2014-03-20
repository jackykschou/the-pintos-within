#include <OgreSceneManager.h>
#include "ParticleManager.h"

ParticleManager::ParticleManager(){
}
ParticleManager::~ParticleManager(){
}

Ogre::ParticleSystem* ParticleManager::Emit(std::string particleSystemName,Ogre::Vector3 origin,
    Ogre::Vector3 direction){
  Ogre::SceneManager* sceneManager=SceneManager::instance()->current_scene->manager;
  //extern Ogre::SceneManager* sceneManager;
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
  system->getEmitter(0)->setEnabled(true);
  return system;
}

Ogre::ParticleSystem* ParticleManager::lookUpSystem(std::string particleSystemName){
  auto range=_systems.equal_range(particleSystemName);
  Ogre::ParticleSystem* system{nullptr};
  int howMany{1};
  for(auto systemElement=range.first;systemElement!=range.second;++systemElement){
    //if((*systemElement).second->getNumParticles()==0){
    if(!(*systemElement).second->getEmitter(0)->getEnabled()){
      system=(*systemElement).second.get();
      break;
    }
    ++howMany;
  }
  std::cout<<"How many: "<<howMany<<std::endl;
  if(system==nullptr){
    Ogre::SceneManager* sceneManager=SceneManager::instance()->current_scene->manager;
    //extern Ogre::SceneManager* sceneManager;
    system=sceneManager->createParticleSystem(particleSystemName+std::to_string(howMany),particleSystemName);
    _systems.insert(std::make_pair(particleSystemName,std::unique_ptr<Ogre::ParticleSystem>{system}));
  }
  return system;
}

void ParticleManager::positionAndDirectSceneNode(Ogre::SceneNode* node,Ogre::Vector3 origin, Ogre::Vector3 direction){
  node->setPosition(origin);
  node->setDirection(direction);
}
