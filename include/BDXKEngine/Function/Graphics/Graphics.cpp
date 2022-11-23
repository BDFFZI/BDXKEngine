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
    ObjectPtr<Buffer> Graphics::objectInfoBuffer = nullptr;
    ObjectPtr<Buffer> Graphics::lightInfoBuffer = nullptr;

    Window* Graphics::window = nullptr;
    ObjectPtr<Mesh> Graphics::drawTextureMesh = nullptr;
    ObjectPtr<Texture2D> Graphics::defaultTexture2D = nullptr;
    ObjectPtr<TextureCube> Graphics::defaultTextureCube = nullptr;

    void Graphics::UpdateWorldInfo(WorldInfo worldInfo)
    {
        worldInfoBuffer->SetData(&worldInfo);
    }
    void Graphics::UpdateCameraInfo(CameraInfo cameraInfo)
    {
        cameraInfoBuffer->SetData(&cameraInfo);
    }
    void Graphics::UpdateObjectInfo(ObjectInfo objectInfo)
    {
        objectInfoBuffer->SetData(&objectInfo);
    }
    void Graphics::UpdateLightInfo(LightInfo lightInfo, const ObjectPtr<Texture>& shadowMap)
    {
        lightInfoBuffer->SetData(&lightInfo);
        if (lightInfo.lightType == LightType::Point)
        {
            defaultTexture2D->SetPass(5);
            shadowMap->SetPass(6);
        }
        else
        {
            shadowMap->SetPass(5);
            defaultTextureCube->SetPass(6);
        }
    }

    void Graphics::DrawMeshNow(const ObjectPtr<Mesh>& mesh)
    {
        mesh->SetPass();
        GL::Render(mesh->GetTrianglesCount());
    }
    void Graphics::DrawTexture(const ObjectPtr<Texture2D>& texture, Rect screenRect)
    {
        const ObjectPtr<Texture> renderTarget = Texture::GetRenderTarget();
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
        texture->SetPass(0);

        DrawMeshNow(drawTextureMesh);
    }
    void Graphics::Blit(const ObjectPtr<Texture2D>& source, const ObjectPtr<Texture2D>& dest, const ObjectPtr<Material>& blitMaterial)
    {
        dest->SetRenderTarget();
        blitMaterial->SetPass(0);
        DrawTexture(source, {0, 0, static_cast<float>(source->GetWidth()), static_cast<float>(source->GetHeight())});
        Texture::SetRenderTargetDefault();
    }

    void Graphics::Initialize(Window* window)
    {
        //创建渲染用的通用常量缓冲区
        worldInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(WorldInfo));
        cameraInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(CameraInfo));
        lightInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(LightInfo));
        objectInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(ObjectInfo));
        //设置缓冲区
        worldInfoBuffer->SetPass(1);
        cameraInfoBuffer->SetPass(2);
        lightInfoBuffer->SetPass(4);
        objectInfoBuffer->SetPass(3);

        defaultTexture2D = Texture2D::Create(1, 1);
        defaultTextureCube = TextureCube::Create(1, 1);
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
        });
    }
}
