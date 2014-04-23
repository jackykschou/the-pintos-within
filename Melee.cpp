#include "Melee.h"

// Weapon::Weapon(PlayerCharacter* player_p, std::string mesh_name, int id_p, double reload_time_p, 
//     int shoot_cost_p, int max_mag_cap_p, int max_ammo_p, double cooldown_p, float posX, 
//             float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
//             float scaleX, float scaleY, float scaleZ, PlayerBox* pos) : Component((GameObject*)player)

Melee::Melee(PlayerCharacter* player_p, std::string mesh_name, float posX, 
			float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, PlayerBox* box) : 
			Weapon(player_p, mesh_name, MELEE_ID, 3, 0, 1, 1, 0.3, posX, 
			posY, posZ, rotX, rotY, rotZ, rotW, scaleX, scaleY, scaleZ, box)

{
    Transform* tran = ((GameObject*)player_p)->getComponent<Transform>();
    node->setPosition(player->mesh->node->convertWorldToLocalPosition(
        Ogre::Vector3(tran->posX + posX, tran->posY + posY, tran->posZ + posZ)));
    damage = 60;
    damage_radius = 50;
}

void Melee::shoot_hook()
{
    Ogre::Vector3 cam_dir = ((Camera*)(player->controller->fps_camera))->camera->getDirection();
    Ogre::Vector3 cam_pos = ((Camera*)(player->controller->fps_camera))->camera->getPosition();

    Ogre::Vector3 shoot_vector = shoot_pos->node->convertLocalToWorldPosition(shoot_pos->node->getPosition());

    btVector3 from = btVector3(cam_pos.x, cam_pos.y, cam_pos.z) + (btVector3(cam_dir.x, cam_dir.y, cam_dir.z) * 75);
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

        // ParticleManager::instance()->EmitBloodSpurt(Ogre::Vector3(point.x(), point.y(), point.z()), -cam_dir);
        // NetworkManager::instance()->particle->setBlood(point.x(), point.y() , point.z(), -cam_dir.x, -cam_dir.y, -cam_dir.z);
        // NetworkManager::instance()->sendParticle();

        for(int i = 0; i < GameState::instance()->num_player; ++i)
        {
            if(GameState::instance()->players[i] != NULL && GameState::instance()->players[i] != GameState::instance()->player)
            {
                Transform* tran = GameState::instance()->players[i]->transform;
                btVector3 tran_vector = btVector3(tran->posX, tran->posY, tran->posZ);
                if(tran_vector.distance(point) <= radius)
                {
                    uint32_t enemy_id = GameState::instance()->players[i]->player_id;
                    NetworkManager::instance()->vital->setDamage(damage_sent, enemy_id);
                }
            }
        }
    }
}