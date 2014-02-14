#include "GraphicsManager.h"

bool GraphicsManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(_window->isClosed())
        return false;

    if(_shutdown)
        return false;

    //!Need to update game objects in the current scene

    //!Need to update physics manager

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

bool GraphicsManager::initialize(std::string window_name)
{
	initializeRoot();
	configureWindow(window_name);
}


Ogre::Root* GraphicsManager::getRenderRoot()
{
    return _root
}

Ogre::RenderWindow* GraphicsManager::getRenderWindow()
{
    return _window;
}

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



void GraphicsManager::initializeRoot()
{
	_root = new Ogre::Root(ResourcesManager.instance()->getPluginsCfg());
}

bool GraphicsManager::configureWindow(std::string window_name)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(_root->showConfigDialog())
    {
        // If returned true, user clicked OK so initialise
        // Here we choose to let the system create a default rendering window by passing 'true'
        _window = _root->initialise(true, window_name);

        _shutdown = false;
    }
    else
    {
        _shutdown = true;
    }
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
