#include "GUIManager.h"

void GUIManager::initialize(string appName) {
	_mTrayMgr = new OgreBites::SdkTrayManager(
		appName,
		GraphicsManager::instance()->getRenderWindow(),
		GraphicsManager::instance()->getMouse(),
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

    _mDetailsPanel = _mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    _mDetailsPanel->setParamValue(9, "Bilinear");
    _mDetailsPanel->setParamValue(10, "Solid");
    _mDetailsPanel->hide();
}
