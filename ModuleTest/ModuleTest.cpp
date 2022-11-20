#include <clocale>

#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GL/GL.h"
#include "BDXKEngine/Platform/GL/Mesh/MeshImporter.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"

using namespace BDXKEngine;

int main()
{
    std::setlocale(LC_ALL, "zh-CN.UTF-8");

    {
        Window window = {L"鸡汤来喽"};

        GL::Initialize(&window);

        const ObjectPtr<Shader> shader = ShaderImporter::Import("C:/Users/BDFFZI/Desktop/Test.hlsl");
        const ObjectPtr<Mesh> mesh = MeshImporter::Import("C:/Users/BDFFZI/Desktop/Blender.glb");
        const ObjectPtr<Buffer> buffer = Buffer::Create(BufferTarget::Constant, sizeof(Vector4));

        shader->SetCull(Cull{Cull::State::Off});

        window.AddRenewEvent([&]()
        {
            const Vector2 windowSize = window.GetSize();
            buffer->SetData(&windowSize);
            buffer->SetPass(0);

            GL::SetDefaultRenderTarget();

            GL::Clear(true, true, Color::blue);
            shader->SetPass();
            mesh->SetPass();
            GL::Render(mesh->GetTrianglesCount());
        });

        window.Show();
    }

    auto b = Object::GetAllObjects();
    ObjectPtrBase::PrintRefCountMap();

    getchar();
}
