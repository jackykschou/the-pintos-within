#include "GraphicsManager.h"

#define FRAME_RATE 60

GraphicsManager::~GraphicsManager()
{
    delete _root;
}

bool GraphicsManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    Ogre::Real ttW = (1000.0 / FRAME_RATE) - (1000.0 * evt.timeSinceLastFrame); 
    if (ttW > 0) 
        usleep(ttW * 1000.0); 

    if(_window->isClosed())
        return false;

    if(_shutdown)
        return false;
    
    if(_frame_event != NULL)
    {
        delete _frame_event;
        _frame_event = NULL;
    }

    _frame_event = new Ogre::FrameEvent(evt);

    InputManager::instance()->capture();
/* Examples/demos for emitting particles.
    if(InputManager::instance()->isKeyPressed(OIS::KC_B)){
      Ogre::ParticleSystem* system1=ParticleManager::instance()->Emit("BloodSpurt",Ogre::Vector3{25,25,25},Ogre::Vector3{1,1,1});

      //ParticleManager::instance()->Emit("JetPackYellowSparks",Ogre::Vector3{-10,25,-10},Ogre::Vector3{0,-1,0});
      //ParticleManager::instance()->Emit("JetPackOrangeSparks",Ogre::Vector3{-10,25,-10},Ogre::Vector3{0,-1,0});
      //ParticleManager::instance()->Emit("JetPackSmoke",Ogre::Vector3{-10,25,-10},Ogre::Vector3{0,1,0});

      ParticleManager::instance()->EmitSparks(Ogre::Vector3{-10,25,-10},Ogre::Vector3{0,-1,0});

      //ParticleManager::instance()->Emit("OrangeExplosion",Ogre::Vector3{10,25,-10},Ogre::Vector3{0,1,0});
      //ParticleManager::instance()->Emit("YellowExplosion",Ogre::Vector3{10,25,-10},Ogre::Vector3{0,1,0});
      //ParticleManager::instance()->Emit("OrangeExplosionCloud",Ogre::Vector3{10,25,-10},Ogre::Vector3{0,1,0});

      ParticleManager::instance()->EmitExplosion(Ogre::Vector3{10,25,-10});
    }
*/
    GUIManager::instance()->update(evt);
    
    SceneManager::instance()->updateScene(evt.timeSinceLastFrame);

    GameState::instance()->update();

    NetworkManager::instance()->update();

    return true;
}

void GraphicsManager::initialize(std::string window_name, Ogre::String pluginsCfg)
{
    // srand (0);
    srand (time(NULL));
	initializeRoot(pluginsCfg);
	configureWindow(window_name);
}

Ogre::Root* GraphicsManager::getRenderRoot()
{
    return _root;
}

Ogre::RenderWindow* GraphicsManager::getRenderWindow()
{
    return _window;
}

void GraphicsManager::initializeRoot(Ogre::String pluginsCfg)
{
	_root = new Ogre::Root(pluginsCfg);
    _root->addFrameListener(this);
}

void GraphicsManager::configureWindow(std::string window_name)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(_root->restoreConfig() || _root->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        _window = _root->initialise(true, window_name.c_str());

        _shutdown = false;
    }
    else
    {
        _shutdown = true;
    }
}

Ogre::FrameEvent* GraphicsManager::getFrameEvent()
{
    return _frame_event;
}

void GraphicsManager::startRendering()
{
    _root->startRendering();
}

void GraphicsManager::stopRendering()
{
	_shutdown = true;

    InputManager::instance()->windowClosed(_window);
	//! need to to clean up of other managers
}
