#include "common.h"

#define APPLICATION_NAME "THE EPIC GAME"

void initializeGame()
{
    ResourcesManager::instance()->initialize();
    GraphicsManager::instance()->initialize(APPLICATION_NAME);
    InputManager::instance()->initialize();
    PhysicsManager::initialize()->initialize();
    SceneManager::initialize()->initialize();
    AudioManager::initialize()->initialize();
}
void createGameContents();
void startGame();


/****************************************************************************
*                                                                           *
*                         Where the game starts                             *
*                                                                           *
*****************************************************************************/

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        try 
        {
            initializeGame();
            createGameContents();
            startGame();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

/****************************************************************************
*                                                                           *
*                                                                           *
*                                                                           *
*****************************************************************************/
