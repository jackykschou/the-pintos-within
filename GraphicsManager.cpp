#include "GraphicsManager.h"

GraphicsManager::~GraphicsManager()
{
    windowClosed();
    delete _root;
}

void GraphicsManager::windowClosed()
{
    //!detech input manager

    // //Only close for window that created OIS (the main window in these demos)
    // if( rw == _window )
    // {
    //     if( mInputManager )
    //     {
    //         mInputManager->destroyInputObject( mMouse );
    //         mInputManager->destroyInputObject( mKeyboard );

    //         OIS::InputManager::destroyInputSystem(mInputManager);
    //         mInputManager = 0;
    //     }
    // }
}

bool GraphicsManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(_window->isClosed())
        return false;

    if(_shutdown)
        return false;

    last_render_time = clock();

    _frame_event = new Ogre::FrameEvent(evt);

    SceneManager::instance()->current_scene->update();

    SceneManager::instance()->current_scene->physics_world->stepSimulation(((double)(clock() - last_render_time)) / CLOCKS_PER_SEC);


    //Need to capture/update each device
    //!mKeyboard->capture();
    //!mMouse->capture();

    //!mTrayMgr->frameRenderingQueued(evt);

    //!!
    /*if (!mTrayMgr->isDialogVisible())
    {
        if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(cameras[cameras_index]->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(cameras[cameras_index]->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(cameras[cameras_index]->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(cameras[cameras_index]->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(cameras[cameras_index]->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(cameras[cameras_index]->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(cameras[cameras_index]->getDerivedOrientation().z));
        }
    }*/

    return true;
}

void GraphicsManager::initialize(std::string window_name)
{
	initializeRoot();
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

void GraphicsManager::initializeRoot()
{
	_root = new Ogre::Root(ResourcesManager::instance()->getPluginsCfg());
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

	//! need to to clean up of other managers
}
