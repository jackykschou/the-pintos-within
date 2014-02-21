#ifndef __ResourcesManager_h_
#define __ResourcesManager_h_

#include "common.h"
#include "Singleton.h"

class ResourcesManager : public Singleton<ResourcesManager>
{

public:
    virtual void initialize(Ogre::String, Ogre::String);

    Ogre::String getResourcesCfg();
    Ogre::String getPluginsCfg();
protected:
    Ogre::String _resourcesCfg;
    Ogre::String _pluginsCfg;

    virtual void setupResources();
    virtual void createResourceListener();
    virtual void loadResources();
};

#endif // #ifndef __ResourcesManager_h_
