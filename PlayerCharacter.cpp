#include "PlayerCharacter.h"

#include "Pistol.h"
#include "ShotGun.h"
#include "AssaultRifle.h"
#include "Blaster.h"
#include "NetworkManager.h"
#include "PlayerSpawner.h"

PlayerCharacter::PlayerCharacter(bool is_yourself_p, Scene* scene, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ, uint32_t player_id_p) : GameObject("Player", scene)
{
	player_id = player_id_p;

	is_yourself = is_yourself_p;
	is_shooting = false;
	is_idle = false;
	is_dead = false;

	run_animation_time = 0.0;
	shoot_animation_time = 0.0;
	idle_animation_time = 0.0;

	max_health = 100;
	health = max_health;
	health_regen = 1.0;
	health_regen_rate = 1.0;

	Transform* tran = this->getComponent<Transform>();
	tr = tran;

	transform = this->getComponent<Transform>();
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

	shooting_animation_states[PISTOL_ID] = mesh->entity->getAnimationState("Shooting");
	shooting_animation_states[PISTOL_ID]->setLoop(false);
	shooting_animation_states[PISTOL_ID]->setWeight(0);

	reload_animation_states[PISTOL_ID] = mesh->entity->getAnimationState("Reloading");
	reload_animation_states[PISTOL_ID]->setLoop(false);
	reload_animation_states[PISTOL_ID]->setWeight(0);

	shooting_animation_states[1] = mesh->entity->getAnimationState("Shooting");
	shooting_animation_states[1]->setLoop(false);
	shooting_animation_states[1]->setWeight(0);

	reload_animation_states[1] = mesh->entity->getAnimationState("Reloading");
	reload_animation_states[1]->setLoop(false);
	reload_animation_states[1]->setWeight(0);

	shooting_animation_states[2] = mesh->entity->getAnimationState("Shooting");
	shooting_animation_states[2]->setLoop(false);
	shooting_animation_states[2]->setWeight(0);

	reload_animation_states[2] = mesh->entity->getAnimationState("Reloading");
	reload_animation_states[2]->setLoop(false);
	reload_animation_states[2]->setWeight(0);

	shooting_animation_states[3] = mesh->entity->getAnimationState("Shooting");
	shooting_animation_states[3]->setLoop(false);
	shooting_animation_states[3]->setWeight(0);

	reload_animation_states[3] = mesh->entity->getAnimationState("Reloading");
	reload_animation_states[3]->setLoop(false);
	reload_animation_states[3]->setWeight(0);

	running_animation_state = mesh->entity->getAnimationState("Running");
	running_animation_state->setLoop(true);
	running_animation_state->setWeight(0);

	idle_animation_state = mesh->entity->getAnimationState("Idle");
	idle_animation_state->setLoop(true);
	idle_animation_state->setWeight(0);

	jumping_animation_state = mesh->entity->getAnimationState("Jump");
	jumping_animation_state->setLoop(false);
	jumping_animation_state->setWeight(0);

	die_animation_state = mesh->entity->getAnimationState("Death");
	die_animation_state->setLoop(false);
	die_animation_state->setWeight(0);

    health_regen_debouncer = new Debouncer(health_regen_rate * 1000);

    walk_sound_debouncer = new Debouncer(0.2 * 1000, [this]()
    {
    	AudioManager::instance()->playWalkStep(Ogre::Vector3(transform->posX, transform->posY, transform->posZ));
    });

    jet_pack_particle_debouncer = new Debouncer(0.1 * 1000, [this](){
    	Ogre::Vector3 pos1 = jet_pack_shoot_pos1->node->
    						convertLocalToWorldPosition(jet_pack_shoot_pos1->node->getPosition());
    	ParticleManager::instance()->EmitSparks(pos1, Ogre::Vector3::NEGATIVE_UNIT_Y);

    	Ogre::Vector3 pos2 = jet_pack_shoot_pos2->node->
    						convertLocalToWorldPosition(jet_pack_shoot_pos2->node->getPosition());
    	ParticleManager::instance()->EmitSparks(pos2, Ogre::Vector3::NEGATIVE_UNIT_Y);
    });

    jet_pack_sound_debouncer = new Debouncer(0.1 * 1000, [this](){
    	AudioManager::instance()->playJetPack(Ogre::Vector3(transform->posX, transform->posY, transform->posZ));
    });

	controller = new FPSBoxController(is_yourself, this, std::string("Camm"), box_half_size.y * scaleY + 1, btVector3(box_half_size.x * scaleX, box_half_size.y * scaleY, box_half_size.z * scaleZ), 
										3, COL_CHARACTER, CHARACTER_COLLIDER_WITH, mesh->node);
	if(is_yourself)
  	{
		scene->main_camera = (Camera*)(controller->fps_camera);
	}

	//hard code position
	Weapon* pistol = new Pistol(this, "Pistol.mesh", 
			0, 0, 0, 
			0, 0, 0, 1,
			10, 10, 10, 
			new PlayerBox(this, "sphere.mesh",
			0, 0, 0, 
			0, 0, 0, 1,
			0.049863, 0.049863, 0.049863, COL_NOTHING, COL_NOTHING));

	weapons[1] = new ShotGun(this, "Shotgun.mesh", 
			0, 0, 0, 
			0, 0, 0, 1,
			10, 10, 10, 
			new PlayerBox(this, "sphere2.mesh",
			0, 0, 0, 
			0, 0, 0, 1,
			0.049863, 0.049863, 0.049863, COL_NOTHING, COL_NOTHING));

	weapons[1]->node->setVisible(false);

	weapons[2] = new AssaultRifle(this, "Rifle.mesh", 
			0, 0, 0, 
			0, 0, 0, 1,
			10, 10, 10, 
			new PlayerBox(this, "sphere3.mesh",
			0, 0, 0, 
			0, 0, 0, 1,
			0.049863, 0.049863, 0.049863, COL_NOTHING, COL_NOTHING));

	weapons[2]->node->setVisible(false);

	weapons[3] = new Blaster(this, "BFG.mesh", 
			0, 0, 0, 
			0, 0, 0, 1,
			10, 10, 10, 
			new PlayerBox(this, "sphere4.mesh",
			0, 0, 0, 
			0, 0, 0, 1,
			0.049863, 0.049863, 0.049863, COL_NOTHING, COL_NOTHING));

	weapons[3]->node->setVisible(false);

	PlayerBox* jet_pack = new PlayerBox(this, "Jetpack.mesh",
									0, 0, 0,
										0, 0, 0, 1,
										1, 1, 1,
										COL_NOTHING, COL_NOTHING);

	//Jet Pack states

	jet_pack_shooting_animation_states[PISTOL_ID] = jet_pack->entity->getAnimationState("Shooting");
	jet_pack_shooting_animation_states[PISTOL_ID]->setLoop(false);
	jet_pack_shooting_animation_states[PISTOL_ID]->setWeight(0);

	jet_pack_reload_animation_states[PISTOL_ID] = jet_pack->entity->getAnimationState("Reloading");
	jet_pack_reload_animation_states[PISTOL_ID]->setLoop(false);
	jet_pack_reload_animation_states[PISTOL_ID]->setWeight(0);

	jet_pack_shooting_animation_states[1] = jet_pack->entity->getAnimationState("Shooting");
	jet_pack_shooting_animation_states[1]->setLoop(false);
	jet_pack_shooting_animation_states[1]->setWeight(0);

	jet_pack_reload_animation_states[1] = jet_pack->entity->getAnimationState("Reloading");
	jet_pack_reload_animation_states[1]->setLoop(false);
	jet_pack_reload_animation_states[1]->setWeight(0);

	jet_pack_shooting_animation_states[2] = jet_pack->entity->getAnimationState("Shooting");
	jet_pack_shooting_animation_states[2]->setLoop(false);
	jet_pack_shooting_animation_states[2]->setWeight(0);

	jet_pack_reload_animation_states[2] = jet_pack->entity->getAnimationState("Reloading");
	jet_pack_reload_animation_states[2]->setLoop(false);
	jet_pack_reload_animation_states[2]->setWeight(0);

	jet_pack_shooting_animation_states[3] = jet_pack->entity->getAnimationState("Shooting");
	jet_pack_shooting_animation_states[3]->setLoop(false);
	jet_pack_shooting_animation_states[3]->setWeight(0);

	jet_pack_reload_animation_states[3] = jet_pack->entity->getAnimationState("Reloading");
	jet_pack_reload_animation_states[3]->setLoop(false);
	jet_pack_reload_animation_states[3]->setWeight(0);

	jet_pack_running_animation_state = jet_pack->entity->getAnimationState("Running");
	jet_pack_running_animation_state->setLoop(true);
	jet_pack_running_animation_state->setWeight(0);

	jet_pack_idle_animation_state = jet_pack->entity->getAnimationState("Idle");
	jet_pack_idle_animation_state->setLoop(true);
	jet_pack_idle_animation_state->setWeight(0);

	jet_pack_jumping_animation_state = jet_pack->entity->getAnimationState("Jump");
	jet_pack_jumping_animation_state->setLoop(false);
	jet_pack_jumping_animation_state->setWeight(0);

	jet_pack_die_animation_state = jet_pack->entity->getAnimationState("Death");
	jet_pack_die_animation_state->setLoop(false);
	jet_pack_die_animation_state->setWeight(0);

	jet_pack->node->setVisible(true);

	jet_pack_shoot_pos1 = new PlayerBox(this, "Hitbox.003.mesh",
										0.2, 0.609656, -0.792315,
										0, 0, 0, 1,
										0.049863, 0.049863, 0.049863,
										COL_NOTHING, COL_NOTHING);

	jet_pack_shoot_pos2 = new PlayerBox(this, "Hitbox.005.mesh",
										0.3, 0.609656, -0.792315,
										0, 0, 0, 1,
										0.049863, 0.049863, 0.049863,
										COL_NOTHING, COL_NOTHING);

	// <node name="Hitbox.PixelMan.Chest" >
 //        <position y="-0.475911" x="0.000000" z="-0.101981" />
 //        <rotation qw="1.000000" qz="-0.000000" qy="0.000000" qx="0.000000" />
 //        <scale y="2.157996" x="0.824681" z="0.907342" />
 //        <game >
 //          <sensors />
 //          <actuators />
 //        </game>
 //        <entity mass_radius="1.0" ghost="False" velocity_min="0.0" physics_type="STATIC" inertia_tensor="0.4000000059604645" meshFile="Hitbox.mesh" lock_trans_y="False" lock_trans_x="False" lock_trans_z="False" friction_z="1.0" friction_y="1.0" friction_x="1.0" name="Hitbox" velocity_max="0.0" mass="1.0" lock_rot_z="False" actor="False" lock_rot_y="False" damping_trans="0.03999999910593033" lock_rot_x="False" anisotropic_friction="False" damping_rot="0.10000000149011612" />
 //      </node>
 //      <node name="Hitbox.PixelMan.Head" >
 //        <position y="2.120457" x="-0.000000" z="-0.137225" />
 //        <rotation qw="1.000000" qz="-0.000000" qy="0.000000" qx="0.000000" />
 //        <scale y="0.425870" x="0.353341" z="0.350068" />


	body_box = new HitBox(this, "Hitbox.mesh",
							0, -4.75911, -1.01981, 
							0, 0, 0, 1,
							8.24681, 21.57996, 9.07342, 1.0);

	head_box = new HitBox(this, "Hitbox2.mesh",
							0, 20.120457, -1.37225, 
							0, 0, 0, 1,
							3.53341, 4.25870, 3.50068, 3.0);

	weapons[PISTOL_ID] = pistol;
	current_weapon = pistol;

	is_shooting = false;
	is_moving = false;
	is_idle = false;
	is_reloading = false;
	is_jet_packing = false;
	is_jumping = false;
	is_running = false;
	is_dead = false;

	weapon_running_animation_state = current_weapon->running_animation_state;
	weapon_idle_animation_state = current_weapon->idle_animation_state;
	weapon_shooting_animation_state = current_weapon->shooting_animation_state;
	weapon_reload_animation_state = current_weapon->reload_animation_state;
	weapon_jumping_animation_state = current_weapon->jumping_animation_state;
	weapon_die_animation_state = current_weapon->die_animation_state;

	current_reload_animation_state = reload_animation_states[current_weapon->weapon_id];
	current_shooting_animation_state = shooting_animation_states[current_weapon->weapon_id];
	current_jet_pack_reload_animation_state = jet_pack_reload_animation_states[current_weapon->weapon_id];
	current_jet_pack_shooting_animation_state = jet_pack_shooting_animation_states[current_weapon->weapon_id];
}

PlayerCharacter:: ~PlayerCharacter()
{
	if(is_yourself)
	{
		NetworkManager::instance()->vital->setPlayerDie();
		GameState::instance()->players[NetworkManager::instance()->player_id] = NULL;
		GameState::instance()->player = NULL;
		scene->removeGameObject((GameObject*)this);
		scene->main_camera = NULL;
	}

	delete health_regen_debouncer;
	delete walk_sound_debouncer;
}

void PlayerCharacter::update()
{
	GameObject::update();

	weapon_running_animation_state = current_weapon->running_animation_state;
	weapon_idle_animation_state = current_weapon->idle_animation_state;
	weapon_shooting_animation_state = current_weapon->shooting_animation_state;
	weapon_reload_animation_state = current_weapon->reload_animation_state;
	weapon_jumping_animation_state = current_weapon->jumping_animation_state;
	weapon_die_animation_state = current_weapon->die_animation_state;

	current_reload_animation_state = reload_animation_states[current_weapon->weapon_id];
	current_shooting_animation_state = shooting_animation_states[current_weapon->weapon_id];
	current_jet_pack_reload_animation_state = jet_pack_reload_animation_states[current_weapon->weapon_id];
	current_jet_pack_shooting_animation_state = jet_pack_shooting_animation_states[current_weapon->weapon_id];

	if(is_yourself)
	{
		is_shooting = false;
		is_moving = false;
		is_idle = false;
		is_reloading = false;
		is_jet_packing = false;
		is_jumping = false;
		is_running = false;

		if((health <= 0 || transform->posY <= -150) && !is_dead)
		{
			is_dead = true;
			controller->can_move = false;
			health = 0;
			AudioManager::instance()->playDeath(Ogre::Vector3(transform->posX, transform->posY, transform->posZ));
		}

		if(is_dead)
		{
			//LOG("Playing dead");

			current_shooting_animation_state->setWeight(0);
			current_shooting_animation_state->setEnabled(false);
			current_shooting_animation_state->setTimePosition(0);

			current_jet_pack_shooting_animation_state->setWeight(0);
			current_jet_pack_shooting_animation_state->setEnabled(false);
			current_jet_pack_shooting_animation_state->setTimePosition(0);

			current_reload_animation_state->setWeight(0);
			current_reload_animation_state->setEnabled(false);
			current_reload_animation_state->setTimePosition(0);

			current_jet_pack_reload_animation_state->setWeight(0);
			current_jet_pack_reload_animation_state->setEnabled(false);
			current_jet_pack_reload_animation_state->setTimePosition(0);

			weapon_shooting_animation_state->setWeight(0);
			weapon_shooting_animation_state->setEnabled(false);
			weapon_shooting_animation_state->setTimePosition(0);

			weapon_reload_animation_state->setWeight(0);
			weapon_reload_animation_state->setEnabled(false);
			weapon_reload_animation_state->setTimePosition(0);

			idle_animation_state->setWeight(0);
			idle_animation_state->setEnabled(false);
			idle_animation_state->setTimePosition(0);

			jet_pack_idle_animation_state->setWeight(0);
			jet_pack_idle_animation_state->setEnabled(false);
			jet_pack_idle_animation_state->setTimePosition(0);

			weapon_idle_animation_state->setWeight(0);
			weapon_idle_animation_state->setEnabled(false);
			weapon_idle_animation_state->setTimePosition(0);

			die_animation_state->setWeight(1);
			die_animation_state->setEnabled(true);
			die_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

			jet_pack_die_animation_state->setWeight(1);
			jet_pack_die_animation_state->setEnabled(true);
			jet_pack_die_animation_state->setTimePosition(die_animation_state->getTimePosition());

			weapon_die_animation_state->setWeight(1);
			weapon_die_animation_state->setEnabled(true);
			weapon_die_animation_state->setTimePosition(die_animation_state->getTimePosition());

			running_animation_state->setWeight(0);
			running_animation_state->setEnabled(false);
			running_animation_state->setTimePosition(0);

			jet_pack_running_animation_state->setWeight(0);
			jet_pack_running_animation_state->setEnabled(false);
			jet_pack_running_animation_state->setTimePosition(0);

			weapon_running_animation_state->setWeight(0);
			weapon_running_animation_state->setEnabled(false);
			weapon_running_animation_state->setTimePosition(0);

			if(die_animation_state->hasEnded())
			{
				delete this;
				GuiManager::instance()->Reinitialize();
				return;
			}
		}
		else
		{
			die_animation_state->setWeight(0);
			die_animation_state->setEnabled(false);
			jet_pack_die_animation_state->setWeight(0);
			jet_pack_die_animation_state->setEnabled(false);
			weapon_die_animation_state->setWeight(0);
			weapon_die_animation_state->setEnabled(false);

			if(current_weapon)
			{
				current_weapon->shoot();
			}

			if((InputManager::instance()->isKeyPressed(OIS::KC_R) || (current_weapon->current_mag_count == 0)) && current_weapon)
			{
				current_weapon->reload();
			}

			auto regen_function = [](PlayerCharacter* player)
			{
				if(!player->is_dead)
				{
					player->health = ((player->health + player->health_regen) >= 100.0) ? (100.0) : (player->health + player->health_regen); 
				}
			};

			if(!is_dead && health_regen_debouncer->run(NULL))
				regen_health();

			if(controller->is_jet_packing && InputManager::instance()->isMouseRightDown())
			{
				is_jet_packing = true;
				jet_pack_sound_debouncer->run();
				jet_pack_particle_debouncer->run();
			}
			else
			{
				is_jet_packing = false;
			}

			if(!controller->is_jet_packing && !controller->controller->onGround() && controller->controller->isJumping())
			{
				// LOG("Playing jump");

				running_animation_state->setWeight(0);
				weapon_running_animation_state->setWeight(0);
				idle_animation_state->setWeight(0);
				weapon_idle_animation_state->setWeight(0);

				jet_pack_idle_animation_state->setWeight(0);
				jet_pack_running_animation_state->setWeight(0);

				is_jumping = true;
				jumping_animation_state->setWeight(1);
				jumping_animation_state->setEnabled(true);
				jumping_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

				jet_pack_jumping_animation_state->setWeight(1);
				jet_pack_jumping_animation_state->setEnabled(true);
				jet_pack_jumping_animation_state->setTimePosition(jumping_animation_state->getTimePosition());

				weapon_jumping_animation_state->setWeight(1);
				weapon_jumping_animation_state->setEnabled(true);
				weapon_jumping_animation_state->setTimePosition(jumping_animation_state->getTimePosition());
			}
			else
			{
				is_jumping = false;

				jumping_animation_state->setWeight(0);
				jumping_animation_state->setEnabled(false);
				jumping_animation_state->setTimePosition(0);

				jet_pack_jumping_animation_state->setWeight(0);
				jet_pack_jumping_animation_state->setEnabled(false);
				jet_pack_jumping_animation_state->setTimePosition(0);

				weapon_jumping_animation_state->setWeight(0);
				weapon_jumping_animation_state->setEnabled(false);
				weapon_jumping_animation_state->setTimePosition(0);
			}

			if(controller->is_walking && !controller->is_jet_packing && controller->controller->onGround())
			{
				is_moving = true;
				// LOG("Playing walk");

				running_animation_state->setWeight(1);
				running_animation_state->setEnabled(true);

				jet_pack_running_animation_state->setWeight(1);
				jet_pack_running_animation_state->setEnabled(true);

				if(controller->is_running)
				{
					is_running = true;
					running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame * 1.3f);
				}
				else
				{
					running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
				}

				if(running_animation_state->getTimePosition() > 0.2f && running_animation_state->getTimePosition() < 0.3f)
					walk_sound_debouncer->run();
				else if(running_animation_state->getTimePosition() > 0.5f && running_animation_state->getTimePosition() < 0.7f)
					walk_sound_debouncer->run();

				jet_pack_running_animation_state->setWeight(1);
				jet_pack_running_animation_state->setEnabled(true);
				jet_pack_running_animation_state->setTimePosition(running_animation_state->getTimePosition());

				weapon_running_animation_state->setWeight(1);
				weapon_running_animation_state->setEnabled(true);
				weapon_running_animation_state->setTimePosition(running_animation_state->getTimePosition());
			}
			else
			{
				is_moving = false;

				running_animation_state->setWeight(0);
				running_animation_state->setEnabled(false);
				running_animation_state->setTimePosition(0);

				jet_pack_running_animation_state->setWeight(0);
				jet_pack_running_animation_state->setEnabled(false);
				jet_pack_running_animation_state->setTimePosition(0);

				weapon_running_animation_state->setWeight(0);
				weapon_running_animation_state->setEnabled(false);
				weapon_running_animation_state->setTimePosition(0);
			}


			if(current_weapon->is_shooting && !weapon_shooting_animation_state->hasEnded())
			{
				// LOG("Playing shoot");

				running_animation_state->setWeight(0.5);
				weapon_running_animation_state->setWeight(0.5);
				idle_animation_state->setWeight(0.5);
				weapon_idle_animation_state->setWeight(0.5);

				is_shooting = true;
				weapon_shooting_animation_state->setWeight(1);
				weapon_shooting_animation_state->setEnabled(true);
				weapon_shooting_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

				current_jet_pack_shooting_animation_state->setWeight(1);
				current_jet_pack_shooting_animation_state->setEnabled(true);
				current_jet_pack_shooting_animation_state->setTimePosition(weapon_shooting_animation_state->getTimePosition());

				current_shooting_animation_state->setWeight(1);
				current_shooting_animation_state->setEnabled(true);
				current_shooting_animation_state->setTimePosition(weapon_shooting_animation_state->getTimePosition());
			}
			else
			{
				is_shooting = false;
				current_weapon->is_shooting = false;
				current_shooting_animation_state->setWeight(0);
				current_shooting_animation_state->setEnabled(false);
				current_shooting_animation_state->setTimePosition(0);

				current_jet_pack_shooting_animation_state->setWeight(0);
				current_jet_pack_shooting_animation_state->setEnabled(false);
				current_jet_pack_shooting_animation_state->setTimePosition(0);

				weapon_shooting_animation_state->setWeight(0);
				weapon_shooting_animation_state->setEnabled(false);
				weapon_shooting_animation_state->setTimePosition(0);
			}

			if(current_weapon->is_reloading)
			{
				// LOG("Playing reload");

				running_animation_state->setWeight(0.5);
				weapon_running_animation_state->setWeight(0.5);
				idle_animation_state->setWeight(0.5);
				weapon_idle_animation_state->setWeight(0.5);

				is_reloading = true;
				weapon_reload_animation_state->setWeight(1);
				weapon_reload_animation_state->setEnabled(true);
				weapon_reload_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

				current_jet_pack_reload_animation_state->setWeight(1);
				current_jet_pack_reload_animation_state->setEnabled(true);
				current_jet_pack_reload_animation_state->setTimePosition(weapon_reload_animation_state->getTimePosition());
				
				current_reload_animation_state->setWeight(1);
				current_reload_animation_state->setEnabled(true);
				current_reload_animation_state->setTimePosition(weapon_reload_animation_state->getTimePosition());
			}
			else
			{
				is_reloading = false;
				current_reload_animation_state->setWeight(0);
				current_reload_animation_state->setEnabled(false);
				current_reload_animation_state->setTimePosition(0);

				current_jet_pack_reload_animation_state->setWeight(0);
				current_jet_pack_reload_animation_state->setEnabled(false);
				current_jet_pack_reload_animation_state->setTimePosition(0);

				weapon_reload_animation_state->setWeight(0);
				weapon_reload_animation_state->setEnabled(false);
				weapon_reload_animation_state->setTimePosition(0);
			}

			if((is_jet_packing || !controller->controller->onGround() || (!controller->is_walking && controller->controller->onGround())))
			{
				// LOG("Playing idle");

				is_idle = true;

				idle_animation_state->setWeight(1);
				idle_animation_state->setEnabled(true);
				idle_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

				jet_pack_idle_animation_state->setWeight(1);
				jet_pack_idle_animation_state->setEnabled(true);
				jet_pack_idle_animation_state->setTimePosition(idle_animation_state->getTimePosition());

				weapon_idle_animation_state->setWeight(1);
				weapon_idle_animation_state->setEnabled(true);
				weapon_idle_animation_state->setTimePosition(idle_animation_state->getTimePosition());
			}
			else
			{
				is_idle = false;

				idle_animation_state->setWeight(0);
				idle_animation_state->setEnabled(false);
				idle_animation_state->setTimePosition(0);

				jet_pack_idle_animation_state->setWeight(0);
				jet_pack_idle_animation_state->setEnabled(false);
				jet_pack_idle_animation_state->setTimePosition(0);

				weapon_idle_animation_state->setWeight(0);
				weapon_idle_animation_state->setEnabled(false);
				weapon_idle_animation_state->setTimePosition(0);
			}

			Ogre::Vector3 dir = ((Camera*)(controller->fps_camera))->camera->getDirection();
		}


		run_animation_time = running_animation_state->getTimePosition();
		shoot_animation_time = current_shooting_animation_state->getTimePosition();
		idle_animation_time = idle_animation_state->getTimePosition();
		reload_animation_time = current_reload_animation_state->getTimePosition();
		jump_animation_time = jumping_animation_state->getTimePosition();
		die_animation_time = die_animation_state->getTimePosition();

		NetworkManager::instance()->heartbeat->renewPlayerInfo(this);

		// LOG("is_dead " << is_dead);
		// LOG("is_shooting " << is_shooting);
		// LOG("is_moving " << is_moving);
		// LOG("is_idle " << is_idle);
		// LOG("is_reloading " << is_reloading);
		// LOG("is_jet_packing " << is_jet_packing);
		// LOG("is_jumping " << is_jumping);

	}
	else
	{
		if(is_dead)
		{
			die_animation_state->setEnabled(true);
			die_animation_state->setWeight(1);
			weapon_die_animation_state->setEnabled(true);
			weapon_die_animation_state->setWeight(1);
			jet_pack_die_animation_state->setEnabled(true);
			jet_pack_die_animation_state->setWeight(1);

			die_animation_state->setTimePosition(die_animation_time);
			weapon_die_animation_state->setTimePosition(die_animation_time);
			jet_pack_die_animation_state->setTimePosition(die_animation_time);
		}
		else
		{
			die_animation_state->setEnabled(false);
			die_animation_state->setWeight(0);
			weapon_die_animation_state->setEnabled(false);
			weapon_die_animation_state->setWeight(0);
			jet_pack_die_animation_state->setEnabled(false);
			jet_pack_die_animation_state->setWeight(0);

			die_animation_state->setTimePosition(0);
			weapon_die_animation_state->setTimePosition(0);
			jet_pack_die_animation_state->setTimePosition(0);

			if(is_jet_packing)
			{
				jet_pack_sound_debouncer->run();
				jet_pack_particle_debouncer->run();
			}

			if(is_moving)
			{
				running_animation_state->setEnabled(true);
				running_animation_state->setWeight(1);
				weapon_running_animation_state->setEnabled(true);
				weapon_running_animation_state->setWeight(1);
				jet_pack_running_animation_state->setEnabled(true);
				jet_pack_running_animation_state->setWeight(1);

				running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
				weapon_running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
				jet_pack_running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
			}
			else
			{
				running_animation_state->setEnabled(false);
				running_animation_state->setWeight(0);
				weapon_running_animation_state->setEnabled(false);
				weapon_running_animation_state->setWeight(0);
				jet_pack_running_animation_state->setEnabled(false);
				jet_pack_running_animation_state->setWeight(0);

				running_animation_state->setTimePosition(0);
				weapon_running_animation_state->setTimePosition(0);
				jet_pack_running_animation_state->setTimePosition(0);
			}

			if(is_shooting)
			{
				running_animation_state->setWeight(0.5);
				weapon_running_animation_state->setWeight(0.5);
				idle_animation_state->setWeight(0.5);
				weapon_idle_animation_state->setWeight(0.5);
				jet_pack_idle_animation_state->setWeight(0.5);
				jet_pack_running_animation_state->setWeight(0.5);

				current_shooting_animation_state->setEnabled(true);
				current_shooting_animation_state->setWeight(1);
				weapon_shooting_animation_state->setEnabled(true);
				weapon_shooting_animation_state->setWeight(1);
				current_jet_pack_shooting_animation_state->setEnabled(true);
				current_jet_pack_shooting_animation_state->setWeight(1);

				current_shooting_animation_state->setTimePosition(shoot_animation_time);
				weapon_shooting_animation_state->setTimePosition(shoot_animation_time);
				current_jet_pack_shooting_animation_state->setTimePosition(shoot_animation_time);
			}
			else
			{
				current_shooting_animation_state->setEnabled(false);
				current_shooting_animation_state->setWeight(0);
				weapon_shooting_animation_state->setEnabled(false);
				weapon_shooting_animation_state->setWeight(0);
				current_jet_pack_shooting_animation_state->setEnabled(false);
				current_jet_pack_shooting_animation_state->setWeight(0);
			}

			if(is_idle)
			{
				idle_animation_state->setEnabled(true);
				idle_animation_state->setWeight(1);
				weapon_idle_animation_state->setEnabled(true);
				weapon_idle_animation_state->setWeight(1);
				jet_pack_idle_animation_state->setEnabled(true);
				jet_pack_idle_animation_state->setWeight(1);

				idle_animation_state->setTimePosition(idle_animation_time);
				weapon_idle_animation_state->setTimePosition(idle_animation_time);
				jet_pack_idle_animation_state->setTimePosition(idle_animation_time);
			}
			else
			{
				idle_animation_state->setEnabled(false);
				idle_animation_state->setWeight(0);
				weapon_idle_animation_state->setEnabled(false);
				weapon_idle_animation_state->setWeight(0);
				jet_pack_idle_animation_state->setEnabled(false);
				jet_pack_idle_animation_state->setWeight(0);

				idle_animation_state->setTimePosition(0);
				weapon_idle_animation_state->setTimePosition(0);
				jet_pack_idle_animation_state->setTimePosition(0);
			}

			if(is_reloading)
			{
				running_animation_state->setWeight(0.5);
				weapon_running_animation_state->setWeight(0.5);
				idle_animation_state->setWeight(0.5);
				weapon_idle_animation_state->setWeight(0.5);
				jet_pack_idle_animation_state->setWeight(0.5);
				jet_pack_running_animation_state->setWeight(0.5);

				current_reload_animation_state->setEnabled(true);
				current_reload_animation_state->setWeight(1);
				weapon_reload_animation_state->setEnabled(true);
				weapon_reload_animation_state->setWeight(1);
				current_jet_pack_reload_animation_state->setEnabled(true);
				current_jet_pack_reload_animation_state->setWeight(1);

				current_reload_animation_state->setTimePosition(reload_animation_time);
				weapon_reload_animation_state->setTimePosition(reload_animation_time);
				current_jet_pack_reload_animation_state->setTimePosition(reload_animation_time);
			}
			else
			{
				current_reload_animation_state->setEnabled(false);
				current_reload_animation_state->setWeight(0);
				weapon_reload_animation_state->setEnabled(false);
				weapon_reload_animation_state->setWeight(0);
				current_jet_pack_reload_animation_state->setEnabled(false);
				current_jet_pack_reload_animation_state->setWeight(0);
			}

			if(is_jumping)
			{
				running_animation_state->setWeight(0);
				weapon_running_animation_state->setWeight(0);
				idle_animation_state->setWeight(0);
				weapon_idle_animation_state->setWeight(0);
				jet_pack_idle_animation_state->setWeight(0);
				jet_pack_running_animation_state->setWeight(0);

				jumping_animation_state->setEnabled(true);
				jumping_animation_state->setWeight(1);
				jumping_animation_state->setTimePosition(jump_animation_time);

				weapon_jumping_animation_state->setEnabled(true);
				weapon_jumping_animation_state->setWeight(1);
				weapon_jumping_animation_state->setTimePosition(jump_animation_time);

				jet_pack_jumping_animation_state->setEnabled(true);
				jet_pack_jumping_animation_state->setWeight(1);
				jet_pack_jumping_animation_state->setTimePosition(jump_animation_time);
			}
			else
			{
				jumping_animation_state->setEnabled(false);
				jumping_animation_state->setWeight(0);

				weapon_jumping_animation_state->setEnabled(false);
				weapon_jumping_animation_state->setWeight(0);

				jet_pack_jumping_animation_state->setEnabled(false);
				jet_pack_jumping_animation_state->setWeight(0);
			}
		}
	}
}

void PlayerCharacter::regen_health()
{
	if(!is_dead)
	{
		health = ((health + health_regen) >= 100.0) ? (100.0) : (health + health_regen); 
	}
}

void PlayerCharacter::changeWeapon(int index)
{
	LOG("Change weapon 1");
	ASSERT((index < WEAPON_NUM) && (index >= 0), "Invalid weapon index")

	current_weapon->node->setVisible(false);
	current_weapon = weapons[index];
	current_weapon->switchToThisWeapon();
	current_weapon->node->setVisible(true);

	LOG("Change weapon 2");

	if(is_yourself)
	{
		NetworkManager::instance()->vital->setChangeWeapon(index);
	}

	LOG("Change weapon 3");
}
