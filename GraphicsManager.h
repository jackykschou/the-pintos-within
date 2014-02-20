#ifndef __GraphicsManager_h_
#define __GraphicsManager_h_

#include <OgreRenderWindow.h>
#include <OgreRoot.h>

#include "common.h"

#include "SceneManager.h"
#include "ResourcesManager.h"
#include "InputManager.h"

class GraphicsManager : public Singleton<GraphicsManager>, public Ogre::FrameListener
{
public:
	~GraphicsManager();

	void initialize(std::string, Ogre::String);
	Ogre::Root* getRenderRoot();
	Ogre::RenderWindow* getRenderWindow();
	void startRendering();
	void stopRendering();
	Ogre::FrameEvent* getFrameEvent();

protected:
	Ogre::Root* _root;
	Ogre::RenderWindow* _window;
	bool _shutdown;
	Ogre::FrameEvent* _frame_event;

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void configureWindow(std::string);
	void initializeRoot(Ogre::String);
	void createFrameListener();
};

#endif // #ifndef __GraphicsManager_h_