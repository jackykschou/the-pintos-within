#ifndef __GraphicsManager_h_
#define __GraphicsManager_h_

#include <OgreRenderWindow.h>
#include <OgreRoot.h>

#include <string>

class GraphicsManager : Singleton<GraphicsManager>, public Ogre::FrameListener
{
public:
	~GraphicsManager();

	void initialize(std::string);
	Ogre::Root* getRenderRoot();
	Ogre::RenderWindow* getRenderWindow();
	void startRendering();
	void stopRendering();

protected:
	Ogre::Root* _root;
	Ogre::RenderWindow* _window;
	bool _shutdown;

	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void windowClosed();
	void configureWindow(std::string);
	void initializeRoot();
	void createFrameListener();
};

#endif // #ifndef __GraphicsManager_h_