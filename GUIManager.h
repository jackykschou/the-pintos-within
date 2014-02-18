#ifndef __GUIManager_h_
#define __GUIManager_h_

#include "Singleton.h"
#include <sstream>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

class GUIManager : public Singleton<GUIManager> {
  public:
	void initialize();
};

#endif
