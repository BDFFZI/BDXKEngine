#include "Graphics.h"

namespace BDXKEngine
{
    Vector3 ScreenToViewPos(Vector2 screenSize, Vector2 screenPos)
    {
        screenPos.y = screenSize.y - screenPos.y;
        screenPos = screenPos - screenSize / 2;
        return {screenPos.x, screenPos.y, 0};
    }

    ObjectPtr<Buffer> Graphics::cameraInfoBuffer = nullptr;
    ObjectPtr<Buffer> Graphics::lightInfoBuffer = nullptr;

    Window* Graphics::window = nullptr;
    ObjectPtr<Mesh> Graphics::drawTextureMesh = nullptr;
    ObjectPtr<Texture2D> Graphics::defaultTexture2D = nullptr;
    ObjectPtr<TextureCube> Graphics::defaultTextureCube = nullptr;

    void Graphics::SetCameraInfo(CameraInfo cameraInfo, const ObjectPtr<TextureCube>& skybox)
    {
        cameraInfoBuffer->SetData(&cameraInfo);
        if (skybox.IsNotNull()) skybox->UploadRP(4);
        else defaultTextureCube->UploadRP(4);
    }
    void Graphics::SetCameraInfoNull()
    {
        defaultTextureCube->UploadRP(4);
    }
    void Graphics::SetLightInfo(LightInfo lightInfo, const ObjectPtr<Texture>& shadowMap)
    {
        lightInfoBuffer->SetData(&lightInfo);
        if (shadowMap.IsNull())
        {
            defaultTexture2D->UploadRP(5);
            defaultTextureCube->UploadRP(6);
        }
        else
        {
            if (lightInfo.lightType == LightType::Point)
            {
                defaultTexture2D->UploadRP(5);
                shadowMap->UploadRP(6);
            }
            else
            {
                shadowMap->UploadRP(5);
                defaultTextureCube->UploadRP(6);
            }
        }
    }
    void Graphics::SetLightInfoNull()
    {
        defaultTexture2D->UploadRP(5);
        defaultTextureCube->UploadRP(6);
    }

    void Graphics::DrawMesh(const ObjectPtr<Mesh>& mesh)
    {
        mesh->UploadRP();
        GL::Render(mesh->GetTrianglesCount());
    }
    void Graphics::DrawRect(Rect screenRect)
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
        drawTextureMesh->UpdateGL();

        DrawMesh(drawTextureMesh);
    }
    void Graphics::DrawViewport(bool setDefaultCameraInfo)
    {
        if (setDefaultCameraInfo)
            SetCameraInfo(
                {
                    Matrix4x4::identity, Matrix4x4::identity,
                    Vector4{}, Color{}, Vector4{},
                },
                nullptr
            );

        drawTextureMesh->SetPositions({
            {-1, 1, 0},
            {1, 1, 0},
            {1, -1, 0},
            {-1, -1, 0},
        });
        drawTextureMesh->UpdateGL();

        DrawMesh(drawTextureMesh);
    }
    void Graphics::Blit(const ObjectPtr<Texture2D>& source, const ObjectPtr<Texture2D>& dest, const ObjectPtr<Material>& blitMaterial)
    {
        dest->SetRenderTarget();
        blitMaterial->SetMatrix(0, Matrix4x4::identity);
        blitMaterial->SetTexture2D(0, source);
        blitMaterial->UploadRP(0);

        const Vector2 sourceSize = {source->GetWidth(), source->GetHeight()};
        SetCameraInfo(
            CameraInfo::Orthographic(
                sourceSize.x / sourceSize.y, 0, 1, sourceSize.y,
                Matrix4x4::identity, Vector3::zero, Color::black, 0
            ), nullptr
        );
        DrawRect({Vector2::zero, sourceSize});

        Material::UploadRPNull();
        Texture::SetRenderTargetDefault();
    }

    void Graphics::Initialize(Window* window)
    {
        //创建渲染用的通用常量缓冲区
        cameraInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(CameraInfo));
        lightInfoBuffer = Buffer::Create(BufferTarget::Constant, sizeof(LightInfo));
        //设置缓冲区
        cameraInfoBuffer->UploadRP(1);
        lightInfoBuffer->UploadRP(2);

        defaultTexture2D = Texture2D::Create(1, 1, TextureFormat::B8G8R8A8_UNORM);
        defaultTextureCube = TextureCube::Create(1, 1, TextureFormat::B8G8R8A8_UNORM);
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
            cameraInfoBuffer = nullptr;
            lightInfoBuffer = nullptr;
        });
    }
}
