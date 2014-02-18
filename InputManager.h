#ifndef __InputManager_h_
#define __InputManager_h_

#include "Singleton.h"
#include <sstream>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreRenderWindow.h>


class InputManager : public Singleton<InputManager>, public OIS::KeyListener, public OIS::MouseListener {
  public:
  	InputManager();

  	void initialize();
  	void capture();

    // OIS Input devices
    OIS::InputManager* _mInputManager;
    OIS::Mouse*        _mMouse;
    OIS::Keyboard*     _mKeyboard;

    // OIS::KeyListener callbacks
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    // OIS::MouseListener callbacks
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // Ogre::WindowEventListener
    // Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    // Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);
};

#endif
