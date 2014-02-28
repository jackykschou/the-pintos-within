#include "BallSpawner.h"

#define MAX_BALL_SCALE 0.1
#define MAX_BALL_FORCE 10
#define SPAWNOFFSET 200

BallSpawner::BallSpawner(std::string tag, Scene* scene, 
			int mask, int col_mask, std::string mesh_name,
			float posX, float posY, float posZ, float rate, float chance) : GameObject(tag, scene)
{
	_transform = this->getComponent<Transform>();
	_transform->posX = posX;
	_transform->posY = posY;
	_transform->posZ = posZ;
	_timer = rate;

	spawn_rate = rate;
	spawn_change = chance;

	_mesh_name = mesh_name;
	_mask = mask;
	_col_mask = col_mask;
}

BallSpawner::~BallSpawner()
{
}

void BallSpawner::update()
{
	GameObject::update();

	if(_timer <= 0.0f)
	{
		_timer = spawn_rate;
		if(CHANCE(spawn_change))
		{
			float scale = RAND * MAX_BALL_SCALE;
		new Ball("Ball", scene, 
				_mask, _col_mask, _mesh_name,
				_transform->posX + (SPAWNOFFSET * RAND) * (CHANCE(0.5) ? 1 : -1), 
				_transform->posY + 80, 
				_transform->posZ + (SPAWNOFFSET * RAND) * (CHANCE(0.5) ? 1 : -1), 0, 0, 0, 1,
				scale, scale, scale, 
				btVector3(RAND * MAX_BALL_FORCE, RAND * MAX_BALL_FORCE, RAND * MAX_BALL_FORCE));
		}
		_timer = spawn_rate;
	}

	if(GraphicsManager::instance()->getFrameEvent() != NULL)
		_timer -= (GraphicsManager::instance()->getFrameEvent()->timeSinceLastFrame);
}