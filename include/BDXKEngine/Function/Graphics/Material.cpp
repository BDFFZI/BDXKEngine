#include "Material.h"
#include <algorithm>

namespace BDXKEngine
{
    ObjectPtr<Material> Material::Create(const std::vector<std::tuple<ObjectPtr<Shader>, ShaderType>>& shaders)
    {
        const auto material = new Material{};

        for (auto& [shader,shaderType] : shaders)
        {
            material->shaders.emplace_back(shader);
            material->shaderTypes.emplace_back(shaderType);
        }

        return Instantiate<Material>(material);
    }

    RenderQueue Material::GetRenderQueue() const
    {
        return renderQueue;
    }
    int Material::GetShaderCount() const
    {
        return static_cast<int>(shaders.size());
    }
    ShaderType Material::GetShaderType(int index) const
    {
        return shaderTypes[index];
    }

    void Material::SetRenderQueue(RenderQueue renderQueue)
    {
        this->renderQueue = renderQueue;
    }
    void Material::SetFloat(int slotIndex, float value)
    {
        if (slotIndex < 0 || slotIndex >= 8)
            throw std::exception("超出容量范围");
        reinterpret_cast<float*>(parameters.GetPtr())[slotIndex] = value;
    }
    void Material::SetVector(int slotIndex, Vector4 value)
    {
        if (slotIndex < 0 || slotIndex >= 6)
            throw std::exception("超出容量范围");
        reinterpret_cast<Vector4*>(parameters.GetPtr())[2 + slotIndex] = value;
    }
    void Material::SetMatrix(int slotIndex, Matrix4x4 value)
    {
        if (slotIndex < 0 || slotIndex >= 4)
            throw std::exception("超出容量范围");
        reinterpret_cast<Matrix4x4*>(parameters.GetPtr())[2 + slotIndex] = value;
    }
    void Material::SetTexture(int slotIndex, const ObjectPtr<Texture>& texture)
    {
        if (slotIndex < 0 || slotIndex >= 4)
            throw std::exception("超出容量范围");
        (&texture0)[slotIndex] = texture;
    }
    void Material::SetPass(int shaderIndex)
    {
        //设置纹理
        if (texture0 != nullptr) texture0->SetPass(0);
        else Texture::SetPassNull(0);
        if (texture1 != nullptr)texture1->SetPass(1);
        else Texture::SetPassNull(1);
        if (texture2 != nullptr)texture2->SetPass(2);
        else Texture::SetPassNull(2);
        if (texture3 != nullptr)texture3->SetPass(3);
        else Texture::SetPassNull(3);

        //设置常量
        parametersBuffer->SetData(parameters.GetPtr());
        parametersBuffer->SetPass(0);

        //设置着色器
        shaders[shaderIndex]->SetPass();
    }

    void Material::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferNestedInfo(parameters);
        TransferFieldInfoOf(renderQueue, int);
        TransferFieldInfo(texture0);
        TransferFieldInfo(texture1);
        TransferFieldInfo(texture2);
        TransferFieldInfo(texture3);
        TransferFieldInfo(shaders);
    }
    void Material::Awake()
    {
        Object::Awake();

        parametersBuffer = Buffer::Create(BufferTarget::Constant, Parameters::GetSize());
    }
}
