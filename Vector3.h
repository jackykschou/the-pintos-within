#ifndef __Vector3_h_
#define __Vector3_h_

#include "common.h"

class Vector3
{
public:
	float x;
	float y;
	float z;
	Vector3(float a, float b, float c) : x(a), y(b), z(c) {};
	~Vector3();

	/**Overloaded Ogre Vector Cast*/
	operator Ogre::Vector3() {
		return Ogre::Vector3 (x, y, z);
	}

	/**Overloaded Bullet Vector Cast*/
	operator btVector3() {
		return btVector3 (x, y, z);
	}

	/* data */
};

#endif
