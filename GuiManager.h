#ifndef __GuiManager_h_
#define __GuiManager_h_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <OISMouse.h>
#include <OgreRoot.h>
#include <OgreFrameListener.h>
#include "Singleton.h"
#include "GraphicsManager.h"
#include "GameState.h"
#include "PlayerCharacter.h"

class Gui;
class GuiManager:public Singleton<GuiManager>{
  public:
    GuiManager();
    virtual ~GuiManager();
    void EnableStart();
    void Update(const Ogre::FrameEvent& event);
    void Initialize(std::string applicationName);
    bool IsExpectingMouse();
    bool IsExpectingKeyboard();
    bool HostGame(const CEGUI::EventArgs& e);
    bool JoinGame(const CEGUI::EventArgs& e);
    bool Exit(const CEGUI::EventArgs& e);
    bool Start(const CEGUI::EventArgs& e);
    bool Connect(const CEGUI::EventArgs& e);
    static CEGUI::MouseButton TranslateButton(OIS::MouseButtonID buttonId);
    void Reinitialize();
  private:
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonId);
    CEGUI::OgreRenderer* _renderer;
    bool _isDisplayed;
    Gui* _hud;
    Gui* _mainMenu;
    Gui* _waitingPrompt;
    Gui* _hostDialog;
    Gui* _current;
};
class Gui{
  public:
    Gui();
    void Display();
  protected:
    Gui(std::string layoutFileName);
    CEGUI::Window* _root;
};
class MainMenu:public Gui{
  public:
	MainMenu();
  private:
	CEGUI::PushButton* _hostGame;
	CEGUI::PushButton* _joinGame;
	CEGUI::PushButton* _exit;
};
class Hud:public Gui{
  public:
	Hud();
    void UpdateHealth(float percentHealth);
    void UpdateFuel(float percentFuel);
    void UpdateAmmoCount(int ammoCount);
    void UpdateMagCount(int magCount);
  private:
    CEGUI::ProgressBar* _healthBar;
    CEGUI::ProgressBar* _fuelBar;
    CEGUI::Window* _ammoCount;
    CEGUI::Window* _magCount;
};
class WaitingPrompt:public Gui{
  public:
    WaitingPrompt();
    void EnableStart();
    void RemoveStart();
  private:
    CEGUI::PushButton* _start;
};

class HostDialog: public Gui {
  public:
    HostDialog();
    const char* ReadHost();
    const char* ReadName();
    bool NameCaretMoved(const CEGUI::EventArgs& e);
    bool HostCaretMoved(const CEGUI::EventArgs& e);

  private:
    CEGUI::Editbox*    _host;
    CEGUI::Editbox*    _name;
    CEGUI::PushButton* _connect;
    CEGUI::PushButton* _back;
};

#endif
