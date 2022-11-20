#include <clocale>

#include "BDXKEngine/Base/BDXKObject/BDXKObject.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GL/GL.h"
#include "BDXKEngine/Platform/GL/Mesh/MeshImporter.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"
#include "BDXKEngine/Platform/GL/Texture/TextureImporter.h"

using namespace BDXKEngine;

int main()
{
    std::setlocale(LC_ALL, "zh-CN.UTF-8");

    {
        Window window = {L"鸡汤来喽"};

        GL::Initialize(&window);

        //const ObjectPtr<Texture2D> texture = TextureImporter::Import("C:/Users/BDFFZI/Desktop/KeQing.png");
        //const ObjectPtr<Shader> shader = ShaderImporter::Import("C:/Users/BDFFZI/Desktop/Test.hlsl");
        //const ObjectPtr<Mesh> mesh = MeshImporter::Import("C:/Users/BDFFZI/Desktop/Blender.glb");

        //BDXKObject::Save("C:/Users/BDFFZI/Desktop/BDXKTemp/shader.json", shader);

        const ObjectPtr<Buffer> buffer = Buffer::Create(BufferTarget::Constant, sizeof(Vector4));

        clock_t begin = clock();
        const ObjectPtr<Mesh> mesh = BDXKObject::Load<Mesh>("C:/Users/BDFFZI/Desktop/BDXKTemp/mesh.json");
        const ObjectPtr<Texture2D> texture = BDXKObject::Load<Texture2D>("C:/Users/BDFFZI/Desktop/BDXKTemp/KeQing.json");
        const ObjectPtr<Shader> shader = BDXKObject::Load<Shader>("C:/Users/BDFFZI/Desktop/BDXKTemp/shader.json");
        clock_t end = clock();
        double time = end - begin;

        shader->SetCull(Cull{Cull::State::Off});

        window.AddRenewEvent([&]()
        {
            const Vector4 windowSize = {window.GetSize(), texture->GetSize()};
            buffer->SetData(&windowSize);
            buffer->SetPass(0);

            GL::SetDefaultRenderTarget();

            GL::Clear(true, true, Color::blue);
            shader->SetPass();
            mesh->SetPass();
            texture->SetPass(0);
            GL::Render(mesh->GetTrianglesCount());
        });

        window.Show();
    }

    auto b = Object::GetAllObjects();
    ObjectPtrBase::PrintRefCountMap();

    getchar();
}
