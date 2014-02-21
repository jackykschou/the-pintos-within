// #include "TheEngine.h"

#include "TheEngine.h"

#define APPLICATION_NAME "THE EPIC GAME"

void initializeGame()
{
	Ogre::String resourcesCfg;
    Ogre::String pluginsCfg;

    resourcesCfg = "resources.cfg";
    pluginsCfg = "plugins.cfg";

	GraphicsManager::instance()->initialize(APPLICATION_NAME, pluginsCfg);
    ResourcesManager::instance()->initialize(resourcesCfg, pluginsCfg);
    InputManager::instance()->initialize();
    GUIManager::instance()->initialize(APPLICATION_NAME);
    PhysicsManager::instance()->initialize();
    SceneManager::instance()->initialize();
    AudioManager::instance()->initialize();
}

void createGameContents()
{
    Scene* first_scene = new Scene("New Scene");
    SceneManager::instance()->changeCurrentScene(first_scene);
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

/****************************************************************************
*                                                                           *
*                                                                           *
*                                                                           *
*****************************************************************************/
