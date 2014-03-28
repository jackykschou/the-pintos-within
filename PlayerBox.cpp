#include "PlayerBox.h"

PlayerBox::PlayerBox(PlayerCharacter* player_p, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ, int col_mask, int col_to_masks) : Component((GameObject*)player_p)
{
	player = player_p;

	Transform* tran = ((GameObject*)player_p)->getComponent<Transform>();

	std::ostringstream id_stream;
  	id_stream << _gameObject->id;

	entity = ((GameObject*)player_p)->scene->manager->createEntity(mesh_name + std::string(" Entity") + id_stream.str(), mesh_name.c_str());
	node = player->mesh->node->createChildSceneNode(mesh_name + std::string(" Node") + id_stream.str(), 
                                                  player->mesh->node->convertWorldToLocalPosition(Ogre::Vector3(posX, posY, posZ)));
  node->attachObject(entity);
  entity->setCastShadows(true);

  node->setPosition(player->mesh->node->convertWorldToLocalPosition(Ogre::Vector3(tran->posX + posX, tran->posY + posY, tran->posZ + posZ)));
  // node->setInheritOrientation(false);
  node->setOrientation(node->convertWorldToLocalOrientation(Ogre::Quaternion(rotW, rotX, rotY, rotZ)));
  node->setInheritScale (false);
  node->setScale (scaleX, scaleY, scaleZ);

  node->setVisible(false);

	Ogre::Vector3 box_half_size = entity->getBoundingBox().getHalfSize();

	btVector3 inertia(0, 0, 0);
	float mass = 0.0f;

	btCollisionShape* collisionShape = new btBoxShape(btVector3(box_half_size.x * scaleX, 
		box_half_size.y * scaleY, box_half_size.z * scaleZ));

	btRigidBody::btRigidBodyConstructionInfo* info = new btRigidBody::btRigidBodyConstructionInfo(mass ,NULL,collisionShape,inertia);

	rigidbody = new BoxRigidbody((GameObject*)player_p, btVector3(box_half_size.x * scaleX, box_half_size.y * scaleY, box_half_size.z * scaleZ), 
		0, col_mask, col_to_masks, info);
}

PlayerBox::~PlayerBox()
{
  delete node;
  delete entity;
}

void PlayerBox::update()
{
	Component::update();
}

