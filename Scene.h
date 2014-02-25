#ifndef __Scene_h_
#define __Scene_h_

#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>

#include "common.h"

#include "GraphicsManager.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "Component.h"
#include "GameObject.h"

class Camera;
class Transform;
class GameObject;

class Scene
{
protected:
		int _game_object_id_assigner;
public:
		int id;
		std::string name;
		btDiscreteDynamicsWorld* physics_world;
		Ogre::SceneManager* manager;
		std::vector<GameObject*> game_objects;
		Camera* main_camera;

		Scene(std::string);
		virtual ~Scene();

		void addGameObject(GameObject*);
		void removeGameObject(GameObject*);
		bool containsGameObject(GameObject*);
		std::vector<GameObject*> getGameObjectsOfTag(std::string);
		void update(float);
};

#endif // #ifndef __Scene_h_

