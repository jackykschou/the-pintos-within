#ifndef __FPSCamera_h_
#define __FPSCamera_h_

#include <OISEvents.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <SdkCameraMan.h>
#include <OgreQuaternion.h>
#include "Camera.h"
#include "Debug.h"
#include "common.h"

#include "GraphicsManager.h"

class FPSCamera : Camera
{
protected:
	float timer;
	float bobbingAmount;
	float bobOffsetY;
	float bobSpeed;
	double _height_offset;
	void clampCameraRotation();
	void updateTransformRotation();
public:

	bool is_running;
	bool is_walking;
	FPSCamera(GameObject*, std::string, double);
	virtual ~FPSCamera();
	Ogre::SceneNode* node;

	OgreBites::SdkCameraMan* cameraMan;

	virtual void update();

};

#endif // #ifndef __FPSCamera_h_
