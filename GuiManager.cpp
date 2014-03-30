#include "GuiManager.h"

GuiManager::GuiManager():_isDisplayed{false}{
}
GuiManager::~GuiManager(){
  delete _hud;
  delete _mainMenu;
  delete _waitingPrompt;
  delete _hostDialog;
  CEGUI::OgreRenderer::destroySystem();
}
void GuiManager::Update(const Ogre::FrameEvent& event){
  CEGUI::System::getSingleton().injectTimePulse(event.timeSinceLastFrame);
  if(_isDisplayed){
    if(_current==_hud||GameState::instance()->isRunning()){
      if(_current!=_hud){
        _current=_hud;
        _current->Display();
      }
      PlayerCharacter* player=GameState::instance()->player;
      if(player!=nullptr){
        Hud* hud=static_cast<Hud*>(_hud);
        hud->UpdateHealth(0.01f*player->health);
      }
    }//else if(_current==WaitingPrompt&&){
      //EnableStart();
    //}
  }else{
    _isDisplayed=true;
    _current->Display();
  }
}
void GuiManager::Initialize(std::string applicationName){
  _renderer=&CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::SchemeManager::getSingleton().create( "TaharezLook.scheme" );
  CEGUI::System::getSingletonPtr()->setDefaultMouseCursor("TaharezLook","MouseArrow");

  _hud=new Hud;
  _mainMenu=new MainMenu;
  _waitingPrompt=new WaitingPrompt;
  _hostDialog=new HostDialog;
  _current=_mainMenu;
}
bool GuiManager::IsExpectingMouse(){
  return _current!=_hud;
}
bool GuiManager::IsExpectingKeyboard(){
  return _current==_hostDialog;
}
bool GuiManager::HostGame(const CEGUI::EventArgs& e){
  _current=_waitingPrompt;
  _current->Display();
  LOG("STARTING IN SERVER MODE");
  NetworkManager::instance()->startServer();
  return false;
}
bool GuiManager::JoinGame(const CEGUI::EventArgs& e){
  static_cast<WaitingPrompt*>(_waitingPrompt)->RemoveStart();
  _current=_hostDialog;
  _current->Display();
  return false;
}
bool GuiManager::Exit(const CEGUI::EventArgs& e){
  GraphicsManager::instance()->stopRendering();
  return false;
}
bool GuiManager::Start(const CEGUI::EventArgs& e){
  _current=_hud;
  _current->Display();
  LOG("STARTING GAME.");
  GameState::instance()->start();
  return false;
}
bool GuiManager::Connect(const CEGUI::EventArgs& e){
  LOG("STARTING IN CLIENT MODE");
  NetworkManager::instance()->startClient(static_cast<HostDialog*>(_hostDialog)->ReadHost());
  _current=_waitingPrompt;
  _current->Display();
  return false;
}
void GuiManager::EnableStart(){
  static_cast<WaitingPrompt*>(_waitingPrompt)->EnableStart();
}
CEGUI::MouseButton GuiManager::TranslateButton(OIS::MouseButtonID buttonId){
  switch(buttonId){
    case OIS::MB_Left:
	  return CEGUI::LeftButton;
    case OIS::MB_Right:
      return CEGUI::RightButton;
    case OIS::MB_Middle:
      return CEGUI::MiddleButton;
    default:
      return CEGUI::LeftButton;
  }
}

Hud::Hud():Gui("Hud.layout"){/*
  _healthBar = static_cast<CEGUI::ProgressBar*>(
      CEGUI::WindowManager::getSingletonPtr()->createWindow("TaharezLook/ProgressBar","HealthBar"));
  _root->addChildWindow(_healthBar);
  _healthBar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 10.0f), CEGUI::UDim( 0.92f, 0.0f)));
  _healthBar->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.02f, 0.0f)));

  _fuelBar = static_cast<CEGUI::ProgressBar*>(
      CEGUI::WindowManager::getSingletonPtr()->createWindow("TaharezLook/ProgressBar","FuelBar"));
  _root->addChildWindow(_fuelBar);
  _fuelBar->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 10.0f), CEGUI::UDim( 0.96f, 0.0f)));
  _fuelBar->setSize(CEGUI::UVector2(CEGUI::UDim(0.15f, 0.0f), CEGUI::UDim(0.02f, 0.0f)));
*/
}
void Hud::UpdateHealth(float percentHealth){
  _healthBar->setProgress(0.75f);
}
void Hud::UpdateFuel(float percentFuel){
  _fuelBar->setProgress(0.2f);
}
MainMenu::MainMenu():Gui("MainMenu.layout"){
  _hostGame=static_cast<CEGUI::PushButton*>(_root->getChild("MainMenu/HostGame"));
  _joinGame=static_cast<CEGUI::PushButton*>(_root->getChild("MainMenu/JoinGame"));
  _exit=static_cast<CEGUI::PushButton*>(_root->getChild("MainMenu/Exit"));

  _hostGame->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::HostGame,GuiManager::instance()));
  _joinGame->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::JoinGame,GuiManager::instance()));
  _exit->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::Exit,GuiManager::instance()));
}
WaitingPrompt::WaitingPrompt():Gui("WaitingPrompt.layout"){
  _start=static_cast<CEGUI::PushButton*>(_root->getChild("WaitingPrompt/Start"));
  _start->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::Start,GuiManager::instance()));
  _start->disable();
}
void WaitingPrompt::EnableStart(){
  _start->enable();
}
void WaitingPrompt::RemoveStart(){
  _root->removeChildWindow(_start);
}
HostDialog::HostDialog():Gui("HostDialog.layout"){
  _host=static_cast<CEGUI::Editbox*>(_root->getChild("HostDialog/HostName"));
  _connect=static_cast<CEGUI::PushButton*>(_root->getChild("HostDialog/Connect"));
  _connect->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::Connect,GuiManager::instance()));
}
const char* HostDialog::ReadHost(){
  return _host->getText().c_str();
}
Gui::Gui(std::string layoutFileName):_root(CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout(layoutFileName)){}
void Gui::Display(){
  CEGUI::System::getSingleton().setGUISheet(_root);
}
