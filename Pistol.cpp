#include "Pistol.h"

// Weapon::Weapon(PlayerCharacter* player_p, std::string mesh_name, int id_p, double reload_time_p, 
//     int shoot_cost_p, int max_mag_cap_p, int max_ammo_p, double cooldown_p, float posX, 
//             float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
//             float scaleX, float scaleY, float scaleZ, PlayerBox* pos) : Component((GameObject*)player)

Pistol::Pistol(PlayerCharacter* player_p, std::string mesh_name, float posX, 
			float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
			float scaleX, float scaleY, float scaleZ, PlayerBox* box) : 
			Weapon(player_p, mesh_name, 0, 3, 1, 6, 30, 0.3, posX, 
			posY, posZ, rotX, rotY, rotZ, rotW, scaleX, scaleY, scaleZ, box)

{
    damage = 10;
    shoot_distance = 100;
}

void Pistol::shoot_hook()
{
    Ogre::Vector3 cam_dir = ((Camera*)(player->controller->fps_camera))->camera->getDirection();
    Ogre::Vector3 cam_pos = ((Camera*)(player->controller->fps_camera))->camera->getPosition();

    Ogre::Vector3 shoot_vector = shoot_pos->node->convertLocalToWorldPosition(shoot_pos->node->getPosition());

    btVector3 from = btVector3(shoot_vector.x, shoot_vector.y, shoot_vector.z);
    btVector3 to = btVector3(cam_pos.x, cam_pos.y, cam_pos.z) + (btVector3(cam_dir.x, cam_dir.y, cam_dir.z) * shoot_distance);

    btCollisionWorld::ClosestRayResultCallback rayCallback(from, to);

    rayCallback.m_collisionFilterGroup = COL_BULLET;
    rayCallback.m_collisionFilterMask = COL_BULLET_COLLIDER_WITH;

    Component::_gameObject->scene->physics_world->rayTest(from, to, rayCallback);

    if(rayCallback.hasHit())
    {
        if(rayCallback.m_collisionObject->getUserPointer() != NULL)
        {
            HitBox* hit_box = (HitBox*)(rayCallback.m_collisionObject->getUserPointer());
            hit_box->takeDamage(damage);
            //blood particle system
        }
        else
        {
            //dust particle system
        }
    }
}