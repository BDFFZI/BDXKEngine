#pragma once
#include "Core/Menu.h"
#include "BDXKEngine/Framework/Core/GameObject.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class CreateGameObjectMenu : public Menu<ObjectPtr<GameObject>()>
    {
    };

#define CustomCreateGameObjectMenu(funcName) CustomMenu(ObjectPtr<GameObject>(),funcName)

    ObjectPtr<GameObject> CreateCamera();
    ObjectPtr<GameObject> CreateDirectionalLight();
    ObjectPtr<GameObject> CreatePointLight();
    ObjectPtr<GameObject> CreateCube();
    ObjectPtr<GameObject> CreateSphere();
    ObjectPtr<GameObject> CreatePlane();

    CustomCreateGameObjectMenu(CreateCamera)
    CustomCreateGameObjectMenu(CreateDirectionalLight)
    CustomCreateGameObjectMenu(CreatePointLight)
    CustomCreateGameObjectMenu(CreateCube)
    CustomCreateGameObjectMenu(CreateSphere)
    CustomCreateGameObjectMenu(CreatePlane)
}
