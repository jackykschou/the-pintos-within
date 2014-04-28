#include "WeaponSpawner.h"
#include "NetworkManager.h"
#include "Debouncer.h"
#include "WeaponPickUp.h"

WeaponSpawner::WeaponSpawner(std::string tag, Scene* s) : GameObject(tag, s), positions()
{
	float spawn_rate = 3.0f;

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

void WeaponSpawner::spawnWeapon(float x, float y, float z, uint32_t id)
{
	if(id == 0)
	{
		new WeaponPickUp("PistolPickup", scene, "Pickup.Pistol.mesh",
		x, y + 20, z, 0, 0, 0, 1,
		10, 10, 10, 0);
	}
	else if(id == 1)
	{
		new WeaponPickUp("ShotgunPickup", scene, "Pickup.Shotgun.mesh",
		x, y + 20, z, 0, 0, 0, 1,
		10, 10, 10, 1);
	}
	else if(id == 2)
	{
		new WeaponPickUp("RiflePickup", scene, "Pickup.Rifle.mesh",
		x, y + 20, z, 0, 0, 0, 1,
		10, 10, 10, 2);
	}
	else
	{
		new WeaponPickUp("BFGPickup", scene, "Pickup.BFG.mesh",
		x, y + 20, z, 0, 0, 0, 1,
		10, 10, 10, 3);
	}
}

Ogre::Vector3 WeaponSpawner::spawnWeapon()
{
	Ogre::Vector3 position = positions[RAND_RANGE(0, positions.size())];
	
	bool self = false;

	float random_0_1 = RAND;
	uint32_t index = 0;

	if(random_0_1 < 0.35f)
	{
		index = 1;
		new WeaponPickUp("ShotgunPickup", scene, "Pickup.Shotgun.mesh",
		position.x, position.y + 20, position.z, 0, 0, 0, 1,
		10, 10, 10, 1);
	}
	else if(random_0_1 < 0.7f)
	{
		index = 2;
		new WeaponPickUp("RiflePickup", scene, "Pickup.Rifle.mesh",
		position.x, position.y + 20, position.z, 0, 0, 0, 1,
		10, 10, 10, 2);
	}
	else
	{
		index = 3;
		new WeaponPickUp("BFGPickup", scene, "Pickup.BFG.mesh",
		position.x, position.y + 20, position.z, 0, 0, 0, 1,
		10, 10, 10, 3);
	}

	NetworkManager::instance()->vital->setSpawnWeapon(index, position.x, position.y, position.z);

	return position;
}

void WeaponSpawner::update()
{
	GameObject::update();
	if(positions.size() != 0)
	{
		spawn_debouncer->run();
	}
}



