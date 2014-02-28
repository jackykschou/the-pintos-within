#ifndef __BallSpawner_h_
#define __BallSpawner_h_

#include "common.h"

#include "PhysicsManager.h"
#include "GameObject.h"
#include "Ball.h"
#include "GameState.h"

class BallSpawner : GameObject
{
public:
	Transform* _transform;
	float spawn_rate;
	float spawn_change;

	BallSpawner(std::string tag, Scene* scene, 
			int mask, int col_mask, std::string mesh_name,
			float posX, float posY, float posZ, float rate, float chance);

	virtual ~BallSpawner();

	virtual void update();

protected:
	float _timer;

	int _mask;
	int _col_mask;
	std::string _mesh_name;

};



#endif // #ifndef __BallSpawner_h_
