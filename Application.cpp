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
    AudioManager::instance()->initialize();
    InputManager::instance()->initialize();
    GUIManager::instance()->initialize(APPLICATION_NAME);
    PhysicsManager::instance()->initialize();
    SceneManager::instance()->initialize();
}

void createGameContents()
{
    Scene* first_scene = new Scene("New Scene");
    SceneManager::instance()->changeCurrentScene(first_scene);


    GameObject *go = new GameObject("Camra", first_scene);
    FPSCamera *c = new FPSCamera(go, "camera1", 0);
    Transform *tran1 = go->getComponent<Transform>();
    tran1->posX = 30;
    tran1->posY = 30;
    tran1->posZ = 30;


    GameObject *ogre = new GameObject("Head", first_scene);
    Mesh *head = new Mesh(ogre, "ogrehead.mesh");
    Transform *tran2 = ogre->getComponent<Transform>();
    tran2->posX = 1;
    tran2->posY = 1;
    tran2->posZ = 1;

    SceneManager::instance()->current_scene->main_camera = (Camera*)c;
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

