#include "Graphics.h"

namespace BDXKEngine
{
    Vector3 ScreenToViewPos(Vector2 screenSize, Vector2 screenPos)
    {
        screenPos.y = screenSize.y - screenPos.y;
        screenPos = screenPos - screenSize / 2;
        return {screenPos.x, screenPos.y, 0};
    }

    ObjectPtr<Buffer> Graphics::worldInfoBuffer = nullptr;
    ObjectPtr<Buffer> Graphics::cameraInfoBuffer = nullptr;
    ObjectPtr<Buffer> Graphics::lightInfoBuffer = nullptr;
    ObjectPtr<Buffer> Graphics::objectInfoBuffer = nullptr;
    ObjectPtr<Buffer> Graphics::shadowInfoBuffer = nullptr;

    Window* Graphics::window = nullptr;
    ObjectPtr<Mesh> Graphics::drawTextureMesh = nullptr;
    ObjectPtr<Texture> Graphics::defaultTexture2D = nullptr;
    ObjectPtr<Texture> Graphics::defaultTextureCube = nullptr;
    
    void Graphics::UpdateWorldInfo(WorldInfo worldInfo)
    {
        worldInfoBuffer->SetData(reinterpret_cast<char*>(&worldInfo));
    }
    void Graphics::UpdateCameraInfo(CameraInfo cameraInfo)
    {
        cameraInfoBuffer->SetData(reinterpret_cast<char*>(&cameraInfo));
    }
    void Graphics::UpdateObjectInfo(ObjectInfo objectInfo)
    {
        objectInfoBuffer->SetData(reinterpret_cast<char*>(&objectInfo));
    }
    void Graphics::UpdateLightInfo(LightInfo lightInfo, ShadowInfo shadowInfo, const ObjectPtr<Texture>& shadowMap)
    {
        lightInfoBuffer->SetData(reinterpret_cast<char*>(&lightInfo));
        shadowInfoBuffer->SetData(reinterpret_cast<char*>(&shadowInfo));
        if (lightInfo.lightType == LightType::Point)
        {
            GL::SetTexture(4, defaultTexture2D);
            GL::SetTexture(5, shadowMap);
        }
        else
        {
            GL::SetTexture(4, shadowMap);
            GL::SetTexture(5, defaultTextureCube);
        }
    }

    void Graphics::DrawMeshNow(const ObjectPtr<Mesh>& mesh)
    {
        GL::SetMesh(mesh);
        GL::Render(mesh->GetTrianglesCount());
    }
    void Graphics::DrawTexture(ObjectPtr<Texture2D> texture, Rect screenRect)
    {
        const ObjectPtr<Texture2D> renderTarget = GL::GetRenderTarget();
        Vector2 screenSize;
        if (renderTarget.IsNull() == false)
            screenSize = {static_cast<float>(renderTarget->GetWidth()), static_cast<float>(renderTarget->GetHeight())};
        else
            screenSize = window->GetSize();
        Vector3 minPosition = ScreenToViewPos(screenSize, screenRect.GetMin());
        Vector3 maxPosition = ScreenToViewPos(screenSize, screenRect.GetMax());

        drawTextureMesh->SetPositions({
            minPosition,
            {maxPosition.x, minPosition.y, 0},
            maxPosition,
            {minPosition.x, maxPosition.y, 0}
        });
        drawTextureMesh->UploadMeshData();

        UpdateObjectInfo({Matrix4x4::identity});
        UpdateCameraInfo(CameraInfo::Orthographic(
            Vector3::zero,
            Matrix4x4::identity,
            screenSize.x / screenSize.y,
            0, 1, screenSize.y
        ));
        GL::SetTexture(0, texture.ToObjectPtr<Texture>());

        DrawMeshNow(drawTextureMesh);
    }
    void Graphics::Blit(const ObjectPtr<Texture2D>& source, const ObjectPtr<Texture2D>& dest, const ObjectPtr<Material>& blitMaterial)
    {
        GL::SetRenderTarget(dest);
        blitMaterial->SetPass(0);
        DrawTexture(source, {0, 0, static_cast<float>(source->GetWidth()), static_cast<float>(source->GetHeight())});
        GL::SetRenderTarget(nullptr);
    }

    void Graphics::Initialize(Window* window)
    {
        //创建渲染用的通用常量缓冲区
        worldInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(WorldInfo));
        cameraInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(CameraInfo));
        objectInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(ObjectInfo));
        lightInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(LightInfo));
        shadowInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(ShadowInfo));
        //设置缓冲区
        GL::SetBuffer(1, worldInfoBuffer);
        GL::SetBuffer(2, cameraInfoBuffer);
        GL::SetBuffer(3, objectInfoBuffer);
        GL::SetBuffer(4, lightInfoBuffer);
        GL::SetBuffer(5, shadowInfoBuffer);

        defaultTexture2D = Texture2D::Create(1, 1).ToObjectPtr<Texture>();
        defaultTextureCube = TextureCube::Create(1, 1).ToObjectPtr<Texture>();
        drawTextureMesh = Mesh::Create();
        drawTextureMesh->SetTriangles({
            0, 1, 3,
            3, 1, 2
        });
        drawTextureMesh->SetUVs({
            {0, 1},
            {1, 1},
            {1, 0},
            {0, 0},
        });

        Graphics::window = window;
        window->AddDestroyEvent([]()
        {
            drawTextureMesh = nullptr;
            defaultTexture2D = nullptr;
            defaultTextureCube = nullptr;
            worldInfoBuffer = nullptr;
            cameraInfoBuffer = nullptr;
            lightInfoBuffer = nullptr;
            objectInfoBuffer = nullptr;
            shadowInfoBuffer = nullptr;
        });
    }
}
