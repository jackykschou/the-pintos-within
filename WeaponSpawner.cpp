#include "WeaponSpawner.h"
#include "NetworkManager.h"
#include "Debouncer.h"
#include "WeaponPickUp.h"
#include "HairPickUp.h"

WeaponSpawner::WeaponSpawner(std::string tag, Scene* s) : GameObject(tag, s), positions(), positions_with_weapons()
{
	float spawn_rate = 5.0f;

	current_weapon_count = 0;

	spawn_debouncer = new Debouncer(spawn_rate * 1000, 
		[this]()
		{
			if(NetworkManager::instance()->isServer())
				spawnWeapon();
		});
}

WeaponSpawner::~WeaponSpawner()
{
	delete spawn_debouncer;
}

void WeaponSpawner::startGame()
{
}

void WeaponSpawner::addSpawnPoint(Ogre::Vector3 point)
{
	positions.push_back(point);
}

void WeaponSpawner::spawnWeapon(float x, float y, float z, uint32_t id, int pick_up_id, int pick_up_type)
{
	if(pick_up_type == WEAPON_PICKUP)
	{
		if(id == 0)
		{
			new WeaponPickUp("PistolPickup", scene, "Pickup.Pistol.mesh",
			x, y + 20, z, 0, 0, 0, 1,
			10, 10, 10, 0, pick_up_id);
		}
		else if(id == 1)
		{
			new WeaponPickUp("ShotgunPickup", scene, "Pickup.Shotgun.mesh",
			x, y + 20, z, 0, 0, 0, 1,
			10, 10, 10, 1, pick_up_id);
		}
		else if(id == 2)
		{
			new WeaponPickUp("RiflePickup", scene, "Pickup.Rifle.mesh",
			x, y + 20, z, 0, 0, 0, 1,
			10, 10, 10, 2, pick_up_id);
		}
		else
		{
			new WeaponPickUp("BFGPickup", scene, "Pickup.BFG.mesh",
			x, y + 20, z, 0, 0, 0, 1,
			10, 10, 10, 3, pick_up_id);
		}
	}
	else
	{
		if(id == JOE_HAIR_ID)
		{
			new HairPickUp("JoeHairPickup", scene, "Pickup.Hair.Joe.mesh",
			x, y + 20, z, 0, 0, 0, 1,
			1, 1, 1, JOE_HAIR_ID, pick_up_id);
		}
		else if(id == MIKE_HAIR_ID)
		{
			new HairPickUp("MikeHairPickup", scene, "Pickup.Hair.Mikey.mesh",
			x, y + 20, z, 0, 0, 0, 1,
			1, 1, 1, MIKE_HAIR_ID, pick_up_id);
		}
		else if(id == KEEGAN_HAIR_ID)
		{
			new HairPickUp("KeeganHairPickup", scene, "Pickup.Hair.Keegan.mesh",
			x, y + 20, z, 0, 0, 0, 1,
			1, 1, 1, KEEGAN_HAIR_ID, pick_up_id);
		}
		else
		{
			new HairPickUp("JackyHairPickup", scene, "Pickup.Hair.Jacky.mesh",
			x, y + 20, z, 0, 0, 0, 1,
			1, 1, 1, JACKY_HAIR_ID, pick_up_id);
		}
	}
}

void WeaponSpawner::spawnWeapon()
{
	if(positions_with_weapons.size() == positions.size())
		return;

	int position_index;
	do
	{
		position_index = RAND_RANGE(0, positions.size());
	}while(positions_with_weapons.find(position_index) != positions_with_weapons.end());

	Ogre::Vector3 position = positions[position_index];

	positions_with_weapons.insert(position_index);

	uint32_t index = 0;

	float random_0_1 = RAND;

	if(random_0_1 < 0.7)
	{
		float random_0_1_weapon = RAND;
		if(random_0_1_weapon < 0.35f)
		{
			index = 1;
			new WeaponPickUp("ShotgunPickup", scene, "Pickup.Shotgun.mesh",
			position.x, position.y + 20, position.z, 0, 0, 0, 1,
			10, 10, 10, 1, position_index);
		}
		else if(random_0_1_weapon < 0.7f)
		{
			index = 2;
			new WeaponPickUp("RiflePickup", scene, "Pickup.Rifle.mesh",
			position.x, position.y + 20, position.z, 0, 0, 0, 1,
			10, 10, 10, 2, position_index);
		}
		else
		{
			index = 3;
			new WeaponPickUp("BFGPickup", scene, "Pickup.BFG.mesh",
			position.x, position.y + 20, position.z, 0, 0, 0, 1,
			10, 10, 10, 3, position_index);
		}
		NetworkManager::instance()->vital->setSpawnWeapon(index, position.x, position.y, position.z, position_index, WEAPON_PICKUP);
	}
	else
	{
		float random_0_1_hair = RAND;
		if(random_0_1_hair < 0.25f)
		{
			index = JOE_HAIR_ID;
			new HairPickUp("JoeHairPickup", scene, "Pickup.Hair.Joe.mesh",
			position.x, position.y + 20, position.z, 0, 0, 0, 1,
			1, 1, 1, JOE_HAIR_ID, position_index);
		}
		else if(random_0_1_hair < 0.5f)
		{
			index = MIKE_HAIR_ID;
			new HairPickUp("MikeHairPickup", scene, "Pickup.Hair.Mikey.mesh",
			position.x, position.y + 20, position.z, 0, 0, 0, 1,
			1, 1, 1, MIKE_HAIR_ID, position_index);
		}
		else if(random_0_1_hair < 0.75f)
		{
			index = KEEGAN_HAIR_ID;
			new HairPickUp("KeeganHairPickup", scene, "Pickup.Hair.Keegan.mesh",
			position.x, position.y + 20, position.z, 0, 0, 0, 1,
			1, 1, 1, KEEGAN_HAIR_ID, position_index);
		}
		else
		{
			index = JACKY_HAIR_ID;
			new HairPickUp("JackyHairPickup", scene, "Pickup.Hair.Jacky.mesh",
			position.x, position.y + 20, position.z, 0, 0, 0, 1,
			1, 1, 1, JACKY_HAIR_ID, position_index);
		}
		NetworkManager::instance()->vital->setSpawnWeapon(index, position.x, position.y, position.z, position_index, HAIR_PICKUP);
	}
}

void WeaponSpawner::update()
{
	GameObject::update();
	if(positions.size() != 0)
	{
		spawn_debouncer->run();
	}
}



