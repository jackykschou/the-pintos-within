#include <OgreSceneManager.h>
#include "ParticleManager.h"

ParticleManager::ParticleManager(){
}
ParticleManager::~ParticleManager(){
}
void ParticleManager::Emit(Ogre::Vector3 origin, Ogre::Vector3 direction, std::string particleSystemName){
  //Ogre::SceneManager* sceneManager=SceneManager::instance()->current_scene->manager;
  extern Ogre::SceneManager* sceneManager;

  auto range=_systems.equal_range(particleSystemName);
  Ogre::ParticleSystem* system{nullptr};
  int howMany{1};
  for(auto systemElement=range.first;systemElement!=range.second;++systemElement){
    if((*systemElement).second->getNumParticles()==0){
      system=(*systemElement).second.get();
      break;
    }
    ++howMany;
  }
  Ogre::SceneNode* node;
  if(system!=nullptr){
    node=system->getParentSceneNode();
    Ogre::SceneNode* parent=node->getParentSceneNode();
    //system->setEmitting(false);
    system->detachFromParent();
    parent->removeChild(node);
    node->attachObject(system);
    parent->addChild(node);
    //system->setEmitting(true);
  }else{
    system=sceneManager->createParticleSystem(particleSystemName+std::to_string(howMany),particleSystemName);
    _systems.insert(std::make_pair(particleSystemName,std::unique_ptr<Ogre::ParticleSystem>{system}));
    node=sceneManager->createSceneNode();
    node->attachObject(system);
    sceneManager->getRootSceneNode()->addChild(node);
  }
}
