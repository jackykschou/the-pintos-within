#ifndef __SceneManager_h_
#define __SceneManager_h_

#include "common.h"
#include "Scene.h"
#include "Singleton.h"
#include "DotSceneLoader.h"

class Scene;

class SceneManager: public Singleton<SceneManager>
{
public:
	Scene* current_scene;
    uint32_t current_scene_mode;

   ~SceneManager();

	void initialize();    
    int addScene(Scene*);
    void changeCurrentScene(uint32_t);
    void updateScene(float);

protected:
    int _scene_id_assigner;
    DotSceneLoader scene_loader;
};

#endif // #ifndef __SceneManager_h_