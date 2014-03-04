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

class FPSCamera : Camera
{
protected:
	long timer;
	long bobSpeed;
	float bobbingAmount;
	float bobOffsetY;
	double _height_offset;
	void clampCameraRotation();
	void updateTransformRotation();
public:

	FPSCamera(GameObject*, std::string, double);
	virtual ~FPSCamera();

	OgreBites::SdkCameraMan* cameraMan;

	virtual void update();

};

#endif // #ifndef __FPSCamera_h_
