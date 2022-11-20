#include "Material.h"
#include <algorithm>

namespace BDXKEngine
{
    ObjectPtr<Material> Material::Create(const std::vector<ObjectPtr<Shader>>& shaders)
    {
        const auto material = new Material{};
        material->shaders = shaders;

        return Instantiate<Material>(material);
    }

    std::vector<ObjectPtr<Shader>> Material::GetShaders()
    {
        return shaders;
    }
    RenderQueue Material::GetRenderQueue() const
    {
        return renderQueue;
    }
    int Material::GetPassCount() const
    {
        return static_cast<int>(shaders.size());
    }
    PassType Material::GetPassType(int index) const
    {
        return shaders[index]->GetPassType();
    }

    void Material::SetShaders(const std::vector<ObjectPtr<Shader>>& shader)
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
    void Material::SetPass(int index)
    {
        //设置纹理
        if (texture0 != nullptr) texture0->SetPass(0);
        else GL::SetNullTexture2D(0);
        if (texture1 != nullptr)texture1->SetPass(1);
        else GL::SetNullTexture2D(1);
        if (texture2 != nullptr)texture2->SetPass(2);
        else GL::SetNullTexture2D(2);
        if (texture3 != nullptr)texture3->SetPass(3);
        else GL::SetNullTexture2D(3);

        //设置常量
        parametersBuffer->SetData(parameters.GetPtr());
        parametersBuffer->SetPass(0);

        //设置着色器
        shaders[index]->SetPass();
    }

    void Material::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        transferer.TransferNested("parameters", parameters);
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
