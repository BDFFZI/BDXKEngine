// #include "BDXKEngine/Framework/Renderer/Core/Camera.h"

// using namespace BDXKEngine;

#include <iostream>

#include "BDXKEditor/BDXKEditor.h"
int main()
{
    BDXKEditor::Run();

    // Window window{L"BDXKEngine"};
    //
    // {
    //     auto a = GameObject::Create("A");
    //     auto b = GameObject::Create("B", a);
    //     GL::Initialize(&window);
    //     GUI::Initialize(&window);
    //     Component::Create<Camera>(a);
    //     Component::Create<Camera>(b);
    //     Texture::ResetDefaultRenderTarget();
    //     window.AddRenewEvent([&]()
    //     {
    //         //渲染
    //         Texture::SetRenderTargetDefault();
    //         GL::Clear(true, true, Color::clear);
    //
    //         //UI
    //         GUI::BeginDraw();
    //         ImGui::ShowDemoWindow();
    //         if (ImGui::Button("A"))
    //         {
    //             a->SetIsEnabling(!a->GetIsEnabling());
    //         }
    //         if (ImGui::Button("B"))
    //         {
    //             b->SetIsEnabling(!b->GetIsEnabling());
    //         }
    //         GUI::EndDraw();
    //
    //         GL::Present();
    //     });
    //
    //     window.Show();
    //
    //     Object::DestroyImmediate(a);
    // }

    // getchar();
}

// {
//     Window window = {L"鸡汤来喽"};
//
//     GL::Initialize(&window);
//     GUI::Initialize(&window);
//
//     //const ObjectPtr<Buffer> buffer = Buffer::Create(BufferTarget::Constant, sizeof(Vector4));
//     const ObjectPtr<Texture2D> texture = Resources::Load<Texture2D>("texture.bin");
//     const ObjectPtr<Mesh> mesh = Resources::Load<Mesh>("mesh.bin");
//     const ObjectPtr<Shader> shader = Resources::Load<Shader>("shader.bin");
//     const ObjectPtr<Material> material = Resources::Load<Material>("material.json", Resources::GetJsonSerializer());
//     // const ObjectPtr<Material> material = Material::Create({
//     //     {shader, ShaderType::ForwardBase}
//     // });
//     // material->SetTexture2D(0, texture);
//     // Resources::Save("material.json", material, Resources::GetJsonSerializer());
//         
//     Texture::ResetDefaultRenderTarget();
//
//     window.AddRenewEvent([&]()
//     {
//         const Vector4 windowSize = {window.GetSize(), texture->GetSize()};
//         // buffer->SetData(&windowSize);
//
//         material->SetFloat(0, windowSize.x);
//         material->SetFloat(1, windowSize.y);
//         material->SetFloat(2, windowSize.z);
//         material->SetFloat(3, windowSize.w);
//
//         //渲染
//         Texture::SetRenderTargetDefault();
//         GL::Clear(true, true, Color::blue);
//         material->SetPass(0);
//         mesh->SetPass();
//         GL::Render(mesh->GetTrianglesCount());
//
//         //UI
//         GUI::BeginDraw();
//         ImGui::ShowDemoWindow();
//         GUI::EndDraw();
//
//         GL::Present();
//     });
//
//     window.Show();
// }
//
// auto b = Object::GetAllObjects();
// ObjectPtrBase::PrintRefCountMap();
