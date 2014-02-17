#include "Scene.h"

Scene::Scene(): _game_objects(), _cameras()
{
	_manager = GraphicsManager::instance()->getRenderRoot->CreateSceneManager(Ogre::ST_GENERIC);;
	_physics_world = PhysicsManager::instance()->CreatePhysicsWorld();
	_game_object_id_assigner = 0;
}

virtual int Scene::addGameObject(GameObject* gameObject)
{
	gameObject->setId(_game_object_id_assigner++);
	_game_objects.push_back(gameObject);
}

virtual GameObject* Scene::getGameObject(unsigned int index)
{
	return _game_objects[index];
}

virtual void Scene::addCamera(Camera* camera)
{

}

virtual void Scene::getCamera(int);

void Scene::update()
{
	for (auto gameObject : _game_objects)
  	{  
     	gameObject.update();
  	}
}

