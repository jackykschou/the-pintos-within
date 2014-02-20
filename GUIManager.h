#ifndef __GUIManager_h_
#define __GUIManager_h_

#include "common.h"

class GUIManager : public Singleton<GUIManager>, public OgreBites::SdkTrayListener {
  public:
	void initialize(const Ogre::String& appName);
	bool isVisible();

  private:
  	OgreBites::SdkTrayManager* _mTrayMgr;
  	OgreBites::ParamsPanel*    _mDetailsPanel;
};

#endif
