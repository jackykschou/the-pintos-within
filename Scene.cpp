#include "Scene.h"

Scene::Scene(): _game_objects(),
{
	manager = GraphicsManager::instance()->getRenderRoot->CreateSceneManager(Ogre::ST_GENERIC);;
	physics_world = PhysicsManager::instance()->CreatePhysicsWorld();
	_game_object_id_assigner = 0;
}

Scene::~Scene()
{
	for(auto gameObject : _game_objects)
	{
		delete gameObject;
	}

	delete manager;
	delete physics_world;

	SceneManager::instance()->removeScene(this);
}

int Scene::addGameObject(GameObject* gameObject)
{
	gameObject->id = (++_game_object_id_assigner);
	_game_objects.push_back(gameObject);
}

void Scene::removeGameObject(GameObject* gameObject)
{
	int i = 0;
	for(auto g in _game_objects)
	{
		if(g->id == gameObject->id)
		{
			_game_objects.erase(_game_objects.begin() + i);
			delete g;
		}
		++i;
	}
}

GameObject* Scene::getGameObject(int id)
{
	for(auto gameObject : _game_objects)
	{
		if(gameObject->id == id)
			return gameObject;
	}
}

void Scene::update()
{
	for (auto gameObject : _game_objects)
  { 
  		if(gameObject->active)
     		gameObject->update();
  }
}
