#pragma once
#include "BDXKEngine/Framework/Core/GameObject.h"
#include "Core/Menu.h"
#include "BDXKEngine/Function/Graphics/GraphicsInfo.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

#define CustomCreateGameObjectMenu(menuItemName,funcName) CustomMenu("GameObject/Create/" menuItemName,funcName)

    class GameObjectMenu
    {
    public:
        static ObjectPtr<GameObject> CreateGameObject();
        static ObjectPtr<GameObject> CreateCamera();
        static ObjectPtr<GameObject> CreateLight(LightType lightType, const char* name = "Light");
        static ObjectPtr<GameObject> CreateDirectionalLight();
        static ObjectPtr<GameObject> CreatePointLight();
        static ObjectPtr<GameObject> CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name, Color color = Color::white);
        static ObjectPtr<GameObject> CreateCube();
        static ObjectPtr<GameObject> CreateSphere();
        static ObjectPtr<GameObject> CreatePlane();

        static void ClearParent();
    private:
        CustomCreateGameObjectMenu("Empty", CreateGameObject)
        CustomCreateGameObjectMenu("Camera", CreateCamera)
        CustomCreateGameObjectMenu("Light/Directional Light", CreateDirectionalLight)
        CustomCreateGameObjectMenu("Light/Point Light", CreatePointLight)
        CustomCreateGameObjectMenu("3D Object/Cube", CreateCube)
        CustomCreateGameObjectMenu("3D Object/Sphere", CreateSphere)
        CustomCreateGameObjectMenu("3D Object/Plane", CreatePlane)
        CustomMenu("GameObject/Clear Parent", ClearParent)
    };
}
