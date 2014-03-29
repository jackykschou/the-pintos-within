#ifndef __GuiManager_h_
#define __GuiManager_h_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <OISMouse.h>
#include <OgreFrameListener.h>
#include "Singleton.h"

class GuiManager:public Singleton<GuiManager>{
  public:
    GuiManager();
    virtual ~GuiManager();
    void Update(const Ogre::FrameEvent& event);
    void Initialize(std::string applicationName);
  private:
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonId);
    CEGUI::OgreRenderer* _renderer;
};

#endif
