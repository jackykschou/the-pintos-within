#ifndef __GUIManager_h_
#define __GUIManager_h_

#include "common.h"
#include "GameState.h"
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

	// Main menu panel: shows game start button
	void hideMainMenu();
	void showMainMenu();

	// HUD panel: shows time and stats
	void hideHUD();
	void showHUD();


	// Called from the InputManager to inject mouse events
	bool injectMouseUp(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool injectMouseMove(const OIS::MouseEvent& evt);

  private:
  	void buildHUD();
  	void updateHUD();

  	void buildMainMenu();

	void buildDebugPanel();
	void updateDebugPanel();
	
	OgreBites::SdkTrayManager* _trayMgr;
	OgreBites::ParamsPanel*    _debugPanel;
	Ogre::Overlay*             _mainMenuOverlay;
	Ogre::Overlay*             _hudOverlay;
};

#endif
