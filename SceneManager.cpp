#include "SceneManager.h"

SceneManager::~SceneManager()
{
	for(auto scene: _scenes)
	{
		delete scene;
	}
}

void SceneManager::initialize()
{
	_scene_id_assigner = 0;
}

int SceneManager::addScene(Scene* scene)
{
	scene->id = ++_scene_id_assigner;
	_scenes.push_back(scene);
}

void SceneManager::removeScene(Scene* scene)
{
	int i = 0;
	for(auto s : _scenes)
	{
		if(s->name == scene->name)
		{
			if(s == current_scene)
			{
				std::cout << "Cannot erase current scene" << std::endl;
				return;
			}
			else
			{
				_scenes.erase(_scenes.begin() + i);
				delete s;
			}
		}
		++i;
	}
}

Scene* SceneManager::getScene(std::string name)
{
	for(auto scene : _scenes)
	{
		if(scene->name == name)
			return scene;
	}
	return NULL;
}

void SceneManager::changeCurrentScene(Scene* scene)
{
	current_scene = scene;
}

