// #include "TheEngine.h"

#include "TheEngine.h"
#include "Application.h"

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


    // GameObject *go = new GameObject("Camra", first_scene);
    // FPSCamera *c = new FPSCamera(go, "camera1", 0);
    // Transform *tran1 = go->getComponent<Transform>();
    // tran1->posX = 30;
    // tran1->posY = 30;
    // tran1->posZ = 30;


    GameObject *go = new GameObject("Controller", first_scene);
    Transform *tran1 = new Transform(go);
    tran1->posX = 0;
    tran1->posY = 20;
    tran1->posZ = 0;
    FPSBoxController *c = new FPSBoxController(go, "Cam", 0.5, btVector3(1, 1, 1), 1, COL_CHARACTER, CHARACTER_COLLIDER_WITH);

    GameObject *ogre = new GameObject("Head", first_scene);
    Mesh *head = new Mesh(ogre, "ogrehead.mesh");
    Transform *tran2 = ogre->getComponent<Transform>();
    tran2->posX = 1;
    tran2->posY = 1;
    tran2->posZ = 1;

    SceneManager::instance()->current_scene->main_camera = (Camera*)c->fps_camera;
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

