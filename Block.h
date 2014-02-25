#ifndef __Block_h_
#define __Block_h_

#include "GameObject.h"
#include "Mesh.h"

class Block : GameObject
{
public:
	Mesh block_mesh;

	Block();
	virtual Block();

};

#endif // #ifndef __Block_h_