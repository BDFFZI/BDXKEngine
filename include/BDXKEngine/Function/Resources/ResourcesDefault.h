#pragma once
#include "BDXKEngine/Base/Object/Core/ObjectPtr.h"
#include "BDXKEngine/Function/Graphics/Material.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"

namespace BDXKEngine
{
    class ResourcesDefault
    {
    public:
        static ObjectPtr<Material> GetUnlitMaterial();
        static ObjectPtr<Material> GetStandardMaterial();
        static ObjectPtr<Material> GetSkyboxMaterial();
        static ObjectPtr<Mesh> GetCubeMesh();
        static ObjectPtr<Mesh> GetSphereMesh();
        static ObjectPtr<Mesh> GetBlenderMesh();
        static ObjectPtr<Texture2D> GetWhiteTexture();
        
        static void Initialize();
    private:
        static ObjectPtr<Material> unlitMaterial;
        static ObjectPtr<Material> standardMaterial;
        static ObjectPtr<Material> skyboxMaterial;
        static ObjectPtr<Mesh> cubeMesh;
        static ObjectPtr<Mesh> sphereMesh;
        static ObjectPtr<Mesh> blenderMesh;
        static ObjectPtr<Texture2D> whiteTexture;
    };
}
