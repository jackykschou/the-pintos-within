#include "PlayerPickUp.h"
#include "WeaponPickUp.h"
#include "PlayerCharacter.h"
#include "Mesh.h"
#include "SphereRigidbody.h"
#include "GameState.h"
#include "AudioManager.h"
#include "Debouncer.h"
#include "WeaponSpawner.h"

PlayerPickUp::PlayerPickUp(std::string tag, Scene* scene, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ, int id) : GameObject(tag, scene)
{
	pick_debouncer = new Debouncer(1000*1000);
	pick_up_id = id;

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

	pick_radius = 18;

	btVector3 inertia(0, 0, 0);
	float mass = 0;
	float biggest_scale = std::max(std::max(scaleX, scaleY), scaleZ);
	btCollisionShape* collisionShape = new btSphereShape(radius);
	btRigidBody::btRigidBodyConstructionInfo* info = new btRigidBody::btRigidBodyConstructionInfo(mass ,NULL,collisionShape,inertia);

	collided = false;

	rigidbody = new SphereRigidbody(this, 10, 10, COL_PICKUP, COL_PICKUP_COLLIDER_WIH, info);
}

PlayerPickUp::~PlayerPickUp()
{
	GameState::instance()->weapon_spawner->positions_with_weapons.erase(pick_up_id);
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

	for(int i = 0; i < GameState::instance()->num_player; ++i)
    {
            if(GameState::instance()->players[i] != NULL && !collided)
            {
                Transform* tran = GameState::instance()->players[i]->transform;
                btVector3 tran_vector = btVector3(tran->posX, tran->posY, tran->posZ);
                btVector3 point = ((Rigidbody*)rigidbody)->rigidbody->getCenterOfMassTransform().getOrigin();
                if(tran_vector.distance(point) <= pick_radius && pick_debouncer->run(NULL))
                {
                    if(!GameState::instance()->players[i]->in_pinto_form)
					{
						if(GameState::instance()->players[i]->player_id == NetworkManager::instance()->player_id)
						{
							onPicked(GameState::instance()->players[i]);
						} 
					}
					collided = true;
					scene->removeGameObject(this);
					delete this;
					return;
                }
            }
     }
}

void PlayerPickUp::onPicked(PlayerCharacter*)
{
}

