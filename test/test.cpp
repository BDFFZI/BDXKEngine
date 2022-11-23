#include <clocale>
#include <iostream>
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GL/GL.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "BDXKEngine/Function/Graphics/Graphics.h"
#include "BDXKEngine/Platform/GL/Mesh/MeshImporter.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"
#include "BDXKEngine/Platform/Resources/Resources.h"

using namespace BDXKEngine;

int main()
{
    std::setlocale(LC_ALL, "zh-CN.UTF-8");
    {
        Window window = {L"鸡汤来喽"};

        GL::Initialize(&window);
        GUI::Initialize(&window);

        //const ObjectPtr<Buffer> buffer = Buffer::Create(BufferTarget::Constant, sizeof(Vector4));
        const ObjectPtr<Texture2D> texture = Resources::Load<Texture2D>("texture.bin");
        const ObjectPtr<Mesh> mesh = Resources::Load<Mesh>("mesh.bin");
        const ObjectPtr<Shader> shader = Resources::Load<Shader>("shader.bin");
        const ObjectPtr<Material> material = Resources::Load<Material>("material.json", Resources::GetJsonSerializer());
        // const ObjectPtr<Material> material = Material::Create({
        //     {shader, ShaderType::ForwardBase}
        // });
        // material->SetTexture2D(0, texture);
        // Resources::Save("material.json", material, Resources::GetJsonSerializer());
        Texture::ResetDefaultRenderTarget();

        window.AddRenewEvent([&]()
        {
            const Vector4 windowSize = {window.GetSize(), texture->GetSize()};
            // buffer->SetData(&windowSize);

            material->SetFloat(0, windowSize.x);
            material->SetFloat(1, windowSize.y);
            material->SetFloat(2, windowSize.z);
            material->SetFloat(3, windowSize.w);

            //渲染
            Texture::SetRenderTargetDefault();
            GL::Clear(true, true, Color::blue);
            material->SetPass(0);
            mesh->SetPass();
            GL::Render(mesh->GetTrianglesCount());

            //UI
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
