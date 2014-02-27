#include "Ball.h"

Ball(GameObject*);
virtual ~Ball();

Mesh* mesh;
SphereRigidbody* rigidbody;

virtual void update();

Ball::Ball(std::string tag, Scene* scene, 
	int mask, int col_mask, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ) : GameObject(tag, scene)
{
	Transform* tran = new Transform(this);
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
	float radius = mesh->entity->getBoundingRadius();
	rigidbody = new SphereRigidbody(this, radius, 10, mask, col_mask);

	auto fun = [](btVector3 v1, btVector3 v2, GameObject* g) {LOG("There!"); };

	((Rigidbody*)rigidbody)->onCollision = fun;
}

Ball::~Ball()
{
}

void Ball::update()
{
	GameObject::update();
}