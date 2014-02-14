#ifndef __SceneManager_h_
#define __SceneManager_h_

#include "Scene.h"

class : Singleton<SceneManager>
{
public:
    SceneManager(void);
    virtual ~SceneManager(void);

    virtual int addScene(Scene*);
    virtual void removeScene(int);

    virtual Scene* GetScene()

protected:
    
    Scene* _current_scene;
    std::vector<Scene*> _scenes;

};

#endif // #ifndef __SceneManager_h_