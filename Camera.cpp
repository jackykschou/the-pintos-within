#include "Camera.h"

Camera::Camera(GameObject* gameObject, std::string name) : Component(gameObject)
{
	_transform = gameObject->getComponent<Transform>();

	this->name = name.c_str();

	camera = SceneManager::instance()->current_scene->manager->createCamera(name);

	viewport = GraphicsManager::instance()->getRenderWindow()->addViewport(camera);
	viewport->setBackgroundColour(Ogre::ColourValue(0,1,0));

	camera->setAspectRatio(
	    Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
	camera->setPosition(_transform->posX, _transform->posY, _transform->posZ);
	camera->setOrientation(Ogre::Quaternion(_transform->rotW, _transform->rotX, _transform->rotY, _transform->rotZ));
	camera->lookAt(0, 0, 0);
	camera->setNearClipDistance(1);
}

Camera::~Camera()
{
	delete camera;
	delete viewport;
}

void Camera::update()
{
	Component::update();
	camera->setPosition(_transform->posX, _transform->posY, _transform->posZ);
	camera->setOrientation(Ogre::Quaternion(_transform->rotW, _transform->rotX, _transform->rotY, _transform->rotZ));
}
