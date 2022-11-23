#pragma once
#include "GraphicsInfo.h"
#include "Material.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Platform/GL/Texture/TextureCube.h"

namespace BDXKEngine
{
    class Graphics
    {
    public:
        static void Initialize(Window* window);

        static void UpdateWorldInfo(WorldInfo worldInfo);
        static void UpdateCameraInfo(CameraInfo cameraInfo);
        static void UpdateObjectInfo(ObjectInfo objectInfo);
        static void UpdateLightInfo(LightInfo lightInfo, const ObjectPtr<Texture>& shadowMap);

        static void DrawMeshNow(const ObjectPtr<Mesh>& mesh);
        static void DrawTexture(const ObjectPtr<Texture2D>& texture, Rect screenRect);
        static void Blit(const ObjectPtr<Texture2D>& source, const ObjectPtr<Texture2D>& dest,
                         const ObjectPtr<Material>& material = blitMaterial);
    private:
        static ObjectPtr<Buffer> worldInfoBuffer;
        static ObjectPtr<Buffer> cameraInfoBuffer;
        static ObjectPtr<Buffer> objectInfoBuffer;
        static ObjectPtr<Buffer> lightInfoBuffer;

        static Window* window;
        static ObjectPtr<Mesh> drawTextureMesh;
        static ObjectPtr<Texture2D> defaultTexture2D;
        static ObjectPtr<TextureCube> defaultTextureCube;
        static ObjectPtr<Material> blitMaterial;
    };
}
