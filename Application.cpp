#include "common.h"
#include "ResourcesManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "GUIManager.h"
#include "PhysicsManager.h"
#include "AudioManager.h"

#define APPLICATION_NAME "THE EPIC GAME"

void initializeGame()
{
    ResourcesManager::instance()->initialize();
    GraphicsManager::instance()->initialize(APPLICATION_NAME);
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
