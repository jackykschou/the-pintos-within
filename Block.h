#ifndef __Block_h_
#define __Block_h_

#include "common.h"

#include "GameObject.h"
#include "Mesh.h"
#include "BoxRigidbody.h"

class Block : GameObject
{
public:
	Mesh* mesh;
	BoxRigidbody* rigidbody;

	Block(Scene*, std::string, float, float, float, 
		float, float, float, float,
		float, float, float);
	virtual ~Block();

	virtual void update();
};

#endif // #ifndef __Block_h_