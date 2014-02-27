#include "TheEngine.h"
#include "Application.h"

#define APPLICATION_NAME "Ball Blaster 2000"

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


    // GameObject *go = new GameObject("Camra", first_scene);
    // FPSCamera *c = new FPSCamera(go, "camera1", 0);
    // Transform *tran1 = go->getComponent<Transform>();
    // tran1->posX = 30;
    // tran1->posY = 30;
    // tran1->posZ = 30;

    DotSceneLoader loader;
    loader.parseDotScene(first_scene, "TheCourt.scene", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, first_scene->manager);

    Ogre::Light* pointLight = SceneManager::instance()->current_scene->manager->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(0, 10, 0));
    pointLight->setDiffuseColour(1, 1, 1);
    pointLight->setSpecularColour(1, 1, 1);

    Ogre::Light* pointLight1 = SceneManager::instance()->current_scene->manager->createLight("pointLight1");
    pointLight1->setType(Ogre::Light::LT_POINT);
    pointLight1->setPosition(Ogre::Vector3(10, 0, 0));
    pointLight1->setDiffuseColour(1, 1, 1);
    pointLight1->setSpecularColour(1, 1, 1);


    Ogre::Light* pointLight2 = SceneManager::instance()->current_scene->manager->createLight("pointLight2");
    pointLight2->setType(Ogre::Light::LT_POINT);
    pointLight2->setPosition(Ogre::Vector3(0, 0, 10));
    pointLight2->setDiffuseColour(1, 1, 1);
    pointLight2->setSpecularColour(1, 1, 1);


    // GameObject *go = new GameObject("Controller", first_scene);
    // Transform *tran1 = go->getComponent<Transform>();
    // tran1->posX = 0;
    // tran1->posY = 10;
    // tran1->posZ = 0;
    // FPSBoxController *c = new FPSBoxController(go, "Cam", 0.5, btVector3(10, 10, 10), 0.5, COL_CHARACTER, CHARACTER_COLLIDER_WITH);

    // SceneManager::instance()->current_scene->main_camera = (Camera*)c->fps_camera;
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

