#pragma once
#include <vector>
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Platform/GL/Buffer/Buffer.h"
#include "BDXKEngine/Platform/GL/Command/Blend.h"
#include "BDXKEngine/Platform/GL/Command/Cull.h"
#include "BDXKEngine/Platform/GL/Command/ZTest.h"
#include "BDXKEngine/Platform/GL/Shader/Shader.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"

namespace BDXKEngine
{
    enum class PassType
    {
        None,
        ForwardBase,
        ForwardAdd,
        ShadowCaster
    };

    enum class RenderQueue
    {
        Background = 1000,
        Geometry = 2000,
        AlphaTest = 2450,
        Transparent = 3000,
        Overlay = 4000,
    };

    class Material : public Object
    {
    public:
        static ObjectPtr<Material> Create(const std::vector<std::tuple<PassType, ObjectPtr<Shader>>>& passes);
        static void UploadRPNull();

        int GetPassCount() const;
        PassType GetPassType(int index) const;
        const Blend& GetPassBlend(int index) const;
        const ZTest& GetPassZTest(int index) const;
        const Cull& GetPassCull(int index) const;
        RenderQueue GetRenderQueue() const;

        void SetPassBlend(int index, const Blend& blend);
        void SetPassZTest(int index, const ZTest& zTest);
        void SetPassCull(int index, const Cull& cull);
        void SetRenderQueue(RenderQueue renderQueue);
        void SetFloat(int slotIndex, float value);
        void SetVector(int slotIndex, Vector4 value);
        void SetMatrix(int slotIndex, Matrix4x4 value);
        void SetTexture2D(int slotIndex, const ObjectPtr<Texture2D>& texture);
        /// 用当前材质的所有物填充渲染管线
        void UploadRP(int passIndex) const;
        bool UploadRP(PassType passType) const;
    private:
        struct Parameters
        {
            Vector4 float0_3 = Vector4{0, 0.5f, 0, 0}; //metallic,smoothness
            Vector4 float4_7;
            Vector4 vector0 = Vector4{Color::white}; //color
            Vector4 vector1;
            Vector4 vector2;
            Vector4 vector3;
            Vector4 vector4;
            Vector4 vector5;
            Matrix4x4 matrix0 = Matrix4x4::identity; //objectToWorld
            Matrix4x4 matrix1;
            Matrix4x4 matrix2;
            Matrix4x4 matrix3;

            void Transfer(Transferer& transferer)
            {
                TransferFieldInfo(float0_3);
                TransferFieldInfo(float4_7);
                TransferFieldInfo(vector0);
                TransferFieldInfo(vector1);
                TransferFieldInfo(vector2);
                TransferFieldInfo(vector3);
                TransferFieldInfo(vector4);
                TransferFieldInfo(vector5);
                TransferFieldInfo(matrix0);
                TransferFieldInfo(matrix1);
                TransferFieldInfo(matrix2);
                TransferFieldInfo(matrix3);
            }
        };

        //渲染顺序
        RenderQueue renderQueue = RenderQueue::Geometry;
        //pass数据
        std::vector<PassType> passTypes;
        std::vector<ObjectPtr<Shader>> passShaders;
        std::vector<Blend> passBlends;
        std::vector<ZTest> passZTests;
        std::vector<Cull> passCulls;
        //参数
        Parameters parameters;
        ObjectPtr<Buffer> parametersBuffer;
        ObjectPtr<Texture2D> texture2D0; //albedo
        ObjectPtr<Texture2D> texture2D1;
        ObjectPtr<Texture2D> texture2D2;
        ObjectPtr<Texture2D> texture2D3;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(Material)
}
