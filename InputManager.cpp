#include "InputManager.h"

InputManager::InputManager() :
	_mInputManager(0),
	_mMouse(0),
	_mKeyboard(0)
{}

// Sets up input devices and event callbacks
void InputManager::initialize() {
	OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    //!mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	// Set up the input thread
	_mInputManager = OIS::InputManager::createInputSystem(pl);
	// Set up the input devices
	_mKeyboard = static_cast<OIS::Keyboard*>(_mInputManager->createInputObject(OIS::OISKeyboard, true));
	_mMouse = static_cast<OIS::Mouse*>(_mInputManager->createInputObject(OIS::OISMouse, true));
	// Register for events
	_mMouse->setEventCallback(this);
	_mKeyboard->setEventCallback(this);

	// Set initial mouse clipping size
    //!windowResized(GraphicsManager::instance().getWindow());

	// Register as a Window listener
		//Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}

// Called every frame to snatch the input buffer from the device thread
void InputManager::capture() {
	_mKeyboard->capture();
	_mMouse->capture();
}

bool InputManager::keyPressed(const OIS::KeyEvent &arg) {

}

bool InputManager::keyReleased(const OIS::KeyEvent &arg) {

}

bool InputManager::mouseMoved(const OIS::MouseEvent &arg) {

}

bool InputManager::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {

}

bool InputManager::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {

}

void InputManager::windowResized(Ogre::RenderWindow* rw) {

}

void InputManager::windowClosed(Ogre::RenderWindow* rw) {

}


// TODO

//-------------------------------------------------------------------------------------
// bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
// {
//     if (mTrayMgr->isDialogVisible()) return true;   // don't process any more keys if dialog is up

//     if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
//     {
//         mTrayMgr->toggleAdvancedFrameStats();
//     }
//     else if (arg.key == OIS::KC_G)   // toggle visibility of even rarer debugging details
//     {
//         if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
//         {
//             mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
//             mDetailsPanel->show();
//         }
//         else
//         {
//             mTrayMgr->removeWidgetFromTray(mDetailsPanel);
//             mDetailsPanel->hide();
//         }
//     }
//     else if (arg.key == OIS::KC_T)   // cycle polygon rendering mode
//     {
//         Ogre::String newVal;
//         Ogre::TextureFilterOptions tfo;
//         unsigned int aniso;

//         switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
//         {
//         case 'B':
//             newVal = "Trilinear";
//             tfo = Ogre::TFO_TRILINEAR;
//             aniso = 1;
//             break;
//         case 'T':
//             newVal = "Anisotropic";
//             tfo = Ogre::TFO_ANISOTROPIC;
//             aniso = 8;
//             break;
//         case 'A':
//             newVal = "None";
//             tfo = Ogre::TFO_NONE;
//             aniso = 1;
//             break;
//         default:
//             newVal = "Bilinear";
//             tfo = Ogre::TFO_BILINEAR;
//             aniso = 1;
//         }

//         Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
//         Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
//         mDetailsPanel->setParamValue(9, newVal);
//     }
//     else if (arg.key == OIS::KC_R)   // cycle polygon rendering mode
//     {
//         Ogre::String newVal;
//         Ogre::PolygonMode pm;

//         switch (mCamera->getPolygonMode())
//         {
//         case Ogre::PM_SOLID:
//             newVal = "Wireframe";
//             pm = Ogre::PM_WIREFRAME;
//             break;
//         case Ogre::PM_WIREFRAME:
//             newVal = "Points";
//             pm = Ogre::PM_POINTS;
//             break;
//         default:
//             newVal = "Solid";
//             pm = Ogre::PM_SOLID;
//         }

//         mCamera->setPolygonMode(pm);
//         mDetailsPanel->setParamValue(10, newVal);
//     }
//     else if(arg.key == OIS::KC_F5)   // refresh all textures
//     {
//         Ogre::TextureManager::getSingleton().reloadAll();
//     }
//     else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
//     {
//         mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
//     }
//     else if (arg.key == OIS::KC_ESCAPE)
//     {
//         mShutDown = true;
//     }

//     mCameraMan->injectKeyDown(arg);
//     return true;
// }

// bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
// {
//     mCameraMan->injectKeyUp(arg);
//     return true;
// }

// bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
// {
//     if (mTrayMgr->injectMouseMove(arg)) return true;
//     mCameraMan->injectMouseMove(arg);
//     return true;
// }

// bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
// {
//     if (mTrayMgr->injectMouseDown(arg, id)) return true;
//     mCameraMan->injectMouseDown(arg, id);
//     return true;
// }

// bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
// {
//     if (mTrayMgr->injectMouseUp(arg, id)) return true;
//     mCameraMan->injectMouseUp(arg, id);
//     return true;
// }

// //Adjust mouse clipping area
// void BaseApplication::windowResized(Ogre::RenderWindow* rw)
// {
//     unsigned int width, height, depth;
//     int left, top;
//     rw->getMetrics(width, height, depth, left, top);

//     const OIS::MouseState &ms = mMouse->getMouseState();
//     ms.width = width;
//     ms.height = height;
// }

// //Unattach OIS before window shutdown (very important under Linux)
// void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
// {
//     //Only close for window that created OIS (the main window in these demos)
//     if( rw == mWindow )
//     {
//         if( mInputManager )
//         {
//             mInputManager->destroyInputObject( mMouse );
//             mInputManager->destroyInputObject( mKeyboard );

//             OIS::InputManager::destroyInputSystem(mInputManager);
//             mInputManager = 0;
//         }
//     }
// }