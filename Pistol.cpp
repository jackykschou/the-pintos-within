#include "Pistol.h"

// Weapon::Weapon(PlayerCharacter* player_p, std::string mesh_name, int id_p, double reload_time_p, 
//     int shoot_cost_p, int max_mag_cap_p, int max_ammo_p, double cooldown_p, float posX, 
//             float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
//             float scaleX, float scaleY, float scaleZ, PlayerBox* pos) : Component((GameObject*)player)

Pistol::Pistol(PlayerCharacter* player_p, std::string mesh_name, float posX, 
			float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, PlayerBox* box) : 
			Weapon(player_p, mesh_name, 0, 3, 1, 12, 120, 0.1, posX, 
			posY, posZ, rotX, rotY, rotZ, rotW, scaleX, scaleY, scaleZ, box)

{
    Transform* tran = ((GameObject*)player_p)->getComponent<Transform>();
    node->setPosition(player->mesh->node->convertWorldToLocalPosition(
        Ogre::Vector3(tran->posX + posX, tran->posY + posY, tran->posZ + posZ)));
    damage = 16;
    shoot_distance = 2000;

    reload_speed = 1.0;

    reload_time = reload_animation_state->getLength() * reload_speed;
}

void Pistol::shoot_hook()
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
        
        if(rayCallback.m_collisionObject->getUserPointer() != NULL)
        {
            HitBox* hit_box = (HitBox*)(rayCallback.m_collisionObject->getUserPointer());
            if(hit_box->player->player_id != NetworkManager::instance()->player_id
                && ((GameState::instance()->team_mode != TEAM) || (hit_box->player->team_id != GameState::instance()->player->team_id)))
            {
                int damage_sent = hit_box->getDamage(damage);
                uint32_t enemy_id = hit_box->player->player_id;

                NetworkManager::instance()->vital->setDamage(damage_sent, enemy_id);

                ParticleManager::instance()->EmitBloodSpurt(Ogre::Vector3(point.x(), point.y(), point.z()), -cam_dir);
                NetworkManager::instance()->particle->setBlood(point.x(), point.y() , point.z(), -cam_dir.x, -cam_dir.y, -cam_dir.z);
            }
        }
        else
        {
            AudioManager::instance()->playBulletDirtCollision(Ogre::Vector3(point.x(), point.y(), point.z()));

            ParticleManager::instance()->EmitDust(Ogre::Vector3(point.x(), point.y(), point.z()), -cam_dir);

            NetworkManager::instance()->particle->setDust(point.x(), point.y() , point.z(), -cam_dir.x, -cam_dir.y, -cam_dir.z);
        }
    }
}