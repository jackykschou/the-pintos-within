#include "Ball.h"

Ball::Ball(std::string tag, Scene* scene, 
	int mask, int col_mask, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ, btVector3 init_force) : GameObject(tag, scene)
{
	_transform = this->getComponent<Transform>();
	_transform->posX = posX;
	_transform->posY = posY;
	_transform->posZ = posZ;

	_transform->rotX = rotX;
	_transform->rotY = rotY;
	_transform->rotZ = rotZ;
	_transform->rotW = rotW;

	_transform->scaleX = scaleX;
	_transform->scaleY = scaleY;
	_transform->scaleZ = scaleZ;

	debouncer = new Debouncer(DEBOUNCE_BALL, []() {
		AudioManager::instance()->playDonk();
	});

	mesh = new Mesh(this, mesh_name);

	float radius = mesh->entity->getBoundingRadius();

	float radius_scale = std::max(scaleX, std::max(scaleY,scaleZ));

	btVector3 inertia(0, 0, 0);
	float mass = 10.0f;
	btCollisionShape* collisionShape = new btSphereShape(radius * radius_scale);
	btRigidBody::btRigidBodyConstructionInfo* info = new btRigidBody::btRigidBodyConstructionInfo(mass ,NULL,collisionShape,inertia);
	info->m_restitution = 0.8f;
	info->m_friction = 0.1f;

	collided = 0;

	rigidbody = new SphereRigidbody(this, 10, 10, mask, col_mask, info);

	((Rigidbody*)rigidbody)->rigidbody->setGravity(btVector3(0, -10, 0));
	((Rigidbody*)rigidbody)->rigidbody->setLinearVelocity(init_force);

	auto fun = [](btVector3 v1, btVector3 v2, GameObject* itself, GameObject* other) 
				{
					Ball *ball = (Ball*)itself;
					ball->debouncer->run();
					if((other->tag) == std::string("Player") && !ball->collided && GameState::instance()->isRunning())
					{
						GameState::instance()->score++;
						itself->scene->removeGameObject(itself);
						ball->collided = true;
					}
				};

	((Rigidbody*)rigidbody)->onCollision = fun;
}

Ball::~Ball()
{
	delete debouncer;
	debouncer = NULL;
}

void Ball::update()
{
	GameObject::update();
}
