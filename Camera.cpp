#include "Camera.h"

void Camera::Camera(GameObject gameObject, std::string name) : Component(gameObject)
{
	if((_transform = gameObject.GetComponent<Transform>()) == NULL)
	{
		_transform = GameObject.addComponent(new Transform());
	}

	camera = SceneManager->_current_scene->manager->createCamera(name);

  camera->setPosition(Ogre::Vector3(0,0,0));
  camera->lookAt(Ogre::Vector3(0,0,0));
  camera->setNearClipDistance(5);

  camera->setAspectRatio(
      Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	viewport = GraphicsManager::instance()->getRenderWindow()->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
}

void Camera::~Camera()
{
	delete camera;
	delete viewport;
}

void update()
{
	Component::update();

	camera->setPosition(_transform.posX, _transform.posY, _transform.posZ);
	camera->setOrientation(Ogre::Quaternion(_transform.rotW, _transform.rotX, _transform.rotY, _transform.rotZ));
}
