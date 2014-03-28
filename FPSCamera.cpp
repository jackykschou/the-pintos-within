#include "FPSCamera.h"

#include "PlayerCharacter.h"

#include <math.h>
#define PI 3.14159
#define PIBYTWO 1.570795

FPSCamera::FPSCamera(GameObject* gameObject, std::string name, double height_offset, Ogre::SceneNode* node_p) : Camera(gameObject, name), cameraMan(0)
{
	node = node_p;
	_height_offset = height_offset;
	cameraMan = new OgreBites::SdkCameraMan(camera);   // create a default camera controller
	timer = 0.0;
	bobSpeed = 15.0;
	bobOffsetY = 0.0;
	bobOffsetX = 0.0;
	bobOffsetZ = 0.0;
	bobbingAmountX = 0.8;
	bobbingAmountY = 0.8;
	bobbingAmountZ = 0.8;

	camera->setPosition(_transform->posX, _transform->posY, _transform->posZ);
	camera->setOrientation(Ogre::Quaternion(_transform->rotW, _transform->rotX, _transform->rotY, _transform->rotZ));
	camera->setNearClipDistance(35.0f);

	camera->yaw(-Ogre::Degree(180));
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
	if (timer > PI * 2) 
	{
		timer = fmod(timer, (PI * 2));
	}
	if(waveslice != 0) 
	{
		if(((PlayerCharacter*)_gameObject)->is_running)
		{
			bobOffsetX = waveslice * bobbingAmountX;
			bobOffsetY = waveslice * bobbingAmountY;
			bobOffsetZ = waveslice * bobbingAmountZ;
		}
		else if(((PlayerCharacter*)_gameObject)->is_moving)
		{
			bobOffsetX = waveslice * (bobbingAmountX + 0.3);
			bobOffsetY = waveslice * (bobbingAmountY + 0.3);
			bobOffsetZ = waveslice * (bobbingAmountZ + 0.3);
		}
		else
		{
			bobOffsetY = 0.0f;
			bobOffsetX = 0.0f;
			bobOffsetZ = 0.0f;
		}
	}

	// cameraMan->frameRenderingQueued(*(GraphicsManager::instance()->getFrameEvent()));
	
	// OIS::MouseEvent* evt = InputManager::instance()->getMouseMovedEvent();
	// if (evt) cameraMan->injectMouseMove(*evt);

  	updateTransformRotation();
  	clampCameraRotation();
}

void FPSCamera::clampCameraRotation()
{
	Ogre::Vector3 dir = camera->getDirection();
    if (dir.y>.6) dir.y=.6;
    if (dir.y<-.35) dir.y=-.35;
    camera->setDirection(dir);
}

void FPSCamera::updateTransformRotation()
{
	OIS::MouseEvent* evt = InputManager::instance()->getMouseMovedEvent();
	Ogre::Quaternion new_q = node->getOrientation();
	if(evt)
	{
		if(node)
		{
			node->yaw(-Ogre::Degree(evt->state.X.rel * 0.1f));
			new_q = node->getOrientation();
		}
	}

	// float angle = atan(camera->getDirection().x/camera->getDirection().z);
	// angle *= 2;
	// Ogre::Quaternion* new_q = new Ogre::Quaternion((Ogre::Radian)angle, Ogre::Vector3(0,1,0));

	_transform->rotW = new_q.w;
	_transform->rotX = new_q.x;
	_transform->rotY = new_q.y;
	_transform->rotZ = new_q.z;

	Ogre::Vector3 dir = node->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;

	Ogre::Vector3 to = Ogre::Vector3(_transform->posX, _transform->posY, _transform->posZ) + (dir * 100);

	camera->setPosition(to.x + bobOffsetX, to.y + 30 + bobOffsetY, to.z + bobOffsetZ);


	if(evt && node)
	{
		camera->yaw(-Ogre::Degree(evt->state.X.rel * 0.1f));
		camera->pitch(-Ogre::Degree(evt->state.Y.rel * 0.1f));
	}
}
