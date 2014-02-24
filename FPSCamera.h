#ifndef __FPSCamera_h_
#define __FPSCamera_h_

#include <OISEvents.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkCameraMan.h>

#include "Camera.h"

#include "common.h"

class FPSCamera : Camera
{
protected:
	double _height_offset;
public:

	FPSCamera(GameObject*, std::string, double);
	virtual ~FPSCamera();

	OgreBites::SdkCameraMan* cameraMan;

	virtual void update();

};

#endif // #ifndef __FPSCamera_h_