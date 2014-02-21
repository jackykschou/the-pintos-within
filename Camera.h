#ifndef __Camera_h_
#define __Camera_h_

#include <OgreCamera.h>
#include <OgreViewport.h>
#include "Component.h"
#include "Transform.h"
#include "common.h"

class Camera : Component
{
protected:
	Transform* _transform;

public:
	Camera(GameObject*, std::string);
	~Camera();

	Ogre::Viewport* viewport;
	Ogre::Camera* camera;

	virtual void update();
};

#endif // #ifndef __Camera_h_
