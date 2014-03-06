#include "PlayerCharacter.h"

PlayerCharacter::PlayerCharacter(std::string tag, Scene* scene, 
	int mask, int col_mask, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ) : GameObject(tag, scene)
{
	Transform* tran = this->getComponent<Transform>();

	tran->posX = posX;
	tran->posY = posY;
	tran->posZ = posZ;

	tran->rotX = rotX;
	tran->rotY = rotY;
	tran->rotZ = rotZ;
	tran->rotW = rotW;

	tran->scaleX = scaleX;
	tran->scaleY = scaleY;
	tran->scaleZ = scaleZ;

	mesh = new Mesh(this, mesh_name);

	Ogre::Vector3 box_half_size = mesh->entity->getBoundingBox().getHalfSize();

	animation_state = mesh->entity->getAnimationState("Shooting");
	animation_state->setWeight(1);
    animation_state->setLoop(true);
    animation_state->setEnabled(true);


	controller = new FPSBoxController(this, "Camm", box_half_size.y * scaleY, btVector3(box_half_size.x * scaleX, box_half_size.y * scaleY, box_half_size.z * scaleZ), 
		3, mask, col_mask);
}

PlayerCharacter:: ~PlayerCharacter()
{
}

void PlayerCharacter::update()
{
	GameObject::update();

	// animation_state = mesh->entity->getAnimationState("Running");
	// animation_state->setWeight(1);
    // animation_state->setLoop(true);
    // animation_state->setEnabled(true);

	LOG("Name: : " << animation_state->getAnimationName() << " Enabled: " << animation_state->getEnabled() << " Weight: "
		<< animation_state->getWeight());
	LOG("Length: : " << animation_state->getLength());
	LOG("Current position: " << animation_state->getTimePosition());

    // animation_state->addTime(1);
	animation_state->addTime(0.0001f);
}