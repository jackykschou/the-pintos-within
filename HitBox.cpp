#include "HitBox.h"

HitBox::HitBox(PlayerCharacter* player_p, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ, float damage_multiplier_p): 
	PlayerBox(player_p, mesh_name, posX, posY, posZ, rotX, rotY, rotZ, rotW, scaleY, scaleY, scaleZ,
	 COL_HIT_BOX, COL_HIT_BOX_COLLIDER_WITH)
{
	damage_multiplier = damage_multiplier_p;
	((Rigidbody*)rigidbody)->rigidbody->setUserPointer(this);
}

void HitBox::takeDamage(int amount)
{
	player->health -= (int)(amount * damage_multiplier);
}
