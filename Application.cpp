#include "common.h"
#include "ResourcesManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GUIManager.h"
#include "PhysicsManager.h"
#include "AudioManager.h"

#define APPLICATION_NAME "THE EPIC GAME"

#define RESOURCES_PATH "resources.cfg"
#define RESOURCES_DEBUG_PATH "resources_d.cfg"

#define PLUGIN_PATH "plugins.cfg"
#define PLUGIN_DEBUG_PATH "plugins_d.cfg"


void initializeGame()
{
	Ogre::String resourcesCfg;
    Ogre::String pluginsCfg;

    #ifdef _DEBUG
	    resourcesCfg = "resources_d.cfg";
	    pluginsCfg = "plugins_d.cfg";
	#else
	    resourcesCfg = "resources.cfg";
	    pluginsCfg = "plugins.cfg";
	#endif

	GraphicsManager::instance()->initialize(APPLICATION_NAME, pluginsCfg);
    ResourcesManager::instance()->initialize(resourcesCfg, pluginsCfg);
    InputManager::instance()->initialize();
    GUIManager::instance()->initialize(APPLICATION_NAME);
    PhysicsManager::instance()->initialize();
    SceneManager::instance()->initialize();
    AudioManager::instance()->initialize();
}
void createGameContents();
void startGame();


/****************************************************************************
*                                                                           *
*                         Where the game starts                             *
*                                                                           *
*****************************************************************************/

int main(int argc, char *argv[])
{
    initializeGame();
    return 0;
}

/****************************************************************************
*                                                                           *
*                                                                           *
*                                                                           *
*****************************************************************************/
