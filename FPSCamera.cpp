#include "FPSCamera.h"

FPSCamera::FPSCamera(GameObject* gameObject, std::string name, double height) : Camera(gameObject, name), cameraMan(0)
{
	_height_offset = height;
	cameraMan = new OgreBites::SdkCameraMan(camera);   // create a default camera controller
}

FPSCamera::~FPSCamera()
{
	delete cameraMan;
}


void FPSCamera::update()
{
	camera->setPosition(_transform->posX, _transform->posY + _height_offset, _transform->posZ);
	cameraMan->frameRenderingQueued(*(GraphicsManager::instance()->getFrameEvent()));
	OIS::MouseEvent* evt = InputManager::instance()->getMouseMovedEvent();
	if (evt) cameraMan->injectMouseMove(*evt);
}

