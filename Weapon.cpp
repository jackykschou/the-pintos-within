#include "Weapon.h"

Weapon::Weapon(PlayerCharacter* player_p, std::string mesh_name, int id_p, double reload_time_p, 
	int shoot_cost_p, int max_mag_cap_p, int max_ammo_p, double cooldown_p, float posX, 
			float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, PlayerBox* pos) : Component((GameObject*)player_p)
{
	_transform = ((GameObject*)player_p)->getComponent<Transform>();

	player = player_p;
	shoot_pos = pos;

	max_mag_cap = max_mag_cap_p;
	max_ammo = max_ammo_p;

	current_mag_count = max_mag_cap;
	current_ammo = max_ammo;

	weapon_id = id_p;
	shoot_cost = shoot_cost_p;
	reload_time = reload_time_p;
	cooldown = cooldown_p;
	reload_timer = 0;
	shoot_timer = 0;

	is_reloading = false;
	is_shooting = false;

	std::ostringstream id_stream;
    id_stream << _gameObject->id;

	entity = ((GameObject*)player_p)->scene->manager->createEntity(mesh_name + std::string(" Entity") + id_stream.str()
																			, mesh_name.c_str());
	entity->getMesh()->_setBounds(Ogre::AxisAlignedBox(-50,-50,-50,50,50,50)); //does change bounding box size, but want to do this dynamically
    // entity->getMesh()->_setBoundingSphereRadius(10.0f);  // set inflation amount

	// entity->getMesh()->_setBounds(Ogre::AxisAlignedBox ( Ogre::AxisAlignedBox::Extent::EXTENT_INFINITE));

	node = player->mesh->node->createChildSceneNode(mesh_name + std::string(" Node") + id_stream.str(), 
                                                  player->mesh->node->convertWorldToLocalPosition(Ogre::Vector3(_transform->posX + posX, _transform->posX + posY, _transform->posX + posZ)));
	node->attachObject(entity);
	entity->setCastShadows(true);

	node->setPosition(player->mesh->node->convertWorldToLocalPosition(Ogre::Vector3(_transform->posX + posX, _transform->posY + posY, _transform->posZ + posZ)));
	node->setOrientation(node->convertWorldToLocalOrientation(Ogre::Quaternion(rotW, rotX, rotY, rotZ)));
	node->setInheritScale (false);
	node->setScale (scaleX, scaleY, scaleZ);

	shooting_animation_state = entity->getAnimationState("Shooting");
	shooting_animation_state->setLoop(false);
	shooting_animation_state->setEnabled(false);
	shooting_animation_state->setWeight(0);

	running_animation_state = entity->getAnimationState("Running");
	running_animation_state->setLoop(true);
	running_animation_state->setEnabled(false);
	running_animation_state->setWeight(0);

	idle_animation_state = entity->getAnimationState("Idle");
	idle_animation_state->setLoop(true);
	idle_animation_state->setEnabled(false);
	idle_animation_state->setWeight(0);

	reload_animation_state = entity->getAnimationState("Reloading");
	reload_animation_state->setLoop(false);
	reload_animation_state->setEnabled(false);
	reload_animation_state->setWeight(0);

	reload_time = reload_animation_state->getLength();

	jumping_animation_state = entity->getAnimationState("Jump");
	jumping_animation_state->setLoop(false);
	jumping_animation_state->setEnabled(false);
	jumping_animation_state->setWeight(0);

	die_animation_state = entity->getAnimationState("Death");
	die_animation_state->setLoop(false);
	die_animation_state->setEnabled(false);
	die_animation_state->setWeight(0);
}

Weapon::~Weapon()
{
 delete node;
 delete entity;
}

void Weapon::shoot()
{
	// LOG("Can I shoot? " << ((player->current_weapon == this) && current_mag_count >= shoot_cost && !is_reloading && !player->is_dead));
	LOG("current_mag_count: " << current_mag_count << " / " << current_ammo);
	if((player->current_weapon == this) && current_mag_count >= shoot_cost && !is_reloading && !player->is_dead && (shoot_timer <= 0))
	{
		shoot_timer = cooldown;
		shooting_animation_state->setTimePosition(0);
		is_shooting = true;
		current_mag_count -= shoot_cost;
		shoot_hook();
	}
}

void Weapon::reload()
{
	if((player->current_weapon == this) && !is_reloading && (current_mag_count != max_mag_cap) && 
		(current_ammo != 0) && !player->is_dead && !is_shooting)
	{
		LOG("Reloading...");
		AudioManager::instance()->playReload(Ogre::Vector3(player->transform->posX, player->transform->posY, player->transform->posZ));
		reload_timer = 0;
		reload_animation_state->setTimePosition(0);
		is_reloading = true;
	}
}

void Weapon::update()
{
	Component::update();

	if(shoot_timer >= 0)
		shoot_timer -= GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame;

	if(player->current_weapon == this)
	{
		if(is_reloading)
		{
			reload_timer += GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame;
			if(reload_timer >= reload_time) 
			{
				LOG("Finish Reloading...");
				is_reloading = false;
				current_ammo = (current_ammo >= max_mag_cap) ? (current_ammo - (max_mag_cap - current_mag_count)) : 0;
				current_mag_count = (current_ammo >= max_mag_cap) ? max_mag_cap : current_ammo;
			}
		}
	}
}

void Weapon::switchToThisWeapon()
{
	is_shooting = false;
	is_reloading = false;
	shooting_animation_state->setTimePosition(0);
	reload_animation_state->setTimePosition(0);
	current_mag_count = max_mag_cap;
	current_ammo = max_ammo;

	player->current_weapon = this;
}

