#include <clocale>
#include <iostream>

#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GL/GL.h"
#include "BDXKEngine/Platform/GL/Mesh/MeshImporter.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "BDXKEngine/Platform/Resource/Resource.h"
#include "BDXKEngine/Function/Time.h"
#include "BDXKEngine/Function/Window/Cursor.h"
#include "BDXKEngine/Function/Window/Input.h"
#include "BDXKEngine/Function/Window/Screen.h"

using namespace BDXKEngine;

class Material : public Object
{
public:
    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(texture);
        TransferFieldInfo(shader);
        TransferFieldInfo(mesh);
    }

    ObjectPtr<Texture2D> texture;
    ObjectPtr<Shader> shader;
    ObjectPtr<Mesh> mesh;
};

CustomReflection(Material)

int main()
{
    std::setlocale(LC_ALL, "zh-CN.UTF-8");

    {
        Window window = {L"鸡汤来喽"};

        GL::Initialize(&window);
        GUI::Initialize(&window);
        // Time::Initialize(&window);
        // Cursor::Initialize(&window);
        // Input::Initialize(&window);
        // Cursor::Initialize(&window);
        // Screen::Initialize(&window);
        // Graphics::Initialize(&window);
        Texture::ResetDefaultRenderTarget();

        // ObjectPtr<Material> material = Object::Instantiate<Material>(new Material());
        // material->texture = TextureImporter::Import("C:/Users/BDFFZI/Desktop/KeQing.png");
        // material->shader = ShaderImporter::Import("C:/Users/BDFFZI/Desktop/Test.hlsl");
        // material->mesh = MeshImporter::Import("C:/Users/BDFFZI/Desktop/Blender.glb");
        // material->shader->SetCull(Cull{Cull::State::Off});
        // Resource::Save("shader.bin", material->shader);
        // Resource::Save("texture.bin", material->texture);
        // Resource::Save("mesh.bin", material->mesh);
        // Resource::Save("material.bin", material);

        const ObjectPtr<Shader> shader = Resource::Load<Shader>("shader.bin");
        const ObjectPtr<Mesh> mesh = Resource::Load<Mesh>("mesh.bin");
        const ObjectPtr<Texture2D> texture = Resource::Load<Texture2D>("texture.bin");
        const ObjectPtr<Material> material = Resource::Load<Material>("material.bin");
        const ObjectPtr<Buffer> buffer = Buffer::Create(BufferTarget::Constant, sizeof(Vector4));


        window.AddRenewEvent([&]()
        {
            const Vector4 windowSize = {window.GetSize(), material->texture->GetSize()};
            buffer->SetData(&windowSize);
            buffer->SetPass(0);

            Texture::SetRenderTargetDefault();

            GL::Clear(true, true, Color::blue);
            material->shader->SetPass();
            material->mesh->SetPass();
            material->texture->SetPass(0);
            GL::Render(material->mesh->GetTrianglesCount());


            GUI::BeginDraw();

            ImGui::ShowDemoWindow();

            GUI::EndDraw();
            GL::Present();
        });

        window.Show();
    }

    auto b = Object::GetAllObjects();
    ObjectPtrBase::PrintRefCountMap();

    getchar();
}
