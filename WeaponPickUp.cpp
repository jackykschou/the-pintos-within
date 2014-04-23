#include "WeaponPickUp.h"
#include "PlayerCharacter.h"

// PlayerPickUp::PlayerPickUp(std::string tag, Scene* scene, std::string mesh_name,
// 	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
// 	float scaleX, float scaleY, float scaleZ, uint32_t id) 

WeaponPickUp::WeaponPickUp(std::string tag, Scene* scene, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ, uint32_t weapon_id_p) : 
	PlayerPickUp (tag, scene, mesh_name,
	posX, posY, posZ, rotX, rotY, rotZ, rotW,
	scaleX, scaleY, scaleZ) 
{
	weapon_id = weapon_id_p;
}

void WeaponPickUp::onPicked(PlayerCharacter* player)
{
	LOG("LAAAAAAAAAAAAAAAAAAAAAAAAA");
	player->changeWeapon(weapon_id);
}

