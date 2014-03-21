#include "PlayerCharacter.h"

#include "Pistol.h"

PlayerCharacter::PlayerCharacter(bool is_yourself_p, Scene* scene, std::string mesh_name,
	float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float rotW,
	float scaleX, float scaleY, float scaleZ) : GameObject("Player", scene)
{
	is_yourself = is_yourself_p;
	is_shooting = false;
	is_idle = true;
	is_dead = false;

	run_animation_time = 0.0;
	shoot_animation_time = 0.0;
	idle_animation_time = 0.0;

	health = 100.0;
	health_regen = 1.0;
	health_regen_rate = 1.0;

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

	shooting_animation_states[0] = mesh->entity->getAnimationState("Shooting");
	shooting_animation_states[0]->setLoop(false);
	shooting_animation_states[0]->setWeight(1);

	// reload_animation_states[0] = mesh->entity->getAnimationState("Reload");
	// reload_animation_states[0]->setLoop(false);
	// reload_animation_states[0]->setWeight(1);

	//load the other weapon animations

	running_animation_state = mesh->entity->getAnimationState("Running");
	running_animation_state->setLoop(true);
	running_animation_state->setWeight(1);

	idle_animation_state = mesh->entity->getAnimationState("Idle");
	idle_animation_state->setLoop(true);
	idle_animation_state->setWeight(1);

	jumping_animation_state = mesh->entity->getAnimationState("Jump");
	jumping_animation_state->setLoop(false);
	jumping_animation_state->setWeight(1);

	// head_animation_state = mesh->entity->getAnimationState("Head");
	// head_animation_state->setLoop(true);
	// head_animation_state->setWeight(1);
	// head_animation_state->setEnable(true);

	// die_animation_state = mesh->entity->getAnimationState("Die");
	// die_animation_state->setLoop(false);
	// die_animation_state->setWeight(1);


    health_regen_debouncer = new Debouncer(health_regen_rate * 1000);
    reload_debouncer = new Debouncer(1);

	if(is_yourself)
  	{
		controller = new FPSBoxController(this, "Camm", box_half_size.y * scaleY + 1, btVector3(box_half_size.x * scaleX, box_half_size.y * scaleY, box_half_size.z * scaleZ), 
											3, COL_CHARACTER, CHARACTER_COLLIDER_WITH, mesh->node);

		scene->main_camera = (Camera*)(controller->fps_camera);
	}

	// <position y="0.000000" x="0.000000" z="-0.000000" />
 //        <rotation qz="-0.000000" qy="0.000000" qx="0.709822" qw="0.704381" />
 //        <scale y="1.000000" x="1.000000" z="1.000000" />

	//hard code position
	// Weapon* pistol = new Pistol(this, "Pistol.mesh", 
	// 		0, 0, 0, 
	// 		0.709822, 0, 0, 0.704381,
	// 		10, 10, 10, 
	// 		new PlayerBox(this, "sphere.mesh",
	// 		0, 0, 0, 0, 0, 0, 1,
	// 		1, 1, 1, COL_NOTHING, COL_NOTHING));


	// weapons[0] = pistol;
	// current_weapon = pistol;

	weapon_running_animation_state = current_weapon->running_animation_state;
	weapon_idle_animation_state = current_weapon->idle_animation_state;
	weapon_shooting_animation_state = current_weapon->shooting_animation_state;
	weapon_reload_animation_state = current_weapon->reload_animation_state;
	weapon_jumping_animation_state = current_weapon->jumping_animation_state;
	weapon_die_animation_state = current_weapon->die_animation_state;

	current_reload_animation_state = reload_animation_states[current_weapon->weapon_id];
	current_shooting_animation_state = shooting_animation_states[current_weapon->weapon_id];

}

PlayerCharacter:: ~PlayerCharacter()
{
	scene->main_camera = NULL;
	delete health_regen_debouncer;
}

void PlayerCharacter::update()
{
	GameObject::update();

	Transform* tran = this->getComponent<Transform>();

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
			//play dead animation
		}
		else
		{
			// if(InputManager::instance()->isMouseLeftClicked() && current_weapon)
			// {
			// 	current_weapon->shoot();
			// }

			if(InputManager::instance()->isMouseLeftClicked())
			{
				is_shooting = true;
				current_shooting_animation_state->setTimePosition(0);
				current_weapon->shooting_animation_state->setTimePosition(0);
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
				is_jumping = true;
				jumping_animation_state->setWeight(1);
				jumping_animation_state->setEnabled(true);
				jumping_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

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

				weapon_jumping_animation_state->setWeight(0);
				weapon_jumping_animation_state->setEnabled(false);
				weapon_jumping_animation_state->setTimePosition(0);
			}

			if(controller->is_walking && !controller->is_jet_packing && controller->controller->onGround())
			{
				running_animation_state->setWeight(1);
				running_animation_state->setEnabled(true);

				if(controller->is_running)
					running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame * 1.3f);
				else
					running_animation_state->addTime(GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);

				weapon_running_animation_state->setWeight(1);
				weapon_running_animation_state->setEnabled(true);
				weapon_running_animation_state->setTimePosition(running_animation_state->getTimePosition());

				is_moving = true;
			}
			else
			{
				running_animation_state->setWeight(0);
				running_animation_state->setEnabled(false);
				running_animation_state->setTimePosition(0);

				weapon_running_animation_state->setWeight(0);
				weapon_running_animation_state->setEnabled(false);
				weapon_running_animation_state->setTimePosition(0);

				is_moving = false;
			}

			if(current_weapon->is_shooting && !current_shooting_animation_state->hasEnded() && !weapon_shooting_animation_state->hasEnded())
			{
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

			head_animation_state->setTimePosition();

		}
		run_animation_time = running_animation_state->getTimePosition();
		shoot_animation_time = current_shooting_animation_state->getTimePosition();
		idle_animation_time = idle_animation_state->getTimePosition();
		reload_animation_time = current_reload_animation_state->getTimePosition();
		jump_animation_time = jumping_animation_state->getTimePosition();
		head_animation_time = head_animation_state->getTimePosition();
		die_animation_time = die_animation_state->getTimePosition();

		//sent info to sever
	}
	else
	{
		//get info from server

		
	}
}

void PlayerCharacter::regen_health()
{
	if(!is_dead)
	{
		health = ((health + health_regen) >= 100.0) ? (100.0) : (health + health_regen); 
	}
}

