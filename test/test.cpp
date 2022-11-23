#include <clocale>
#include <iostream>
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GL/GL.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "BDXKEngine/Platform/Resource/Resources.h"
#include "BDXKEngine/Function/Graphics/Graphics.h"
#include "BDXKEngine/Platform/GL/Mesh/MeshImporter.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"

using namespace BDXKEngine;

int main()
{
    std::setlocale(LC_ALL, "zh-CN.UTF-8");
    {
        Window window = {L"鸡汤来喽"};

        GL::Initialize(&window);
        GUI::Initialize(&window);

        const ObjectPtr<Texture2D> texture = Resources::Load<Texture2D>("texture.bin");
        const ObjectPtr<Buffer> buffer = Buffer::Create(BufferTarget::Constant, sizeof(Vector4));
        const ObjectPtr<Mesh> mesh = Resources::Load<Mesh>("mesh.bin");
        const ObjectPtr<Shader> shader = Resources::Load<Shader>("shader.bin");
        Texture::ResetDefaultRenderTarget();

        window.AddRenewEvent([&]()
        {
            const Vector4 windowSize = {window.GetSize(), texture->GetSize()};
            buffer->SetData(&windowSize);

            //渲染
            Texture::SetRenderTargetDefault();
            GL::Clear(true, true, Color::blue);
            texture->SetPass(0);
            buffer->SetPass(0);
            mesh->SetPass();
            shader->SetPass();
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
