#ifndef __GameObject_h_
#define __GameObject_h_

#include "Component.h"

class GameObject
{
public:

	bool active;
	// AudioSource audio;
	// Collider collider;
	Vector3 constantForce;
	int layer;
	// Renderer renderer;
	// Rigidbody rigidbody;
	string tag;
	// Transform transform;
	// std::vector<Component> components;

	GameObject();

	~GameObject();

	// Component 

	/* data */
};

#endif
