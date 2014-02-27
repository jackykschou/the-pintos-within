#include "ResourcesManager.h"

void ResourcesManager::initialize(Ogre::String resourcesCfg, Ogre::String pluginsCfg)
{
    _resourcesCfg = resourcesCfg;
    _pluginsCfg = pluginsCfg;

    setupResources();

    // Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();

    // Load resources
    loadResources();
}

void ResourcesManager::setupResources()
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(_resourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

void ResourcesManager::createResourceListener()
{
}

void ResourcesManager::loadResources()
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
    // Load the system fonts
    Ogre::ResourceManager::ResourceMapIterator iter = Ogre::FontManager::getSingleton().getResourceIterator();
    while (iter.hasMoreElements()) { iter.getNext()->load(); }
}

Ogre::String ResourcesManager::getResourcesCfg()
{
    return _resourcesCfg;
}

Ogre::String ResourcesManager::getPluginsCfg()
{
    return _pluginsCfg;
}



