#ifndef __Camera_h_
#define __Camera_h_

#include "common.h"

class Camera : Component
{
	private:
		 Ogre::Viewport* _view_port;
		 Ogre::Camera* _camera;
	public:
		Camera();
		virtual ~Camera(void);



};

#endif // #ifndef __Camera_h_
