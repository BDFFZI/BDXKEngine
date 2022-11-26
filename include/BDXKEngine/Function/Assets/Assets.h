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
        static ObjectPtr<Material> GetUnlitMaterial();
        static ObjectPtr<Material> GetStandardMaterial();
        static ObjectPtr<Material> GetSkyboxMaterial();
        static ObjectPtr<Mesh> GetCubeMesh();
        static ObjectPtr<Mesh> GetSphereMesh();
        static ObjectPtr<Mesh> GetBlenderMesh();
        static ObjectPtr<Texture2D> GetWhiteTexture();
    protected:
        static ObjectPtr<Material> unlitMaterial;
        static ObjectPtr<Material> standardMaterial;
        static ObjectPtr<Material> skyboxMaterial;
        static ObjectPtr<Mesh> cubeMesh;
        static ObjectPtr<Mesh> sphereMesh;
        static ObjectPtr<Mesh> blenderMesh;
        static ObjectPtr<Texture2D> whiteTexture;
    };
}
