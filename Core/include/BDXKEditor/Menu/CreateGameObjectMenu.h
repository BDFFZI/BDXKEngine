#pragma once
#include "Core/Menu.h"
#include "BDXKEngine/Framework/Core/GameObject.h"
#include "BDXKEngine/Function/Graphics/GraphicsInfo.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

#define CustomCreateGameObjectMenu(funcName) CustomMenu(ObjectPtr<GameObject>(),funcName)

    class CreateGameObjectMenu : public Menu<ObjectPtr<GameObject>()>
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
    private:
        CustomCreateGameObjectMenu(CreateGameObject)
        CustomCreateGameObjectMenu(CreateCamera)
        CustomCreateGameObjectMenu(CreateDirectionalLight)
        CustomCreateGameObjectMenu(CreatePointLight)
        CustomCreateGameObjectMenu(CreateCube)
        CustomCreateGameObjectMenu(CreateSphere)
        CustomCreateGameObjectMenu(CreatePlane)
    };
}
