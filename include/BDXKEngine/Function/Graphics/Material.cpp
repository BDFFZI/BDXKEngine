#include "Material.h"
#include <algorithm>

namespace BDXKEngine
{
    ObjectPtr<Material> Material::Create(const std::vector<std::tuple<PassType, ObjectPtr<Shader>>>& passes)
    {
        ObjectPtr material = new Material{};
        for (auto& [shaderType,shader] : passes)
        {
            material->passTypes.emplace_back(shaderType);
            material->passShaders.emplace_back(shader);
            material->passBlends.emplace_back(Blend::Opaque);
            material->passZTests.emplace_back(ZTest::Default);
            material->passCulls.emplace_back(Cull::Default);
        }
        const ObjectPtr<Texture2D> defaultTex = Texture2D::Create(Color::white);
        material->texture2D0 = defaultTex;
        material->texture2D1 = defaultTex;
        material->texture2D2 = defaultTex;
        material->texture2D3 = defaultTex;

        Instantiate(material);
        return material;
    }
    void Material::UploadRPNull()
    {
        Texture::UploadRPNull(0);
        Texture::UploadRPNull(1);
        Texture::UploadRPNull(2);
        Texture::UploadRPNull(3);
    }

    int Material::GetPassCount() const
    {
        return static_cast<int>(passTypes.size());
    }
    PassType Material::GetPassType(int index) const
    {
        return passTypes[index];
    }
    const Blend& Material::GetPassBlend(int index) const
    {
        return passBlends.at(index);
    }
    const ZTest& Material::GetPassZTest(int index) const
    {
        return passZTests.at(index);
    }
    const Cull& Material::GetPassCull(int index) const
    {
        return passCulls.at(index);
    }
    RenderQueue Material::GetRenderQueue() const
    {
        return renderQueue;
    }

    void Material::SetPassBlend(int index, const Blend& blend)
    {
        this->passBlends[index] = blend;
        this->passBlends[index].UpdateGL();
    }
    void Material::SetPassZTest(int index, const ZTest& zTest)
    {
        this->passZTests[index] = zTest;
        this->passZTests[index].UpdateGL();
    }
    void Material::SetPassCull(int index, const Cull& cull)
    {
        this->passCulls[index] = cull;
        this->passCulls[index].UpdateGL();
    }
    void Material::SetRenderQueue(RenderQueue renderQueue)
    {
        this->renderQueue = renderQueue;
    }
    void Material::SetFloat(int slotIndex, float value)
    {
        if (slotIndex < 0 || slotIndex >= 8)
            throw std::exception("超出容量范围");
        reinterpret_cast<float*>(&parameters)[slotIndex] = value;
    }
    void Material::SetVector(int slotIndex, Vector4 value)
    {
        if (slotIndex < 0 || slotIndex >= 6)
            throw std::exception("超出容量范围");
        reinterpret_cast<Vector4*>(&parameters)[2 + slotIndex] = value;
    }
    void Material::SetMatrix(int slotIndex, Matrix4x4 value)
    {
        if (slotIndex < 0 || slotIndex >= 4)
            throw std::exception("超出容量范围");
        reinterpret_cast<Matrix4x4*>(&parameters)[2 + slotIndex] = value;
    }
    void Material::SetTexture2D(int slotIndex, const ObjectPtr<Texture2D>& texture)
    {
        if (slotIndex < 0 || slotIndex >= 4)
            throw std::exception("超出容量范围");
        (&texture2D0)[slotIndex] = texture;
    }

    void Material::UploadRP(int passIndex) const
    {
        //设置纹理
        if (texture2D0 != nullptr) texture2D0->UploadRP(0);
        else Texture::UploadRPNull(0);
        if (texture2D1 != nullptr)texture2D1->UploadRP(1);
        else Texture::UploadRPNull(1);
        if (texture2D2 != nullptr)texture2D2->UploadRP(2);
        else Texture::UploadRPNull(2);
        if (texture2D3 != nullptr)texture2D3->UploadRP(3);
        else Texture::UploadRPNull(3);

        //设置常量
        parametersBuffer->SetData(&parameters);
        parametersBuffer->UploadRP(0);

        //设置着色器
        passShaders[passIndex]->UploadRP();

        //设置命令
        this->passBlends[passIndex].UploadRP();
        this->passZTests[passIndex].UploadRP();
        this->passCulls[passIndex].UploadRP();
    }
    bool Material::UploadRP(PassType passType) const
    {
        const int count = static_cast<int>(passTypes.size());
        for (int i = 0; i < count; i++)
        {
            if (passTypes[i] == passType)
            {
                UploadRP(i);
                return true;
            }
        }

        return false;
    }

    void Material::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        transferer.SetTransferFunc<PassType>([&](PassType& value)
        {
            transferer.TransferValueOf<int>(value);
        });
        transferer.SetTransferFunc<Blend>([&](Blend& value)
        {
            value.Transfer(transferer);
        });
        transferer.SetTransferFunc<ZTest>([&](ZTest& value)
        {
            value.Transfer(transferer);
        });
        transferer.SetTransferFunc<Cull>([&](Cull& value)
        {
            value.Transfer(transferer);
        });

        TransferFieldInfo(passTypes);
        TransferFieldInfo(passShaders);
        TransferFieldInfo(passBlends);
        TransferFieldInfo(passZTests);
        TransferFieldInfo(passCulls);
        TransferFieldInfoOf(renderQueue, int);
        TransferFieldNestedInfo(parameters);
        TransferFieldInfo(texture2D0);
        TransferFieldInfo(texture2D1);
        TransferFieldInfo(texture2D2);
        TransferFieldInfo(texture2D3);
    }
    void Material::Awake()
    {
        Object::Awake();

        parametersBuffer = Buffer::Create(BufferTarget::Constant, sizeof(Parameters));
        std::ranges::for_each(passBlends, [](Blend& blend) { blend.UpdateGL(); });
        std::ranges::for_each(passZTests, [](ZTest& ztest) { ztest.UpdateGL(); });
        std::ranges::for_each(passCulls, [](Cull& cull) { cull.UpdateGL(); });
    }
}
