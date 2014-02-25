#ifndef __SphereRigidbody_h_
#define __SphereRigidbody_h_

#include "Rigidbody.h"

class SphereRigidbody : Rigidbody
{
public:
	SphereRigidbody(GameObject*, float, float, 
					int, int);
	virtual ~SphereRigidbody();

	virtual void update();

};

#endif // #ifndef __SphereRigidbody_h_