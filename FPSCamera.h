#ifndef __FPSCamera_h_
#define __FPSCamera_h_

#include "common.h"

class FPSCamera : Camera
{
protected:
	double _height_offset;
public:

	virtual FPSCamera(GameObject gameObject, std::string name);
	virtual ~FPSCamera();

	Ogre::Viewport* view_port;
	Ogre::Camera* camera;
	OgreBites::SdkCameraMan* cameraMan;

	virtual void update();

};

#endif // #ifndef __FPSCamera_h_