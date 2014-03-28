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
}

void startGame()
{
    GraphicsManager::instance()->startRendering();
}

int main(int argc, char *argv[])
{
    initializeGame();

    if (argc == 3 && strcmp(argv[1], "-c") == 0) {
        LOG("STARTING IN CLIENT MODE");
        NetworkManager::instance()->startClient(argv[2]);
    } else if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        LOG("STARTING IN SERVER MODE");
        NetworkManager::instance()->startServer();
    } else {
        // no arguments passed, start 1P game
        LOG("STARTING IN 1P MODE");
    }

    startGame();

    return 0;
}

