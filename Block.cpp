#include "Block.h"

Block::Block(std::string tag, Scene* scene, 
	int mask, int col_mask, std::string mesh_name) : GameObject(tag, scene)
{
	new Transform(this);
	mesh = new Mesh(this, mesh_name);
	Ogre::Vector3 box_half_size = mesh->entity->getBoundingBox().getHalfSize();
	rigidbody = new BoxRigidbody(this, btVector3(box_half_size.x, box_half_size.y, box_half_size.z), 0, mask, col_mask);
}

Block::~Block()
{
}

void Block::update()
{
	GameObject::update();
}