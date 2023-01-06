#pragma once
#include "BDXKEngine/Function/Graphics/Material.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class AssetsBuiltIn
    {
    public:
        static ObjectPtr<Mesh>& GetCubeMesh();
        static ObjectPtr<Mesh>& GetSphereMesh();
        static ObjectPtr<Mesh>& GetBlenderMesh();
        static ObjectPtr<Shader>& GetStandardShader();
        static ObjectPtr<Shader>& GetUnlitShader();
        static ObjectPtr<Shader>& GetShadowMapShader();
        static ObjectPtr<Shader>& GetSkyboxShader();

        static void Initialize();
    private:
        static ObjectPtr<Mesh> cubeMesh;
        static ObjectPtr<Mesh> sphereMesh;
        static ObjectPtr<Mesh> blenderMesh;
        static ObjectPtr<Shader> standardShader;
        static ObjectPtr<Shader> unlitShader;
        static ObjectPtr<Shader> shadowMapShader;
        static ObjectPtr<Shader> skyboxShader;
    };
}
