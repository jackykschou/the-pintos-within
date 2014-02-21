#include "GraphicsManager.h"

GraphicsManager::~GraphicsManager()
{
    delete _root;
}

bool GraphicsManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
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

    GUIManager::instance()->update(evt);

    SceneManager::instance()->current_scene->update(evt.timeSinceLastFrame);

    if(InputManager::instance()->isKeyPressed(OIS::KC_ESCAPE))
        stopRendering();

    return true;
}

void GraphicsManager::initialize(std::string window_name, Ogre::String pluginsCfg)
{
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
    if(_root->showConfigDialog())
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
