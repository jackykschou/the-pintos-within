#include "PlayerPickUp.h"
#include "WeaponPickUp.h"
#include "PlayerCharacter.h"
#include "Mesh.h"
#include "SphereRigidbody.h"
#include "GameState.h"
#include "AudioManager.h"
#include "Debouncer.h"

PlayerPickUp::PlayerPickUp(std::string tag, Scene* scene, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ) : GameObject(tag, scene)
{
	pick_debouncer = new Debouncer(1000*1000);

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

	mesh = new Mesh(this, mesh_name);

	float radius = mesh->entity->getBoundingRadius();

	btVector3 inertia(0, 0, 0);
	float mass = 100.0f;
	float biggest_scale = std::max(std::max(scaleX, scaleY), scaleZ);
	btCollisionShape* collisionShape = new btSphereShape(radius);
	btRigidBody::btRigidBodyConstructionInfo* info = new btRigidBody::btRigidBodyConstructionInfo(mass ,NULL,collisionShape,inertia);

	collided = 0;

	rigidbody = new SphereRigidbody(this, 10, 10, COL_PICKUP, COL_PICKUP_COLLIDER_WIH, info);

	((Rigidbody*)rigidbody)->rigidbody->setGravity(btVector3(0, -10, 0));

	auto fun = [](btVector3 v1, btVector3 v2, GameObject* itself, GameObject* other) 
				{
					WeaponPickUp *pick_up = (WeaponPickUp*)itself;

					if((other->tag).find("Pickup") != std::string::npos)
					{
						pick_up->collided = true;
						itself->scene->removeGameObject(itself);
						delete pick_up;
						return;
					}

					if((other->tag) == std::string("Player") && !pick_up->collided && pick_up->pick_debouncer->run(NULL))
					{
						PlayerCharacter* player = (PlayerCharacter*)other;
						if(!player->in_pinto_form)
						{
							if(player->player_id == NetworkManager::instance()->player_id)
							{
								AudioManager::instance()->playPickWeapon(Ogre::Vector3(v1.x(), v1.y(), v1.z()));
								player->changeWeapon(pick_up->weapon_id);
							} 
						}
						pick_up->collided = true;
						itself->scene->removeGameObject(itself);
						delete pick_up;
						return;
					}
				};

	((Rigidbody*)rigidbody)->onCollision = fun;
}

PlayerPickUp::~PlayerPickUp()
{
	delete pick_debouncer;
}

void PlayerPickUp::update()
{
	mesh->node->yaw(Ogre::Radian(0.01));
	Ogre::Quaternion q = mesh->node->_getDerivedOrientation();
	_transform->rotX = q.x;
	_transform->rotY = q.y;
	_transform->rotZ = q.z;
	_transform->rotW = q.w;
	GameObject::update();
}

void PlayerPickUp::onPicked(PlayerCharacter*)
{
}

