#pragma once

#include "BDXKEngine/Function/Graphics/Material.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine
{
    class Assets
    {
    public:
        static void Initialize(Window* window);
        static ObjectPtr<Material> GetShadowMapMaterial();
        static ObjectPtr<Material> GetSkyboxMaterial();
        static ObjectPtr<Material> GetBlitMaterial();
        static ObjectPtr<Texture2D> GetWhiteTexture();
        static ObjectPtr<Mesh> GetCubeMesh();
        static ObjectPtr<Mesh> GetSphereMesh();
        static ObjectPtr<Mesh> GetBlenderMesh();
    protected:
        static ObjectPtr<Material> shadowMapMaterial;
        static ObjectPtr<Material> skyboxMaterial;
        static ObjectPtr<Material> blitMaterial;
        static ObjectPtr<Texture2D> whiteTexture;
        static ObjectPtr<Mesh> cubeMesh;
        static ObjectPtr<Mesh> sphereMesh;
        static ObjectPtr<Mesh> blenderMesh;
    };
}
