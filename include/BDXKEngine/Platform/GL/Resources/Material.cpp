#include "Material.h"
#include <algorithm>

namespace BDXKEngine
{
    ObjectPtr<Material> Material::Create(std::vector<ObjectPtr<Shader>> shaders)
    {
        Material material = {};
        material.shaders = shaders;

        return Instantiate<Material>(&material);
    }

    std::vector<ObjectPtr<Shader>> Material::GetShaders()
    {
        return shaders;
    }
    RenderQueue Material::GetRenderQueue()
    {
        return renderQueue;
    }
    int Material::GetPassCount()
    {
        return (int)shaders.size();
    }
    PassType Material::GetPassType(int index)
    {
        return shaders[index]->GetPassType();
    }

    void Material::SetShaders(std::vector<ObjectPtr<Shader>> shader)
    {
        this->shaders = shader;
    }
    void Material::SetRenderQueue(RenderQueue renderQueue)
    {
        this->renderQueue = renderQueue;
    }

    void Material::SetFloat(int slotIndex, float value)
    {
        if (slotIndex < 0 || slotIndex >= 8)
            throw std::exception("超出容量范围");
        ((float*)&parameters)[slotIndex] = value;
    }
    void Material::SetVector(int slotIndex, Vector4 value)
    {
        if (slotIndex < 0 || slotIndex >= 6)
            throw std::exception("超出容量范围");
        ((Vector4*)&parameters)[2 + slotIndex] = value;
    }
    void Material::SetMatrix(int slotIndex, Matrix4x4 value)
    {
        if (slotIndex < 0 || slotIndex >= 4)
            throw std::exception("超出容量范围");
        ((Matrix4x4*)&parameters)[2 + slotIndex] = value;
    }
    void Material::SetTexture(int slotIndex, ObjectPtr<Texture> texture)
    {
        if (slotIndex < 0 || slotIndex >= 4)
            throw std::exception("超出容量范围");
        (&texture0)[slotIndex] = texture;
    }
    void Material::SetPass(int index)
    {
        //设置纹理
        GL::SetTexture(0, texture0);
        GL::SetTexture(1, texture1);
        GL::SetTexture(2, texture2);
        GL::SetTexture(3, texture3);

        //设置常量
        parametersBuffer->SetData(reinterpret_cast<char*>(&parameters));
        GL::SetBuffer(0, parametersBuffer);

        //设置着色器
        GL::SetShader(shaders[index]);
    }

    void Material::Transfer(transferer& transferer)
    {
        Object::Transfer(transferer);

        transferer.TransferFieldOf<int>("renderQueue", renderQueue);
        transferer.TransferField("parameters", reinterpret_cast<char*>(&parameters), sizeof(Parameters));
        transferer.TransferField(nameof(texture0), texture0);
        transferer.TransferField(nameof(texture1), texture1);
        transferer.TransferField(nameof(texture2), texture2);
        transferer.TransferField(nameof(texture3), texture3);

        int shadersCount = shaders.size();
        transferer.TransferField(nameof(shadersCount), shadersCount);

        if (transferer.GetTransferDirection() == TransferDirection::Input) shaders.resize(shadersCount);
        for (int i = 0; i < shadersCount; i++)
            transferer.TransferField("shader_" + std::to_string(i), shaders[i]);
    }
    void Material::MarkAwake()
    {
        Object::MarkAwake();

        parametersBuffer = Buffer::Create(BufferTarget::Constant, sizeof(Parameters));

        for (auto shader : shaders)
            Object::MarkAwake(shader.ToObjectBase());
        Object::MarkAwake(parametersBuffer.ToObjectBase());
        Object::MarkAwake(texture0.ToObjectBase());
        Object::MarkAwake(texture1.ToObjectBase());
        Object::MarkAwake(texture2.ToObjectBase());
        Object::MarkAwake(texture3.ToObjectBase());
    }
}
