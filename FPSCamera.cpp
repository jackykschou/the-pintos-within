#include "FPSCamera.h"
#include <math.h>
#define PI 3.14159

FPSCamera::FPSCamera(GameObject* gameObject, std::string name, double height_offset) : Camera(gameObject, name), cameraMan(0)
{
	_height_offset = height_offset;
	cameraMan = new OgreBites::SdkCameraMan(camera);   // create a default camera controller
	timer = 0.0;
	bobSpeed = 15.0;
	bobOffsetY = 0.0;
	bobbingAmount = 2.0;
}

FPSCamera::~FPSCamera()
{
	delete cameraMan;
}

void FPSCamera::update()
{
	float timeSince = GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame;
	timer += timeSince*bobSpeed;
	float waveslice = sin(timer);
	if (timer > PI * 2) {
		timer = fmod(timer, (PI * 2));w
	}
	if(waveslice != 0) {
		bobOffsetY = waveslice * bobbingAmount;
	}

	camera->setPosition(_transform->posX, _transform->posY + _height_offset + bobOffsetY, _transform->posZ);
	cameraMan->frameRenderingQueued(*(GraphicsManager::instance()->getFrameEvent()));
	
	OIS::MouseEvent* evt = InputManager::instance()->getMouseMovedEvent();
	if (evt) cameraMan->injectMouseMove(*evt);

	clampCameraRotation();
  	// updateTransformRotation();
}

void FPSCamera::clampCameraRotation()
{
	Ogre::Vector3 dir = camera->getDirection();
    if (dir.y>.9) dir.y=.9;
    if (dir.y<-.9) dir.y=-.9;
    camera->setDirection(dir);
}

void FPSCamera::updateTransformRotation()
{
	Ogre::Quaternion* old_q = new Ogre::Quaternion(_transform->rotW, _transform->rotX, _transform->rotY, _transform->rotZ);
	Ogre::Quaternion* new_q = new Ogre::Quaternion(old_q->xAxis(), camera->getOrientation().yAxis(), old_q->zAxis());

	_transform->rotW = new_q->w;
	_transform->rotX = new_q->x;
	_transform->rotY = new_q->y;
	_transform->rotZ = new_q->z;

	delete old_q;
	delete new_q;
}
