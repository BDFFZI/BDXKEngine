#pragma once
#include "GraphicsInfo.h"
#include "Mesh.h"
#include "Core/Runtime/Base/Data/Graphics/Rect.h"
#include "Core/Runtime/Platform/GL/Texture/Texture2D.h"
#include "Core/Runtime/Platform/GL/Texture/TextureCube.h"

namespace BDXKEngine
{
    class Material;
    class Graphics
    {
    public:
        static void Initialize();

        static void SetCameraInfo(CameraInfo cameraInfo);
        static void SetLightInfo(LightInfo lightInfo, const ObjectPtr<Texture>& shadowMap);
        static void SetCameraInfoNull();
        static void SetLightInfoNull();

        static void DrawMesh(const ObjectPtr<Mesh>& mesh);
        static void DrawRect(Rect screenRect);
        static void DrawViewport(bool setDefaultCameraInfo);
        static void Blit(const ObjectPtr<Texture2D>& source, const ObjectPtr<Texture2D>& dest, const ObjectPtr<Material>& material);

        static const ObjectPtr<Texture2D>& GetWhiteTexture();
        static const ObjectPtr<Texture2D>& GetBlueTexture();

    private:
        inline static ObjectPtr<Buffer> cameraInfoBuffer;
        inline static ObjectPtr<Buffer> lightInfoBuffer;
        inline static ObjectPtr<Mesh> drawTextureMesh;
        inline static ObjectPtr<Texture2D> defaultTexture2D;
        inline static ObjectPtr<TextureCube> defaultTextureCube;
        inline static ObjectPtr<Texture2D> whiteTexture2D;
        inline static ObjectPtr<Texture2D> blueTexture2D;
    };
}

#include "Material.h"
