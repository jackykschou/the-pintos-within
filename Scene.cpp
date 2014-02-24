#include "Scene.h"

Scene::Scene(std::string n)
{
	name = n;
	manager = GraphicsManager::instance()->getRenderRoot()->createSceneManager(Ogre::ST_GENERIC);
	physics_world = PhysicsManager::instance()->createDynamicsWorld();
	_game_object_id_assigner = 0;
	manager->setAmbientLight(Ogre::ColourValue(0, 0, 0));

	SceneManager::instance()->addScene(this);
}

Scene::~Scene()
{
	for(auto gameObject : game_objects)
	{
		delete gameObject;
	}

	delete manager;
	delete physics_world;
}

void Scene::addGameObject(GameObject* gameObject)
{
	gameObject->id = (++_game_object_id_assigner);
	game_objects.push_back(gameObject);
}

void Scene::removeGameObject(GameObject* gameObject)
{
	int i = 0;
	for(auto g : game_objects)
	{
		if(g->id == gameObject->id)
		{
			game_objects.erase(game_objects.begin() + i);
			delete g;
		}
		++i;
	}
}

std::vector<GameObject*> Scene::getGameObjectsOfTag(std::string tag)
{
	std::vector<GameObject*> temp_game_objects;
	for(auto gameObject : game_objects)
	{
		if(gameObject->tag == tag)
			temp_game_objects.push_back(gameObject);
	}
	return temp_game_objects;
}

void Scene::update(float elapsed_time)
{
	physics_world->stepSimulation(elapsed_time);

	for (auto gameObject : game_objects)
  	{ 
  		if(gameObject->active)
     		gameObject->update();
  	}
}
