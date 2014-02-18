#ifndef __Mesh_h_
#define __Mesh_h_

#include <OgreSceneNode.h>

#include "common.h"

class Mesh : Component
{
protected:
	Transform* _transform;
	
	static int entity_id_assigner;
	static int node_id_assigner;
public:
	virtual Mesh::Mesh(GameObject*, std::string);
	virtual ~Mesh();

	Ogre::Entity* entity;
  	Ogre::SceneNode* node;

  	virtual void update();
};
int Mesh::entity_id_assigner = 0;
int Mesh::node_id_assigner = 0;

#endif // #ifndef __Mesh_h_