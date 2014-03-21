#include "Block.h"

Block::Block(Scene* scene, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ) : GameObject("Block", scene)
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

	btVector3 inertia(0, 0, 0);
	float mass = 0.0f;


	btCollisionShape* collisionShape = new btBoxShape(btVector3(box_half_size.x * tran->scaleX, 
		box_half_size.y * tran->scaleY, box_half_size.z * tran->scaleZ));

	btRigidBody::btRigidBodyConstructionInfo* info = new btRigidBody::btRigidBodyConstructionInfo(mass ,NULL,collisionShape,inertia);
	info->m_restitution = 1.0f;
	info->m_friction = 0.1f;

	rigidbody = new BoxRigidbody(this, btVector3(box_half_size.x * tran->scaleX, box_half_size.y * tran->scaleY, box_half_size.z * tran->scaleZ), 
		0, COL_STATIC, STATIC_COLLIDER_WITH, info);
	((Rigidbody*)rigidbody)->rigidbody->setUserPointer(NULL);
}

Block::~Block()
{
}

void Block::update()
{
	GameObject::update();
}

