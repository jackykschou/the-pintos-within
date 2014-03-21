#ifndef __GUIManager_h_
#define __GUIManager_h_

#include "common.h"
#include "Singleton.h"

class GUIManager : public Singleton<GUIManager>, public OgreBites::SdkTrayListener {
  public:
	void initialize(const Ogre::String& appName);
	void update(const Ogre::FrameEvent& evt);

	// Debug panel: shows camera information
	bool isDebugPanelVisible();
	void hideDebugPanel();
	void showDebugPanel();
	void toggleDebugPanel();

	// Game over panel: shows game restart button
	void hideGameOverMenu();
	void showGameOverMenu();
	void flipGameOverLabel();

	// HUD panel: shows time and stats
	void hideHUD();
	void showHUD();

	// Waiting (network) menu
	void showWaitingMenu();
	void hideWaitingMenu();

	// Called from the InputManager to inject mouse events
	bool injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool injectMouseMove(const OIS::MouseEvent& evt);

  private:
  	void buildHUD();
  	void updateHUD();
  	void buildDebugPanel();
	void updateDebugPanel();
	void buildGameOverMenu();
	void handleMouseGameOver();
	void buildWaitingMenu();
	
	OgreBites::SdkTrayManager* _trayMgr;
	OgreBites::ParamsPanel*    _debugPanel;
	Ogre::Overlay*             _mainMenuOverlay;
	Ogre::Overlay*             _hudOverlay;
	Ogre::Overlay*             _gameOverOverlay;
	Ogre::Overlay*             _waitingOverlay;
};

#endif
