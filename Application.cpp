#include "Application.h"

#define APPLICATION_NAME "Ball Blaster 2000"

void initializeGame()
{
	Ogre::String resourcesCfg;
    Ogre::String pluginsCfg;

    resourcesCfg = "resources.cfg";
    pluginsCfg = "plugins.cfg";

    ChatManager::instance()->addMessage("joe", "wassup");
    ChatManager::instance()->addMessage("joe", "asl");
    ChatManager::instance()->addMessage("joe", "i put on my robe and wizard hat");

    GameState::instance()->reset();
	GraphicsManager::instance()->initialize(APPLICATION_NAME, pluginsCfg);
    ResourcesManager::instance()->initialize(resourcesCfg, pluginsCfg);
    AudioManager::instance()->initialize();
    InputManager::instance()->initialize();
    PhysicsManager::instance()->initialize();
    SceneManager::instance()->initialize();
    GuiManager::instance()->Initialize(APPLICATION_NAME);
}

void startGame()
{
    GraphicsManager::instance()->startRendering();
}

int main(int argc, char *argv[])
{
    initializeGame();
    startGame();

    return 0;
}

