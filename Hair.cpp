#include "Hair.h"

Hair::Hair(PlayerCharacter* player_p, std::string mesh_name, int id_p, float posX, 
			float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ) : Component((GameObject*)player_p)
{
	_transform = ((GameObject*)player_p)->getComponent<Transform>();

	player = player_p;

	hair_id = id_p;

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

	jumping_animation_state = entity->getAnimationState("Jump");
	jumping_animation_state->setLoop(false);
	jumping_animation_state->setEnabled(false);
	jumping_animation_state->setWeight(0);

	die_animation_state = entity->getAnimationState("Death");
	die_animation_state->setLoop(false);
	die_animation_state->setEnabled(false);
	die_animation_state->setWeight(0);

	shooting_animation_state->setTimePosition(0);
	reload_animation_state->setTimePosition(0);
}

Hair::~Hair()
{
 delete node;
 delete entity;
}

void Hair::update()
{
	Component::update();
}