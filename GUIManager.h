#ifndef __GUIManager_h_
#define __GUIManager_h_

#include "common.h"
#include "Singleton.h"

class GUIManager : public Singleton<GUIManager>, public OgreBites::SdkTrayListener {
  public:
	void initialize(const Ogre::String& appName);
	void update(const Ogre::FrameEvent& evt);

	bool isTrayVisible();
	void hideTray();
	void showTray();
	void toggleTray();

  private:
	void updateTray();
	OgreBites::SdkTrayManager* _mTrayMgr;
	OgreBites::ParamsPanel*    _mDetailsPanel;
};

#endif
