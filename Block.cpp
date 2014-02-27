#include "Block.h"

Block::Block(std::string tag, Scene* scene, 
	int mask, int col_mask, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ) : GameObject(tag, scene)
{
	Transform* tran = this->getComponent<Transform>();

	tran->posX = posX;
	tran->posY = posY;
	tran->posZ = posZ;

	tran->rotX = rotX;
	tran->rotY = rotY;
	tran->rotZ = rotZ;
	tran->rotW = rotW;

	tran->scaleX = scaleX;
	tran->scaleY = scaleY;
	tran->scaleZ = scaleZ;

	mesh = new Mesh(this, mesh_name);
	Ogre::Vector3 box_half_size = mesh->entity->getBoundingBox().getHalfSize();

	rigidbody = new BoxRigidbody(this, btVector3(box_half_size.x * tran->scaleX, box_half_size.y * tran->scaleY, box_half_size.z * tran->scaleZ), 
		0, mask, col_mask);
}

Block::~Block()
{
}

void Block::update()
{
	GameObject::update();
}

