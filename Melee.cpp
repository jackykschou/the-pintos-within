#include "Melee.h"

// Weapon::Weapon(PlayerCharacter* player_p, std::string mesh_name, int id_p, double reload_time_p, 
//     int shoot_cost_p, int max_mag_cap_p, int max_ammo_p, double cooldown_p, float posX, 
//             float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
//             float scaleX, float scaleY, float scaleZ, PlayerBox* pos) : Component((GameObject*)player)

Melee::Melee(PlayerCharacter* player_p, std::string mesh_name, float posX, 
			float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, PlayerBox* box) : Weapon(player_p)
{
    reload_speed = 1.0f;

    _transform = ((GameObject*)player_p)->getComponent<Transform>();

    player = player_p;
    shoot_pos = box;

    max_mag_cap = 1;
    max_ammo = 1;

    current_mag_count = 1;
    current_ammo = 1;

    weapon_id = 4;
    shoot_cost = 0;
    reload_time = 1;
    cooldown = 0.3;
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

    shooting_animation_state = entity->getAnimationState("PintoStab");
    shooting_animation_state->setLoop(false);
    shooting_animation_state->setEnabled(false);
    shooting_animation_state->setWeight(0);

    running_animation_state = entity->getAnimationState("PintoRun");
    running_animation_state->setLoop(true);
    running_animation_state->setEnabled(false);
    running_animation_state->setWeight(0);

    idle_animation_state = entity->getAnimationState("PintoIdle");
    idle_animation_state->setLoop(true);
    idle_animation_state->setEnabled(false);
    idle_animation_state->setWeight(0);

    reload_animation_state = NULL;

    jumping_animation_state = entity->getAnimationState("PintoJump");
    jumping_animation_state->setLoop(false);
    jumping_animation_state->setEnabled(false);
    jumping_animation_state->setWeight(0);

    die_animation_state = entity->getAnimationState("PintoDeath");
    die_animation_state->setLoop(false);
    die_animation_state->setEnabled(false);
    die_animation_state->setWeight(0);


    Transform* tran = ((GameObject*)player_p)->getComponent<Transform>();
    node->setPosition(player->mesh->node->convertWorldToLocalPosition(
        Ogre::Vector3(tran->posX + posX, tran->posY + posY, tran->posZ + posZ)));
    
    damage = 20;
    damage_radius = 150;

    shooting_animation_state->setTimePosition(0);

    reload_speed = 1.0;
    reload_time = 1.0;
}

void Melee::shoot_hook()
{
    btVector3 point = btVector3(node->_getDerivedPosition().x, node->_getDerivedPosition().y, node->_getDerivedPosition().z);

    for(int i = 0; i < GameState::instance()->num_player; ++i)
    {
        if(GameState::instance()->players[i] != NULL && GameState::instance()->players[i] != GameState::instance()->player
            && ((GameState::instance()->team_mode != TEAM) || (GameState::instance()->players[i]->team_id != GameState::instance()->player->team_id)))
        {
            Transform* tran = GameState::instance()->players[i]->transform;
            btVector3 tran_vector = btVector3(tran->posX, tran->posY, tran->posZ);
            if(tran_vector.distance(point) <= damage_radius)
            {
                uint32_t enemy_id = GameState::instance()->players[i]->player_id;
                NetworkManager::instance()->vital->setDamage(damage, enemy_id);
            }
        }
    }
}
