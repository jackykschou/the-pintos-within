#ifndef __BoxRigidbody_h_
#define __BoxRigidbody_h_

#include "Rigidbody.h"

class BoxRigidbody : Rigidbody
{
public:
	BoxRigidbody(GameObject*, const btVector3&, float, 
				int, int);
	virtual ~BoxRigidbody();

	virtual void update();
};

#endif // #ifndef __BoxRigidbody_h_