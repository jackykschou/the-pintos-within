#include "HitBox.h"

#include "PhysicsManager.h"

HitBox::HitBox(PlayerCharacter* player_p, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ, float damage_multiplier_p): 
	PlayerBox(player_p, mesh_name, posX, posY, posZ, rotX, rotY, rotZ, rotW, scaleX, scaleY, scaleZ,
	 COL_HIT_BOX, COL_HIT_BOX_COLLIDER_WITH)
{
	player = player_p;
	damage_multiplier = damage_multiplier_p;
	((Rigidbody*)rigidbody)->rigidbody->setUserPointer(this);
	enabled = true;
}

void HitBox::Enable()
{
	enabled = true;
	((Rigidbody*)rigidbody)->dynamics_world->addRigidBody(((Rigidbody*)rigidbody)->rigidbody, COL_HIT_BOX, COL_HIT_BOX_COLLIDER_WITH);
}
void HitBox::Disable()
{
	enabled = false;
	((Rigidbody*)rigidbody)->dynamics_world->removeRigidBody(((Rigidbody*)rigidbody)->rigidbody);
}

int HitBox::getDamage(int amount)
{
	if(enabled)
		return amount * damage_multiplier;
	else
		return 0;
}
