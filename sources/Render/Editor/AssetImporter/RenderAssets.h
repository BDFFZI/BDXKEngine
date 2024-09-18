#pragma once
#include "Core/Editor/AssetImporter/ObjectImporter.h"
#include "Core/Editor/Core/Assets/AssetImporter.h"
#include "Core/Runtime/CoreRuntime.h"
#include "Core/Runtime/Framework/Actor/GameObject.h"
#include "Core/Runtime/Platform/GL/Shader/Shader.h"
#include "Render/Runtime/Renderer/Core/RenderSetting.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class RenderAssets
    {
    public:
        static const ObjectPtr<Shader>& GetStandardShader();
        static const ObjectPtr<Shader>& GetUnlitShader();
        static const ObjectPtr<Shader>& GetShadowMapShader();
        static const ObjectPtr<Shader>& GetSkyboxShader();

        static const ObjectPtr<GameObject>& GetCubeModel();
        static const ObjectPtr<GameObject>& GetSphereModel();
        static const ObjectPtr<GameObject>& GetBlenderModel();

    private:
        inline static ObjectPtr<Shader> standardShader = {};
        inline static ObjectPtr<Shader> unlitShader = {};
        inline static ObjectPtr<Shader> shadowMapShader = {};
        inline static ObjectPtr<Shader> skyboxShader = {};

        inline static ObjectPtr<GameObject> cubeModel = {};
        inline static ObjectPtr<GameObject> sphereModel = {};
        inline static ObjectPtr<GameObject> blenderModel = {};

        static void OnEngineStart();
        CustomEngineStartEvent(OnEngineStart, -5)
    };

    CustomAssetsImporter(material, ObjectImporter)
    CustomAssetsImporter(mesh, ObjectImporter)
}
