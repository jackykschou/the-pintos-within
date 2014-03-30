#include "HitBox.h"

#include "PhysicsManager.h"

HitBox::HitBox(PlayerCharacter* player_p, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ, float damage_multiplier_p): 
	PlayerBox(player_p, mesh_name, posX, posY, posZ, rotX, rotY, rotZ, rotW, scaleY, scaleY, scaleZ,
	 COL_HIT_BOX, COL_HIT_BOX_COLLIDER_WITH)
{
	player = player_p;
	damage_multiplier = damage_multiplier_p;
	// ((Rigidbody*)rigidbody)->rigidbody->setCollisionFlags(((Rigidbody*)rigidbody)->rigidbody->getCollisionFlags() | 
	// 	CollisionObject::CF_KINEMATIC_OBJECT);;
	((Rigidbody*)rigidbody)->rigidbody->setUserPointer(this);
}

int HitBox::getDamage(int amount)
{
	return amount * damage_multiplier;
}
