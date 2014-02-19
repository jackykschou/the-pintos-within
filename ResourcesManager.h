#ifndef __ResourcesManager_h_
#define __ResourcesManager_h_

#include <OgreConfigFile.h>

#include "common.h"
#include "GraphicsManager.h"

#define RESOURCES_PATH "resources.cfg"
#define RESOURCES_DEBUG_PATH "resources_d.cfg"

#define PLUGIN_PATH "plugins.cfg"
#define PLUGIN_DEBUG_PATH "plugins_d.cfg"

class ResourcesManager : public Singleton<ResourcesManager>
{
public:
    virtual void initialize();

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
