#pragma once
#include "Core/Editor/Core/Editor/EditorMenu.h"
#include "Core/Editor/EditorMenu/GameObjectMenu.h"
#include "Core/Runtime/Framework/Actor/GameObject.h"
#include "Render/Runtime/Graphics/GraphicsInfo.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class RenderGameObjectMenu
    {
    public:
        static ObjectPtr<GameObject> CreateCamera();
        static ObjectPtr<GameObject> CreateLight(LightType lightType, const char* name = "Light");
        static ObjectPtr<GameObject> CreateDirectionalLight();
        static ObjectPtr<GameObject> CreatePointLight();
        static ObjectPtr<GameObject> CreateBlender();
        static ObjectPtr<GameObject> CreateCube();
        static ObjectPtr<GameObject> CreateSphere();
        static ObjectPtr<GameObject> CreatePlane();

    private:
        CustomCreateGameObjectMenu("Render/Camera", CreateCamera)
        CustomCreateGameObjectMenu("Render/Light/Directional Light", CreateDirectionalLight)
        CustomCreateGameObjectMenu("Render/Light/Point Light", CreatePointLight)
        CustomCreateGameObjectMenu("Render/3D Object/Blender", CreateBlender)
        CustomCreateGameObjectMenu("Render/3D Object/Cube", CreateCube)
        CustomCreateGameObjectMenu("Render/3D Object/Sphere", CreateSphere)
        CustomCreateGameObjectMenu("Render/3D Object/Plane", CreatePlane)
    };
}
