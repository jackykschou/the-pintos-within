#include <Mesh.h>

Mesh::Mesh(GameObject* gameObject, std::string mesh_name) : Component(gameObject)
{
  if((_transform = gameObject.GetComponent<Transform>()) == NULL)
  {
    _transform = GameObject.addComponent(new Transform());
  }

	std::ostringstream entity_stream;
  entity_stream << (Mesh::entity_id_assigner++);

  std::ostringstream node_stream;
  node_stream << (Mesh::node_id_assigner++);

	entity = SceneManager::instance()->current_scene->createEntity((std::string("Entity") + entity_stream.str()).c_str(), mesh_name.c_str());
	node = _gameObject->scene->manager->getRootSceneNode()->createChildSceneNode(((std::string("Node") + node_stream.str()).c_str(), 
                                                                                Ogre::Vector3(_transform.posX, _transform.posY, _transform.posZ));
  node->attachObject(entity);
  entity->setCastShadows(true);
}

~Mesh()
{
  _gameObject->scene->manager->getRootSceneNode()->removeChild(node);

  delete node;
  delete entity;
}

void Mesh::update()
{
  Component::update();

  node->setPosition(_transform.posX, _transform.posY, _transform.posZ);
  node->setOrientation(Ogre::Quaternion(_transform.rotW, _transform.rotX, _transform.rotY, _transform.rotZ));
  node->setScale (_transform.scaleX, _transform.scaleY, _transform.scaleZ);
}

