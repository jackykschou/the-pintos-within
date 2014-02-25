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



  private:
	void buildDebugPanel();
	void updateDebugPanel();
	OgreBites::SdkTrayManager* _trayMgr;
	OgreBites::ParamsPanel*    _debugPanel;
};

#endif
