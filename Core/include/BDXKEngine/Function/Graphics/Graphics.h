#pragma once
#include "GraphicsInfo.h"
#include "Material.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Platform/GL/Texture/TextureCube.h"
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine
{
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
    private:
        static ObjectPtr<Buffer> cameraInfoBuffer;
        static ObjectPtr<Buffer> lightInfoBuffer;

        static ObjectPtr<Mesh> drawTextureMesh;
        static ObjectPtr<Texture2D> defaultTexture2D;
        static ObjectPtr<TextureCube> defaultTextureCube;
    };
}
