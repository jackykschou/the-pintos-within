#include "Camera.h"

Camera::Camera(GameObject* gameObject, std::string name) : Component(gameObject)
{
	if((_transform = gameObject->getComponent<Transform>()) == NULL)
	{
		_transform = new Transform(gameObject);
		gameObject->addComponent(_transform);
	}

	camera = SceneManager::instance()->current_scene->manager->createCamera(name);

	camera->setPosition(Ogre::Vector3(0,0,0));
	camera->lookAt(Ogre::Vector3(0,0,0));
	camera->setNearClipDistance(5);

	viewport = GraphicsManager::instance()->getRenderWindow()->addViewport(camera);
	viewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

	camera->setAspectRatio(
	    Ogre::Real(viewport->getActualWidth()) / Ogre::Real(viewport->getActualHeight()));
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
