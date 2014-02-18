#ifndef __InputManager_h_
#define __InputManager_h_

#include "Singleton.h"
#include <sstream>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>


class InputManager : public Singleton<InputManager>,
                     public Ogre::WindowEventListener,
                     public OIS::KeyListener,
                     public OIS::MouseListener {
  private:

    // OIS Input devices
    OIS::InputManager* _mInputManager;
    OIS::Mouse*        _mMouse;
    OIS::Keyboard*     _mKeyboard;

    // "Buffered" input events are stored by the InputManager
    OIS::KeyEvent*     _lastKeyPressedEvt;
    OIS::KeyEvent*     _lastKeyReleasedEvt;
    OIS::MouseEvent*   _lastMouseMovedEvt;
    OIS::MouseEvent*   _lastMousePressedEvt;
    OIS::MouseEvent*   _lastMouseReleasedEvt;

  public:
  	InputManager();

  	void initialize();
  	void capture();

    // Synchronous methods for checking state input
    bool isKeyDown(OIS::KeyCode kc);
    bool isKeyUp(OIS::KeyCode kc);
    bool isMouseDown(OIS::MouseButtonID button);
    bool isMouseUp(OIS::MouseButtonID button);

    OIS::KeyEvent*   getKeyPressedEvent();
    OIS::KeyEvent*   getKeyReleasedEvent();
    OIS::MouseEvent* getMouseMovedEvent();
    OIS::MouseEvent* getMousePressedEvent();
    OIS::MouseEvent* getMouseReleasedEvent();

    // OIS::KeyListener callbacks
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    // OIS::MouseListener callbacks
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

    // Ogre::WindowEventListener callbacks
    // Adjust mouse clipping area
    virtual void windowResized(Ogre::RenderWindow* rw);
    // Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(Ogre::RenderWindow* rw);
};

#endif
