#include "GuiManager.h"
#include "ChatManager.h"

GuiManager::GuiManager():_isDisplayed{false}{}
GuiManager::~GuiManager(){
  delete _hud;
  delete _mainMenu;
  delete _joinGameMenu;
  delete _createGameMenu;
  delete _lobby;
  delete _waitingPrompt;
  CEGUI::OgreRenderer::destroySystem();
}
void GuiManager::Update(const Ogre::FrameEvent& event){
  Hud* hud=static_cast<Hud*>(_hud);
  if(_isDisplayed){
    if(_current==_hud||GameState::instance()->isRunning()){
      if(_current!=_hud){
        _current=_hud;
        _current->Display();
        CEGUI::MouseCursor::getSingletonPtr()->hide();
      }
      PlayerCharacter* player=GameState::instance()->player;
      if(player!=nullptr){
        hud->UpdateHealth(0.01f*player->health>=0?0.01f*player->health:0.0f);
        FPSBoxController* controller=player->controller;
        if(controller!=nullptr){
          hud->UpdateFuel(controller->jet_pack_current/controller->jet_pack_max);
        }
        Weapon* weapon=player->current_weapon;
        if(weapon!=nullptr){
          hud->UpdateAmmoCount(weapon->current_mag_count);
          hud->UpdateMagCount(weapon->current_ammo);
        }
      }
    }//else{Ogre::Root::getSingletonPtr()->getRenderSystem()->clearFrameBuffer(0);}
    hud->UpdateConsole();
  }else{
    _isDisplayed=true;
    _current->Display();
  }
  CEGUI::System::getSingleton().injectTimePulse(event.timeSinceLastFrame);
}
void GuiManager::Initialize(std::string applicationName){
  _renderer=&CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::SchemeManager::getSingleton().create( "TaharezLook.scheme" );
  CEGUI::System::getSingletonPtr()->setDefaultMouseCursor("TaharezLook","MouseArrow");

  _hud=new Hud;
  _mainMenu=new MainMenu;
  _joinGameMenu=new JoinGameMenu;
  _createGameMenu=new CreateGameMenu;
  _lobby=new Lobby;
  _waitingPrompt=new WaitingPrompt;
  _current=_mainMenu;
}
void GuiManager::Reinitialize(){
  delete _hud;
  delete _mainMenu;
  delete _joinGameMenu;
  delete _createGameMenu;
  delete _lobby;
  delete _waitingPrompt;
  CEGUI::OgreRenderer::destroySystem();
  Initialize("");
}
bool GuiManager::IsExpectingMouse(){
  return _current!=_hud;
}
bool GuiManager::IsExpectingKeyboard(){
  Hud* hud=static_cast<Hud*>(_hud);
  return _current==_joinGameMenu||_current==_createGameMenu||_current==_lobby||(_current==_hud&&hud->IsConsoleVisible());
}
bool GuiManager::CreateGame(const CEGUI::EventArgs& e){
  _current=_createGameMenu;
  _current->Display();
}
bool GuiManager::HostGame(const CEGUI::EventArgs& e){
  _current=_lobby;
  _current->Display();
  LOG("STARTING IN SERVER MODE");
  NetworkManager::instance()->startServer();
  return false;
}
bool GuiManager::JoinGame(const CEGUI::EventArgs& e){
  static_cast<WaitingPrompt*>(_waitingPrompt)->RemoveStart();
  _current=_joinGameMenu;
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
  CEGUI::MouseCursor::getSingletonPtr()->hide();
  LOG("STARTING GAME.");
  GameState::instance()->start();
  Hud* hud=static_cast<Hud*>(_hud);
  return false;
}
bool GuiManager::ConnectToNamedHost(const CEGUI::EventArgs& e){
  Connect(static_cast<JoinGameMenu*>(_joinGameMenu)->ReadNamedHost());
}
bool GuiManager::ConnectToSelectedHost(const CEGUI::EventArgs& e){
  Connect(static_cast<JoinGameMenu*>(_joinGameMenu)->ReadSelectedHost());
}
bool GuiManager::Connect(const char* host){
  LOG("STARTING IN CLIENT MODE");
  NetworkManager::instance()->startClient(host);
  _current=_lobby;
  _current->Display();
  return false;
}
void GuiManager::EnableStart(){
  static_cast<WaitingPrompt*>(_waitingPrompt)->EnableStart();
}
bool GuiManager::BackToMainMenu(const CEGUI::EventArgs& e){
  _current=_mainMenu;
  _current->Display();
  return false;
}
void GuiManager::ToggleConsole() {
  Hud* hud=static_cast<Hud*>(_hud);
  hud->ToggleConsole();
}
bool GuiManager::IsConsoleVisible() {
  Hud* hud=static_cast<Hud*>(_hud);
  return hud->IsConsoleVisible();
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

Hud::Hud():Gui("Hud.layout"){
  _healthBar=static_cast<CEGUI::ProgressBar*>(_root->getChild("Hud/HealthBar"));
  _fuelBar=static_cast<CEGUI::ProgressBar*>(_root->getChild("Hud/FuelBar"));
  _ammoCount=_root->getChild("Hud/AmmoCount");
  _magCount=_root->getChild("Hud/MagCount");
  _console=_root->getChild("Hud/Console");
  _consoleInput = static_cast<CEGUI::Editbox*>(_console->getChild("Hud/ConsoleInput"));
  _consoleText  = static_cast<CEGUI::MultiLineEditbox*>(_console->getChild("Hud/ConsoleText"));
  _consoleInput->subscribeEvent(
    CEGUI::Editbox::EventTextAccepted,
    CEGUI::Event::Subscriber(&Hud::ChatSubmitted, this)
  );
  _root->removeChildWindow(_console);
  _consoleSize = 0;
}
void Hud::UpdateHealth(float percentHealth){
  _healthBar->setProgress(percentHealth);
}
void Hud::UpdateFuel(float percentFuel){
  _fuelBar->setProgress(percentFuel);
}
void Hud::UpdateAmmoCount(int ammoCount){
  _ammoCount->setText(std::to_string(ammoCount));
}
void Hud::UpdateMagCount(int magCount){
  _magCount->setText(std::to_string(magCount));
}
bool Hud::ChatSubmitted(const CEGUI::EventArgs& e) {
  if (strlen(_consoleInput->getText().c_str()) == 0) return false;
  NetworkManager::instance()->sendChat(_consoleInput->getText().c_str());
  ChatManager::instance()->addMessage("you", _consoleInput->getText().c_str());
  _consoleInput->setText("");
  return false;
}
void Hud::ToggleConsole() {
  if (IsConsoleVisible() && GameState::instance()->isRunning()) {
    _root->removeChildWindow(_console);
  } else {
    _root->addChildWindow(_console);
    _consoleInput->activate();
  }
}
void Hud::SetConsoleText(std::string str) {
  _consoleText->setText(str.c_str());
}
bool Hud::IsConsoleVisible() {
  return !!(_console->getParent());
}
void Hud::UpdateConsole() {
  if (_consoleSize != ChatManager::instance()->size()) {
    _consoleSize = ChatManager::instance()->size();
    SetConsoleText(ChatManager::instance()->getTextForConsole().c_str());
    CEGUI::Scrollbar* scroller = _consoleText->getVertScrollbar();
    float offset = scroller->getDocumentSize() + 100;
    scroller->setScrollPosition(std::max(offset, 0.0f));
  }
}

MainMenu::MainMenu():Gui("MainMenu.layout"){
  _hostGame=static_cast<CEGUI::PushButton*>(_root->getChild("MainMenu/HostGame"));
  _joinGame=static_cast<CEGUI::PushButton*>(_root->getChild("MainMenu/JoinGame"));
  _exit=static_cast<CEGUI::PushButton*>(_root->getChild("MainMenu/Exit"));

  _hostGame->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::CreateGame,GuiManager::instance()));
  _joinGame->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::JoinGame,GuiManager::instance()));
  _exit->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::Exit,GuiManager::instance()));
}
JoinGameMenu::JoinGameMenu():Gui("JoinGameMenu.layout"){
  _name=static_cast<CEGUI::Editbox*>(_root->getChild("JoinGameMenu/Name"));
  _hosts=static_cast<CEGUI::ItemListbox*>(_root->getChild("JoinGameMenu/Hosts"));
  _hostsJoin=static_cast<CEGUI::PushButton*>(_root->getChild("JoinGameMenu/HostsJoin"));
  _host=static_cast<CEGUI::Editbox*>(_root->getChild("JoinGameMenu/Host"));
  _hostJoin=static_cast<CEGUI::PushButton*>(_root->getChild("JoinGameMenu/HostJoin"));

  _hosts->setMultiSelectEnabled(false);
  
  _hostsJoin->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::ConnectToSelectedHost,GuiManager::instance()));
  _hostJoin->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::ConnectToNamedHost,GuiManager::instance()));
}
const char* JoinGameMenu::ReadNamedHost(){
  return _host->getText().c_str();
}
const char* JoinGameMenu::ReadSelectedHost(){
  return _hosts->getSelectedCount()>0?_hosts->getFirstSelectedItem()->getText().c_str():nullptr;
}
const char* JoinGameMenu::ReadName(){
  return _name->getText().c_str();
}
CreateGameMenu::CreateGameMenu():Gui("CreateGameMenu.layout"){
  _name=static_cast<CEGUI::Editbox*>(_root->getChild("CreateGameMenu/Name"));
  _timeLimit=static_cast<CEGUI::Editbox*>(_root->getChild("CreateGameMenu/TimeLimit"));
  _maxPlayers=static_cast<CEGUI::Editbox*>(_root->getChild("CreateGameMenu/MaxPlayers"));
  _teamOrganization=static_cast<CEGUI::Combobox*>(_root->getChild("CreateGameMenu/TeamOrganization"));
  _teamOrganization->addItem(new CEGUI::ListboxTextItem("Free-for-All",0));
  _teamOrganization->addItem(new CEGUI::ListboxTextItem("Team",1));
  _gameType=static_cast<CEGUI::Combobox*>(_root->getChild("CreateGameMenu/GameType"));
  _gameType->addItem(new CEGUI::ListboxTextItem("Death Match",0));
  _gameType->addItem(new CEGUI::ListboxTextItem("Hardcore",1));
  _gameType->addItem(new CEGUI::ListboxTextItem("Pintos",2));
  _continue=static_cast<CEGUI::PushButton*>(_root->getChild("CreateGameMenu/Continue"));
  _continue->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::HostGame,GuiManager::instance()));
}
Lobby::Lobby():Gui("Lobby.layout"){}

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
/*
HostDialog::HostDialog():Gui("HostDialog.layout"){
  _host = static_cast<CEGUI::Editbox*>(_root->getChild("HostDialog/HostName"));
  _name = static_cast<CEGUI::Editbox*>(_root->getChild("HostDialog/PlayerName"));
  // cegui mispells caret V_V
  _host->subscribeEvent(CEGUI::Editbox::EventCaratMoved, CEGUI::Event::Subscriber(&HostDialog::HostCaretMoved, this));
  _name->subscribeEvent(CEGUI::Editbox::EventCaratMoved, CEGUI::Event::Subscriber(&HostDialog::NameCaretMoved, this));
  _connect = static_cast<CEGUI::PushButton*>(_root->getChild("HostDialog/Connect"));
  _connect->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiManager::Connect, GuiManager::instance()));
  _back = static_cast<CEGUI::PushButton*>(_root->getChild("HostDialog/Back"));
  _back->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GuiManager::BackToMainMenu, GuiManager::instance()));
}
const char* HostDialog::ReadHost(){
  return _host->getText().c_str();
}
const char* HostDialog::ReadName(){
  return _name->getText().c_str();
}
*/
// random c helper
char* trim(const char* str) {
  char *s = (char*)str;
  while (*s == ' ') s++;
  return s;
}
/*
bool HostDialog::NameCaretMoved(const CEGUI::EventArgs& e){
  if (strcmp(ReadName(), "Your Name") == 0) {
    _name->setText("");
  }

  if (strcmp(trim(ReadHost()), "") == 0) {
    _host->setText("Server");
  }
}

bool HostDialog::HostCaretMoved(const CEGUI::EventArgs& e){
  if (strcmp(trim(ReadName()), "") == 0) {
    _name->setText("Your Name");
  }

  if (strcmp(ReadHost(), "Server") == 0) {
    _host->setText("");
  }
}
*/
Gui::Gui(std::string layoutFileName):_root(CEGUI::WindowManager::getSingletonPtr()->loadWindowLayout(layoutFileName)){}
void Gui::Display(){
  CEGUI::System::getSingleton().setGUISheet(_root);
}
std::shared_ptr<char> Gui::getText(CEGUI::Window* element){
  auto text=element->getText();
  std::shared_ptr<char> copy(new char[text.length()+1],std::default_delete<char[]>());
  strcpy(copy.get(),text.c_str());
  return copy;
}
