#include "PlayerCharacter.h"

#include "Pistol.h"

PlayerCharacter::PlayerCharacter(bool is_yourself_p, Scene* scene, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ) : GameObject("Player", scene)
{
	is_yourself = is_yourself_p;
	is_shooting = false;
	is_idle = false;
	is_dead = false;

	run_animation_time = 0.0;
	shoot_animation_time = 0.0;
	idle_animation_time = 0.0;

	health = 100.0;
	health_regen = 1.0;
	health_regen_rate = 1.0;


	Transform* tran = this->getComponent<Transform>();
	tr = tran;

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

	shooting_animation_states[0] = mesh->entity->getAnimationState("Shooting");
	shooting_animation_states[0]->setLoop(false);
	shooting_animation_states[0]->setWeight(0);

	reload_animation_states[0] = mesh->entity->getAnimationState("Reloading");
	reload_animation_states[0]->setLoop(false);
	reload_animation_states[0]->setWeight(0);

	//load the other weapon animations

	running_animation_state = mesh->entity->getAnimationState("Running");
	running_animation_state->setLoop(true);
	running_animation_state->setWeight(0);

	idle_animation_state = mesh->entity->getAnimationState("Idle");
	idle_animation_state->setLoop(true);
	idle_animation_state->setWeight(0);

	jumping_animation_state = mesh->entity->getAnimationState("Jump");
	jumping_animation_state->setLoop(false);
	jumping_animation_state->setWeight(0);

	head_animation_state = mesh->entity->getAnimationState("Head Pitch");
	head_animation_state->setLoop(true);
	head_animation_state->setWeight(0);
	head_animation_state->setEnabled(true);

	die_animation_state = mesh->entity->getAnimationState("Death");
	die_animation_state->setLoop(false);
	die_animation_state->setWeight(0);


    health_regen_debouncer = new Debouncer(health_regen_rate * 1000);

	if(is_yourself)
  	{
		controller = new FPSBoxController(this, "Camm", box_half_size.y * scaleY + 1, btVector3(box_half_size.x * scaleX, box_half_size.y * scaleY, box_half_size.z * scaleZ), 
											3, COL_CHARACTER, CHARACTER_COLLIDER_WITH, mesh->node);

		scene->main_camera = (Camera*)(controller->fps_camera);
	}

   // <node name="Rifle" >
   //      <position y="0.000000" x="0.000000" z="-0.000000" />
   //      <rotation qz="-0.000000" qy="0.000000" qx="0.709822" qw="0.704381" />
   //      <scale y="1.000000" x="1.000000" z="1.000000" />
   //      <game >

	// </node>
	//      <node name="Muzzle.Rifle" >
	//        <position y="0.000000" x="0.000000" z="-0.000000" />
	//        <rotation qz="-0.000000" qy="0.000000" qx="0.000000" qw="1.000000" />
	//        <scale y="0.049863" x="0.049863" z="0.049863" />
	//        <game >
	//          <sensors />
	//          <actuators />
	//        </game>
	//        <entity velocity_max="0.0" velocity_min="0.0" damping_rot="0.10000000149011612" mass_radius="1.0" ghost="False" lock_rot_z="False" lock_rot_x="False" lock_rot_y="False" damping_trans="0.03999999910593033" friction_z="1.0" friction_y="1.0" friction_x="1.0" physics_type="STATIC" name="Hitbox.002" anisotropic_friction="False" meshFile="Hitbox.002.mesh" lock_trans_y="False" lock_trans_x="False" inertia_tensor="0.4000000059604645" lock_trans_z="False" actor="False" mass="1.0" />
	//      </node>

	//hard code position
	Weapon* pistol = new Pistol(this, "Gun.mesh", 
			0, 0, 0, 
			0.709822, 0, 0, 0.704381,
			10, 10, 10, 
			new PlayerBox(this, "sphere.mesh",
			0, 0, 0, 
			0, 0, 0, 1,
			0.049863, 0.049863, 0.049863, COL_NOTHING, COL_NOTHING));

	// <node name="Hitbox.Head" >
 //        <position z="-0.000000" x="0.000000" y="0.000000" />
 //        <rotation qw="1.000000" qx="0.000000" qy="0.000000" qz="-0.000000" />
 //        <scale z="0.100000" x="0.100000" y="0.100000" />
 //        <game >
 //          <sensors />
 //          <actuators />
 //        </game>

	head_box = new HitBox(this, "Hitbox.mesh",
							0, 0, 0, 
							0, 0, 0, 1,
							1, 1, 1, 2.0);

	// // <node name="Hitbox.Chest" >
 // //        <position z="-0.000000" x="0.000000" y="0.000000" />
 // //        <rotation qw="1.000000" qx="0.000000" qy="0.000000" qz="-0.000000" />
 // //        <scale z="0.100000" x="0.100000" y="0.116851" />
 // //        <game >
 // //          <sensors />

	body_box = new HitBox(this, "Hitbox.001.mesh",
							0, 0, 0, 
							0, 0, 0, 1,
							1, 1.16851, 1, 2.0);

	weapons[0] = pistol;
	current_weapon = pistol;

	weapon_running_animation_state = current_weapon->running_animation_state;
	weapon_idle_animation_state = current_weapon->idle_animation_state;
	weapon_shooting_animation_state = current_weapon->shooting_animation_state;
	weapon_reload_animation_state = current_weapon->reload_animation_state;
	weapon_jumping_animation_state = current_weapon->jumping_animation_state;
	weapon_die_animation_state = current_weapon->die_animation_state;

	current_reload_animation_state = reload_animation_states[current_weapon->weapon_id];
	current_shooting_animation_state = shooting_animation_states[current_weapon->weapon_id];

	GameState::instance()->player = this;
}

PlayerCharacter:: ~PlayerCharacter()
{
	scene->main_camera = NULL;
	delete health_regen_debouncer;
}

void PlayerCharacter::update()
{
	GameObject::update();

	if(is_yourself)
	{
		is_dead = false;
		is_shooting = false;
		is_moving = false;
		is_idle = false;
		is_reloading = false;
		is_jet_packing = false;
		is_jumping = false;

		weapon_running_animation_state = current_weapon->running_animation_state;
		weapon_idle_animation_state = current_weapon->idle_animation_state;
		weapon_shooting_animation_state = current_weapon->shooting_animation_state;
		weapon_reload_animation_state = current_weapon->reload_animation_state;
		weapon_jumping_animation_state = current_weapon->jumping_animation_state;
		weapon_die_animation_state = current_weapon->die_animation_state;

		current_reload_animation_state = reload_animation_states[current_weapon->weapon_id];
		current_shooting_animation_state = shooting_animation_states[current_weapon->weapon_id];

		//!!!!check network to take damage

		if(health <= 0 && !is_dead)
		{
			is_dead = true;
			controller->can_move = false;
			health = 0;
		}

		if(is_dead)
		{
			//LOG("Playing dead");
    		Ogre::Vector3 curPos = Ogre::Vector3(tr->posX, tr->posY, tr->posZ);
			AudioManager::instance()->playDeath(curPos);

			die_animation_state->setWeight(1);
			die_animation_state->setEnabled(true);
			die_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

			weapon_die_animation_state->setWeight(1);
			weapon_die_animation_state->setEnabled(true);
			weapon_die_animation_state->setTimePosition(die_animation_state->getTimePosition());

			if(die_animation_state->getTimePosition() >= (die_animation_state->getLength() + 3.0f))
			{
				//Notify server
			}
		}
		else
		{
			if(InputManager::instance()->isMouseLeftClicked() && current_weapon)
			{
				current_weapon->shoot();
			}

			if(InputManager::instance()->isKeyPressed(OIS::KC_R) && current_weapon)
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

			if(!controller->is_jet_packing && !controller->controller->onGround())
			{
				//LOG("Playing jump");

				is_jumping = true;
				jumping_animation_state->setWeight(1);
				jumping_animation_state->setEnabled(true);
				jumping_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
			}
			else
			{
				is_jumping = false;

				jumping_animation_state->setWeight(0);
				jumping_animation_state->setEnabled(false);
				jumping_animation_state->setTimePosition(0);
			}

			if(controller->is_walking && !controller->is_jet_packing && controller->controller->onGround())
			{
				is_moving = true;
				//LOG("Playing walk");

				running_animation_state->setWeight(1);
				running_animation_state->setEnabled(true);

				if(controller->is_running)
					running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame * 1.3f);
				else
					running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

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

				weapon_running_animation_state->setWeight(0);
				weapon_running_animation_state->setEnabled(false);
				weapon_running_animation_state->setTimePosition(0);
			}

			if(current_weapon->is_shooting && !current_shooting_animation_state->hasEnded() && !weapon_shooting_animation_state->hasEnded())
			{
				//LOG("Playing shoot");

				running_animation_state->setWeight(0.5);
				weapon_running_animation_state->setWeight(0.5);
				idle_animation_state->setWeight(0.5);
				weapon_idle_animation_state->setWeight(0.5);

				is_shooting = true;
				weapon_shooting_animation_state->setWeight(1);
				weapon_shooting_animation_state->setEnabled(true);
				weapon_shooting_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

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

				weapon_shooting_animation_state->setWeight(0);
				weapon_shooting_animation_state->setEnabled(false);
				weapon_shooting_animation_state->setTimePosition(0);
			}

			if(current_weapon->is_reloading)
			{
				//LOG("Playing reload");

				running_animation_state->setWeight(0.5);
				weapon_running_animation_state->setWeight(0.5);
				idle_animation_state->setWeight(0.5);
				weapon_idle_animation_state->setWeight(0.5);

				is_reloading = true;
				weapon_reload_animation_state->setWeight(1);
				weapon_reload_animation_state->setEnabled(true);
				weapon_reload_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
				
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

				weapon_reload_animation_state->setWeight(0);
				weapon_reload_animation_state->setEnabled(false);
				weapon_reload_animation_state->setTimePosition(0);
			}

			if(controller->is_jet_packing || !controller->controller->onGround() || (!controller->is_walking && controller->controller->onGround()))
			{
				//LOG("Playing idle");

				is_idle = true;

				idle_animation_state->setWeight(1);
				idle_animation_state->setEnabled(true);
				idle_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

				weapon_idle_animation_state->setWeight(1);
				weapon_idle_animation_state->setEnabled(true);
				weapon_idle_animation_state->addTime(idle_animation_state->getTimePosition());
			}
			else
			{
				is_idle = false;

				idle_animation_state->setWeight(0);
				idle_animation_state->setEnabled(false);
				idle_animation_state->setTimePosition(0);

				weapon_idle_animation_state->setWeight(0);
				weapon_idle_animation_state->setEnabled(false);
				weapon_idle_animation_state->setTimePosition(0);
			}

			Ogre::Vector3 dir = ((Camera*)(controller->fps_camera))->camera->getDirection();
			head_animation_state->setTimePosition((dir.y + 0.5) * head_animation_state->getLength());
		}

		run_animation_time = running_animation_state->getTimePosition();
		shoot_animation_time = current_shooting_animation_state->getTimePosition();
		idle_animation_time = idle_animation_state->getTimePosition();
		reload_animation_time = current_reload_animation_state->getTimePosition();
		jump_animation_time = jumping_animation_state->getTimePosition();
		head_animation_time = head_animation_state->getTimePosition();
		die_animation_time = die_animation_state->getTimePosition();

		// sent info to sever

		// //LOG("is_dead " << is_dead);
		// //LOG("is_shooting " << is_shooting);
		// //LOG("is_moving " << is_moving);
		// //LOG("is_idle " << is_idle);
		// //LOG("is_reloading " << is_reloading);
		// //LOG("is_jet_packing " << is_jet_packing);
		// //LOG("is_jumping " << is_jumping);

		weapon_running_animation_state->setEnabled(false);
		weapon_idle_animation_state->setEnabled(false);
		weapon_shooting_animation_state->setEnabled(false);
		weapon_reload_animation_state->setEnabled(false);
		weapon_jumping_animation_state->setEnabled(false);
		weapon_die_animation_state->setEnabled(false);
	}
	else
	{
		// get info from server
		if(is_dead)
		{
			die_animation_state->setEnabled(true);
			die_animation_state->setWeight(1);
			weapon_die_animation_state->setEnabled(true);
			weapon_die_animation_state->setWeight(1);

			die_animation_state->setTimePosition(die_animation_time);
			weapon_die_animation_state->setTimePosition(die_animation_time);
		}
		else
		{
			die_animation_state->setEnabled(false);
			die_animation_state->setWeight(0);
			weapon_die_animation_state->setEnabled(false);
			weapon_die_animation_state->setWeight(0);

			if(is_moving)
			{
				running_animation_state->setEnabled(true);
				running_animation_state->setWeight(1);
				weapon_running_animation_state->setEnabled(true);
				weapon_running_animation_state->setWeight(1);

				running_animation_state->setTimePosition(run_animation_time);
				weapon_running_animation_state->setTimePosition(run_animation_time);
			}
			else
			{
				running_animation_state->setEnabled(false);
				running_animation_state->setWeight(0);
				weapon_running_animation_state->setEnabled(false);
				weapon_running_animation_state->setWeight(0);
			}

			if(is_shooting)
			{
				current_shooting_animation_state->setEnabled(true);
				current_shooting_animation_state->setWeight(1);
				weapon_shooting_animation_state->setEnabled(true);
				weapon_shooting_animation_state->setWeight(1);

				current_shooting_animation_state->setTimePosition(shoot_animation_time);
				weapon_shooting_animation_state->setTimePosition(shoot_animation_time);
			}
			else
			{
				current_shooting_animation_state->setEnabled(false);
				current_shooting_animation_state->setWeight(0);
				weapon_shooting_animation_state->setEnabled(false);
				weapon_shooting_animation_state->setWeight(0);
			}

			if(is_idle)
			{
				idle_animation_state->setEnabled(true);
				idle_animation_state->setWeight(1);
				weapon_idle_animation_state->setEnabled(true);
				weapon_idle_animation_state->setWeight(1);

				idle_animation_state->setTimePosition(idle_animation_time);
				weapon_idle_animation_state->setTimePosition(idle_animation_time);
			}
			else
			{
				idle_animation_state->setEnabled(false);
				idle_animation_state->setWeight(0);
				weapon_idle_animation_state->setEnabled(false);
				weapon_idle_animation_state->setWeight(0);
			}

			if(is_reloading)
			{
				current_reload_animation_state->setEnabled(true);
				current_reload_animation_state->setWeight(1);
				weapon_reload_animation_state->setEnabled(true);
				weapon_reload_animation_state->setWeight(1);

				current_reload_animation_state->setTimePosition(reload_animation_time);
				weapon_reload_animation_state->setTimePosition(reload_animation_time);
			}
			else
			{
				current_reload_animation_state->setEnabled(false);
				current_reload_animation_state->setWeight(0);
				weapon_reload_animation_state->setEnabled(false);
				weapon_reload_animation_state->setWeight(0);
			}

			if(is_jumping)
			{
				jumping_animation_state->setEnabled(true);
				jumping_animation_state->setWeight(1);
				jumping_animation_state->setTimePosition(jump_animation_time);
			}
			else
			{
				jumping_animation_state->setEnabled(false);
				jumping_animation_state->setWeight(0);
			}
		}
		head_animation_state->setTimePosition(head_animation_time);
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
	ASSERT((index < WEAPON_NUM) && (index >= 0), "Invalid weapon index")
	current_weapon = weapons[index];

	//update pack
}
