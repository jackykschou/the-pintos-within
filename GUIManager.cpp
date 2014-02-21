#include "GUIManager.h"
#include "InputManager.h"
#include "Camera.h"

void GUIManager::initialize(const Ogre::String& appName) {
	_mTrayMgr = new OgreBites::SdkTrayManager(
		appName,
		GraphicsManager::instance()->getRenderWindow(),
		InputManager::instance()->getMouse(),
		this
	);

    _mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    _mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    _mTrayMgr->hideCursor();

    // create a params panel for displaying sample details
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

    _mDetailsPanel = _mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 500, items);
    _mDetailsPanel->setParamValue(9, "Bilinear");
    _mDetailsPanel->setParamValue(10, "Solid");
}

// Upadtes the stats in the tray
void GUIManager::update(const Ogre::FrameEvent& evt) {
    _mTrayMgr->frameRenderingQueued(evt);
    updateTray();
}

void GUIManager::updateTray() {
    if (!isTrayVisible()) { return; }
    Ogre::Camera *camera = SceneManager::instance()->current_scene->main_camera->camera;
    _mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(camera->getDerivedPosition().x));
    _mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(camera->getDerivedPosition().y));
    _mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(camera->getDerivedPosition().z));
    _mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(camera->getDerivedOrientation().w));
    _mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(camera->getDerivedOrientation().x));
    _mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(camera->getDerivedOrientation().y));
    _mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(camera->getDerivedOrientation().z));
}

// Turns the display on/off
void GUIManager::toggleTray() {
    if (isTrayVisible()) {
        hideTray();
    } else {
        showTray();
    }
}

void GUIManager::hideTray() {
    _mTrayMgr->removeWidgetFromTray(_mDetailsPanel);
    _mDetailsPanel->hide();
}

void GUIManager::showTray() {
    _mTrayMgr->moveWidgetToTray(_mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
    _mDetailsPanel->show();
}

bool GUIManager::isTrayVisible() {
    return _mDetailsPanel->getTrayLocation() != OgreBites::TL_NONE;
}
