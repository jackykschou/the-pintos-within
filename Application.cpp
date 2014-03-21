#include "TheEngine.h"
#include "Application.h"

#define APPLICATION_NAME "Ball Blaster 2000"

void initializeGame()
{
	Ogre::String resourcesCfg;
    Ogre::String pluginsCfg;

    resourcesCfg = "resources.cfg";
    pluginsCfg = "plugins.cfg";

    GameState::instance()->reset();
	GraphicsManager::instance()->initialize(APPLICATION_NAME, pluginsCfg);
    ResourcesManager::instance()->initialize(resourcesCfg, pluginsCfg);
    AudioManager::instance()->initialize();
    InputManager::instance()->initialize();
    GUIManager::instance()->initialize(APPLICATION_NAME);
    PhysicsManager::instance()->initialize();
    SceneManager::instance()->initialize();
    AudioManager::instance()->playBALLZ();
}

void createGameContents()
{
    Scene* first_scene = new Scene("New Scene");
    SceneManager::instance()->changeCurrentScene(first_scene);

    DotSceneLoader loader;
    loader.parseDotScene(first_scene, "TheGauntlet.scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, first_scene->manager);
}

void startGame()
{
    GraphicsManager::instance()->startRendering();
}


/****************************************************************************
*                                                                           *
*                         Where the game starts                             *
*                                                                           *
*****************************************************************************/

int main(int argc, char *argv[])
{
    initializeGame();
    createGameContents();
    startGame();
    return 0;
}

