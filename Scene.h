#ifndef __Scene_h_
#define __Scene_h_

#include "GameObject.h"

#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>

class Scene
{
protected:
		std::vector<GameObject*> _game_objects;
		std::vector<Camera*> _cameras;
		Ogre::SceneManager* _manager;
		btDiscreteDynamicsWorld* _physics_world;

		unsigned int _game_object_id_assigner;
public:
		Scene();

		virtual void addGameObject(GameObject);
		virtual void getGameObject(unsigned int);
		virtual void getManager();
		virtual void addCamera(Camera);
		virtual void getCamera(unsigned int);
		virtual void UpdateScene();

};

#endif // #ifndef __Scene_h_

