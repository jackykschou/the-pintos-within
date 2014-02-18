#ifndef __Camera_h_
#define __Camera_h_

#include "common.h"

class Camera : Component
{
protected:
	Transform* _transform;

public:
	virtual Camera(GameObject gameObject, std::string name);
	virtual ~Camera();

	Ogre::Viewport* view_port;
	Ogre::Camera* camera;

	virtual void update();
};

#endif // #ifndef __Camera_h_
