#include "CreateGameObjectMenu.h"

#include "BDXKEngine/Framework/Scene.h"

namespace BDXKEditor
{
    ObjectPtr<GameObject> CreateCamera()
    {
        return SceneDefault::CreateCamera();
    }
    ObjectPtr<GameObject> CreateDirectionalLight()
    {
        return SceneDefault::CreateDirectionalLight();
    }
    ObjectPtr<GameObject> CreatePointLight()
    {
        return SceneDefault::CreatePointLight();
    }
    ObjectPtr<GameObject> CreateCube()
    {
        return SceneDefault::CreateCube();
    }
    ObjectPtr<GameObject> CreateSphere()
    {
        return SceneDefault::CreateSphere();
    }
    ObjectPtr<GameObject> CreatePlane()
    {
        return SceneDefault::CreatePlane();
    }
}
