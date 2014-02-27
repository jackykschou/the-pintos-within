#include "GUIManager.h"
#include "InputManager.h"
#include "Camera.h"

void GUIManager::initialize(const Ogre::String& appName) {
	_trayMgr = new OgreBites::SdkTrayManager(
		appName,
		GraphicsManager::instance()->getRenderWindow(),
		InputManager::instance()->getMouse(),
		this
	);

    _trayMgr->hideCursor();

    buildDebugPanel();
    hideDebugPanel();

    buildMainMenu();
    buildHUD();
    showHUD();
}

// Upadtes the stats in the tray
void GUIManager::update(const Ogre::FrameEvent& evt) {
    _trayMgr->frameRenderingQueued(evt);
    updateDebugPanel();
    updateHUD();
}

// Turns the display on/off
void GUIManager::toggleDebugPanel() {
    if (isDebugPanelVisible()) {
        hideDebugPanel();
    } else {
        showDebugPanel();
    }
}

void GUIManager::hideDebugPanel() {
    _trayMgr->removeWidgetFromTray(_debugPanel);
    _trayMgr->hideFrameStats();
    _debugPanel->hide();
}

void GUIManager::showDebugPanel() {
    _trayMgr->moveWidgetToTray(_debugPanel, OgreBites::TL_TOPRIGHT, 0);
    _trayMgr->showFrameStats(OgreBites::TL_TOPRIGHT);
    _debugPanel->show();
}

void GUIManager::updateDebugPanel() {
    if (!isDebugPanelVisible()) { return; }
    Ogre::Camera *camera = SceneManager::instance()->current_scene->main_camera->camera;
    _debugPanel->setParamValue(0, Ogre::StringConverter::toString(camera->getDerivedPosition().x));
    _debugPanel->setParamValue(1, Ogre::StringConverter::toString(camera->getDerivedPosition().y));
    _debugPanel->setParamValue(2, Ogre::StringConverter::toString(camera->getDerivedPosition().z));
    _debugPanel->setParamValue(4, Ogre::StringConverter::toString(camera->getDerivedOrientation().w));
    _debugPanel->setParamValue(5, Ogre::StringConverter::toString(camera->getDerivedOrientation().x));
    _debugPanel->setParamValue(6, Ogre::StringConverter::toString(camera->getDerivedOrientation().y));
    _debugPanel->setParamValue(7, Ogre::StringConverter::toString(camera->getDerivedOrientation().z));
}

// create a params panel for displaying sample details
void GUIManager::buildDebugPanel() {
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    _debugPanel = _trayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 300, items);
    _debugPanel->setParamValue(9, "Bilinear");
    _debugPanel->setParamValue(10, "Solid");
}

bool GUIManager::isDebugPanelVisible() {
    return _debugPanel->getTrayLocation() != OgreBites::TL_NONE;
}

// MAIN MENU function

void GUIManager::hideMainMenu() {
    _mainMenuOverlay->hide();
}

void GUIManager::showMainMenu() {
    _mainMenuOverlay->show();
}

void GUIManager::buildMainMenu() {
    _mainMenuOverlay = static_cast< Ogre::Overlay* >( Ogre::OverlayManager::getSingleton().getByName("MyOverlays/MainMenuOverlay"));
}

// HUD functions

void GUIManager::hideHUD() {
    _hudOverlay->hide();
}

void GUIManager::showHUD() {
    _hudOverlay->show();
}

void GUIManager::buildHUD() {
    _hudOverlay = static_cast< Ogre::Overlay* >( Ogre::OverlayManager::getSingleton().getByName("MyOverlays/HUDOverlay"));    
}

void GUIManager::updateHUD() {
    Ogre::OverlayElement* score = _hudOverlay->getChild("HUDPanel")->getChild("Score");
    char str[8];
    snprintf(str, 8, "%d", GameState::instance()->score);
    score->setCaption(str);

    Ogre::OverlayElement* clock = _hudOverlay->getChild("HUDPanel")->getChild("Clock");
    int seconds = GameState::instance()->timeLeft % 60;
    int minutes = GameState::instance()->timeLeft / 60;
    snprintf(str, 8, "%d:%02d", minutes, seconds);
    clock->setCaption(str);
}


// Callbacks from InputManager

bool GUIManager::injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id) {
    return _trayMgr->injectMouseUp(evt, id);
}

bool GUIManager::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id) {
    return _trayMgr->injectMouseDown(evt, id);
}

bool GUIManager::injectMouseMove(const OIS::MouseEvent& evt) {
    return _trayMgr->injectMouseMove(evt);
}
