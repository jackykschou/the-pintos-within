#include "Blaster.h"

// Weapon::Weapon(PlayerCharacter* player_p, std::string mesh_name, int id_p, double reload_time_p, 
//     int shoot_cost_p, int max_mag_cap_p, int max_ammo_p, double cooldown_p, float posX, 
//             float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
//             float scaleX, float scaleY, float scaleZ, PlayerBox* pos) : Component((GameObject*)player)

Blaster::Blaster(PlayerCharacter* player_p, std::string mesh_name, float posX, 
			float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, PlayerBox* box) : 
			Weapon(player_p, mesh_name, 3, 3, 1, 100, 500, 0.1, posX, 
			posY, posZ, rotX, rotY, rotZ, rotW, scaleX, scaleY, scaleZ, box)

{
    Transform* tran = ((GameObject*)player_p)->getComponent<Transform>();
    node->setPosition(player->mesh->node->convertWorldToLocalPosition(
        Ogre::Vector3(tran->posX + posX, tran->posY + posY, tran->posZ + posZ)));

    damage = 100;
    shoot_distance = 2000;

    is_charging = false;
    charge_scale = 0;
    charge_rate = 0.004f;
    blast_radius = 300;
    max_ammot_cost = 25;

    charge_sound_debouncer = new Debouncer(0.3 * 1000, [this]()
    {
        AudioManager::instance()->playBlasterCharge(Ogre::Vector3(player->transform->posX, 
                                                    player->transform->posY, player->transform->posZ), (uint8_t)(charge_scale * 128));
    });

    reload_speed = 1.2f;

    reload_time = reload_animation_state->getLength() * reload_speed;
}

Blaster::~Blaster()
{
    delete charge_sound_debouncer;
}

void Blaster::shoot()
{
    Ogre::Vector3 curPos = Ogre::Vector3(GameState::instance()->player->tr->posX, GameState::instance()->player->tr->posY, GameState::instance()->player->tr->posZ);
    if(!is_reloading && !player->is_dead && (player->current_weapon == this))
    {
        if(InputManager::instance()->isMouseLeftClicked() && !is_charging && (current_mag_count > 0) && (shoot_timer <= 0))
        {
            is_charging = true;
            charge_scale = 0;
        }
        else if(InputManager::instance()->isMouseLeftDown() && is_charging)
        {
            if((current_mag_count > (int)(max_ammot_cost * charge_scale)) && (charge_scale < 1))
            {
                charge_scale += charge_rate;
                if(charge_scale > 1)
                    charge_scale = 1;
            }

            charge_sound_debouncer->run();
        }
        else if(!InputManager::instance()->isMouseLeftDown() && is_charging)
        {
            shoot_timer = cooldown;
            shooting_animation_state->setTimePosition(0);

            is_shooting = true;
            is_charging = false;

            current_mag_count -= (int)(max_ammot_cost * charge_scale);

            AudioManager::instance()->playWeaponFire(curPos, weapon_id, (uint8_t)(128 * charge_scale));
            shoot_hook();
        }
        else if(InputManager::instance()->isMouseLeftClicked() && current_ammo == 0 && current_mag_count == 0)
        {
            AudioManager::instance()->playOutOfAmmo(curPos);
        }
    }
}

void Blaster::shoot_hook()
{
    Ogre::Vector3 cam_dir = ((Camera*)(player->controller->fps_camera))->camera->getDirection();
    Ogre::Vector3 cam_pos = ((Camera*)(player->controller->fps_camera))->camera->getPosition();

    Ogre::Vector3 shoot_vector = shoot_pos->node->convertLocalToWorldPosition(shoot_pos->node->getPosition());

    btVector3 from = btVector3(cam_pos.x, cam_pos.y, cam_pos.z) + (btVector3(cam_dir.x, cam_dir.y, cam_dir.z) * shoot_from_offset);
    btVector3 to = btVector3(cam_pos.x, cam_pos.y, cam_pos.z) + (btVector3(cam_dir.x, cam_dir.y, cam_dir.z) * shoot_distance);

    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);

    rayCallback.m_collisionFilterGroup = COL_BULLET;
    rayCallback.m_collisionFilterMask = COL_BULLET_COLLIDER_WITH;

    Component::_gameObject->scene->physics_world->rayTest(from, to, rayCallback);
    if(rayCallback.hasHit())
    {
        btVector3 point = rayCallback.m_hitPointWorld;
        float radius = blast_radius * charge_scale;
        int damage_sent = damage * charge_scale;

        ParticleManager::instance()->EmitRocketExplosion(Ogre::Vector3(point.x(), point.y(), point.z()));
        NetworkManager::instance()->particle->setBlasterExplosion(point.x(), point.y(), point.z(), radius);

        for(int i = 0; i < GameState::instance()->num_player; ++i)
        {
            if(GameState::instance()->players[i] != NULL
                && ((GameState::instance()->team_mode != TEAM) || (GameState::instance()->players[i]->team_id != GameState::instance()->player->team_id)
                    || (GameState::instance()->game_mode == PINTO)))
            {
                Transform* tran = GameState::instance()->players[i]->transform;
                btVector3 tran_vector = btVector3(tran->posX, tran->posY, tran->posZ);
                if(tran_vector.distance(point) <= radius)
                {
                    uint32_t enemy_id = GameState::instance()->players[i]->player_id;
                    if(GameState::instance()->players[i]->in_pinto_form)
                        damage_sent *= 0.15f;
                    NetworkManager::instance()->vital->setDamage(damage_sent, enemy_id);
                }
            }
        }
    }
}

void Blaster::switchToThisWeapon()
{
    Weapon::switchToThisWeapon();
    is_charging = false;
    charge_scale = 0;
}