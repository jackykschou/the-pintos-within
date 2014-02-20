#ifndef __GraphicsManager_h_
#define __GraphicsManager_h_

#include <OgreRenderWindow.h>
#include <OgreRoot.h>

#include "common.h"

#include "SceneManager.h"
#include "ResourcesManager.h"

class GraphicsManager : public Singleton<GraphicsManager>, public Ogre::FrameListener
{
public:
	~GraphicsManager();

	void initialize(std::string);
	Ogre::Root* getRenderRoot();
	Ogre::RenderWindow* getRenderWindow();
	void startRendering();
	void stopRendering();
	Ogre::FrameEvent* getFrameEvent();

protected:
	Ogre::Root* _root;
	Ogre::RenderWindow* _window;
	clock_t last_render_time;
	bool _shutdown;
	Ogre::FrameEvent* _frame_event;

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void windowClosed();
	void configureWindow(std::string);
	void initializeRoot();
	void createFrameListener();
};

#endif // #ifndef __GraphicsManager_h_