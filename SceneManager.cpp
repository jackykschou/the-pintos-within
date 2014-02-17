#include "SceneManager.h"

int SceneManager::addScene(Scene* scene)
{
	_scenes.push_back(scene);
}
void SceneManager::removeScene(Scene* scene)
{
	Scene* scene = *(_scenes.begin() + index);
	if(scene == _current_scene)
		std::cout << "Cannot erase current scene" << std::endl;
	else
		_scenes.erase(_scenes.begin() + index);
}

Scene* SceneManager::GetScene(int index)
{
	return _scenes[index];
}

void SceneManager::updateCurrentScene()
{
	_current_scene->update();
}
