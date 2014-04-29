#include "GuiManager.h"
#include "ChatManager.h"
#include "Utility.h"

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
    }else if(_current==_joinGameMenu){
      static_cast<JoinGameMenu*>(_joinGameMenu)->UpdateGames();
    }
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
  NetworkManager::instance()->stopServer(); // just in case
  GameState::instance()->current_state=HOST_MENU;
  _current=_createGameMenu;
  _current->Display();
  return false;
}
bool GuiManager::HostGame(const CEGUI::EventArgs& e){
  _current=_lobby;
  _current->Display();
  GameState::instance()->current_state=LOBBY_AS_HOST;
  LOG("STARTING IN SERVER MODE");
  NetworkManager::instance()->startServer();
  return false;
}
bool GuiManager::JoinGame(const CEGUI::EventArgs& e){
  //static_cast<WaitingPrompt*>(_waitingPrompt)->RemoveStart();
  GameState::instance()->current_state=CLIENT_MENU;
  _current=_joinGameMenu;
  _current->Display();
  NetworkManager::instance()->startClientDiscovery();
  return false;
}
bool GuiManager::Exit(const CEGUI::EventArgs& e){
  GraphicsManager::instance()->stopRendering();
  return false;
}
bool GuiManager::Back(const CEGUI::EventArgs& e){
  if(GameState::instance()->current_state==LOBBY_AS_HOST){
    return CreateGame(e);
  }else{
    return JoinGame(e);
  }
}
bool GuiManager::Start(const CEGUI::EventArgs& e){
  _current=_waitingPrompt;
  _current->Display();
  CEGUI::MouseCursor::getSingletonPtr()->hide();
  LOG("STARTING GAME.");
  GameState::instance()->start();
  GameState::instance()->current_state=LOADING;
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
  //NetworkManager::instance()->stopClientDiscovery();
  GameState::instance()->current_state=LOBBY_AS_CLIENT;
  NetworkManager::instance()->startClient(host);
  _current=_lobby;
  _current->Display();
  return false;
}
void GuiManager::EnableStart(){
  static_cast<Lobby*>(_lobby)->EnableStart();
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
std::string GuiManager::GetName() {
  if (NetworkManager::instance()->isClient()) {
    JoinGameMenu* jgm=static_cast<JoinGameMenu*>(_joinGameMenu);
    return std::string(jgm->ReadName());
  } else {
    CreateGameMenu* cgm=static_cast<CreateGameMenu*>(_createGameMenu);
    return std::string(cgm->ReadName());
  }
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
  _hosts=static_cast<CEGUI::Listbox*>(_root->getChild("JoinGameMenu/Hosts"));
  _hostsJoin=static_cast<CEGUI::PushButton*>(_root->getChild("JoinGameMenu/HostsJoin"));
  _host=static_cast<CEGUI::Editbox*>(_root->getChild("JoinGameMenu/Host"));
  _hostJoin=static_cast<CEGUI::PushButton*>(_root->getChild("JoinGameMenu/HostJoin"));

  _hosts->setMultiselectEnabled(false);

  _hostsJoin->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::ConnectToSelectedHost,GuiManager::instance()));
  _hostJoin->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::ConnectToNamedHost,GuiManager::instance()));
  _name->subscribeEvent(CEGUI::Editbox::EventCaratMoved,CEGUI::Event::Subscriber(&JoinGameMenu::DisplayPrompts,this));
  _host->subscribeEvent(CEGUI::Editbox::EventCaratMoved,CEGUI::Event::Subscriber(&JoinGameMenu::DisplayPrompts,this));
  _name->subscribeEvent(CEGUI::Editbox::EventDeactivated,CEGUI::Event::Subscriber(&JoinGameMenu::DisplayPrompts,this));
  _host->subscribeEvent(CEGUI::Editbox::EventDeactivated,CEGUI::Event::Subscriber(&JoinGameMenu::DisplayPrompts,this));

  _hosts->subscribeEvent(CEGUI::Listbox::EventMouseClick,CEGUI::Event::Subscriber([_hosts](const CEGUI::EventArgs& e){
  CEGUI::MouseEventArgs* a=(CEGUI::MouseEventArgs*)(&e);
    auto item=_hosts->getItemAtPoint(a->position);
    if(item){
      _hosts->setItemSelectState(_hosts->getItemAtPoint(a->position),true);
      item->setSelected(true);
    }
    return false;
  }));

  DisplayPrompts(CEGUI::EventArgs{});
}
bool JoinGameMenu::DisplayPrompts(const CEGUI::EventArgs& e){
  displayPrompts(e,std::vector<std::pair<CEGUI::Editbox*,const char*>>{{_name,"Your Name"},{_host,"Server"}});
  return false;
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
void JoinGameMenu::UpdateGames(){
  _hosts->resetList();
  //FF4444AA
  CEGUI::colour c{0.267f,0.267f,0.664f};
  auto games=GameState::instance()->games;
  for(auto i=games.cbegin();i!=games.cend();++i){
    auto label=(*i).first+" \\["+(*i).second.first+"]";
    auto entry=new CEGUI::ListboxTextItem(label);
    entry->setUserData((void*)(&(*i)));
    entry->setSelectionColours(c);
    entry->setSelectionBrushImage("TaharezLook","ListboxSelectionBrush");
    _hosts->addItem(entry);
  }
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
  _name->subscribeEvent(CEGUI::Editbox::EventCaratMoved,CEGUI::Event::Subscriber(&CreateGameMenu::DisplayPrompts,this));
  _timeLimit->subscribeEvent(CEGUI::Editbox::EventCaratMoved,CEGUI::Event::Subscriber(&CreateGameMenu::DisplayPrompts,this));
  _maxPlayers->subscribeEvent(CEGUI::Editbox::EventCaratMoved,CEGUI::Event::Subscriber(&CreateGameMenu::DisplayPrompts,this));
  _name->subscribeEvent(CEGUI::Editbox::EventDeactivated,CEGUI::Event::Subscriber(&CreateGameMenu::DisplayPrompts,this));
  _timeLimit->subscribeEvent(CEGUI::Editbox::EventDeactivated,CEGUI::Event::Subscriber(&CreateGameMenu::DisplayPrompts,this));
  _maxPlayers->subscribeEvent(CEGUI::Editbox::EventDeactivated,CEGUI::Event::Subscriber(&CreateGameMenu::DisplayPrompts,this));
  DisplayPrompts(CEGUI::EventArgs{});
}
bool CreateGameMenu::DisplayPrompts(const CEGUI::EventArgs& e){
  displayPrompts(e,std::vector<std::pair<CEGUI::Editbox*,const char*>>{{_name,"Your Name"},{_timeLimit,"180"},{_maxPlayers,"2"}});
  return false;
}
const char* CreateGameMenu::ReadName() {
  return _name->getText().c_str();
}

Lobby::Lobby():Gui("Lobby.layout"){
  _back  = static_cast<CEGUI::PushButton*>(_root->getChild("Lobby/Back"));
  _back->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::Back,GuiManager::instance()));
  _start=static_cast<CEGUI::PushButton*>(_root->getChild("Lobby/Start"));
  _start->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&GuiManager::Start,GuiManager::instance()));
  DisableStart();
}
void Lobby::DisableStart(){
  _start->disable();
}
void Lobby::EnableStart(){
  _start->enable();
}
void Lobby::RemoveStart(){
  _start->hide();
}
void Lobby::AddStart(){
  _start->show();
}

WaitingPrompt::WaitingPrompt():Gui("WaitingPrompt.layout"){}
// random c helper
char* trim(const char* str) {
  char *s = (char*)str;
  while (*s == ' ') s++;
  return s;
}
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
bool Gui::DisplayPrompts(const CEGUI::EventArgs& e){
  return false;
}
void Gui::displayPrompts(const CEGUI::EventArgs& e,std::vector<std::pair<CEGUI::Editbox*,const char*>> prompts){
  for(auto i=prompts.cbegin();i!=prompts.cend();++i){
    displayPrompt(e,*i);
  }
}
void Gui::displayPrompt(const CEGUI::EventArgs& e,std::pair<CEGUI::Editbox*,const char*> prompt){
  auto box=prompt.first;
  CEGUI::WindowEventArgs* w=(CEGUI::WindowEventArgs*)(&e);
  bool isThis{w->window==box};
  if(dynamic_cast<CEGUI::ActivationEventArgs*>(w)){
    isThis=false;
  }
  if(isThis&&strcmp(trim(box->getText().c_str()),prompt.second)==0){
    box->setText("");
  }else if(!isThis&&strcmp(trim(box->getText().c_str()),"")==0){
    box->setText(prompt.second);
  }
}
