#include "SceneManager.h"

SceneManager::~SceneManager()
{
	delete current_scene;
}

void SceneManager::initialize()
{
	_scene_id_assigner = 0;
	current_scene = NULL;
}

void SceneManager::changeCurrentScene(uint32_t scene_type)
{
	GameState::instance()->current_state = LOADING;

	if(current_scene != NULL)
		delete current_scene;

	current_scene = NULL;

	std::ostringstream scene_id_stream;
  	scene_id_stream << (_scene_id_assigner++);

	current_scene = new Scene(std::string("New Scene") + scene_id_stream.str());

	if(scene_type == THEGAUNTLET)
		scene_loader.parseDotScene(scene_type, current_scene, "TheGauntlet.scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, current_scene->manager);
	else if(scene_type == DUSTTWO)
		scene_loader.parseDotScene(scene_type, current_scene, "de_dust.scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, current_scene->manager);


	GameState::instance()->current_state = IN_GAME;
}

void SceneManager::updateScene(float time)
{
	if(current_scene != NULL && !GameState::instance()->_gameOver)
		current_scene->update(time);
}

