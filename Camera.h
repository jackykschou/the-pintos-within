#ifndef __Camera_h_
#define __Camera_h_

#include "common.h"

class Camera : Component
{
	protected:
		Transform* _transform;

	public:
		Ogre::Viewport* view_port;
		Ogre::Camera* camera;

		void Initialize(GameObject gameObject);
		void Update();
		void Finalize();
};

#endif // #ifndef __Camera_h_
