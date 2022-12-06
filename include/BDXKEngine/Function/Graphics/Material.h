#pragma once
#include <vector>
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Platform/GL/Buffer/Buffer.h"
#include "BDXKEngine/Platform/GL/Shader/Shader.h"
#include "BDXKEngine/Platform/GL/Texture/Texture2D.h"

namespace BDXKEngine
{
    enum class PassType
    {
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
        static ObjectPtr<Material> Create(const std::vector<std::tuple<ObjectPtr<Shader>, PassType>>& passes);
        static void SetPassNull();

        RenderQueue GetRenderQueue() const;
        int GetPassCount() const;
        PassType GetPassType(int index) const;
        const ObjectPtr<Shader>& GetPass(int index) const;

        void SetRenderQueue(RenderQueue renderQueue);
        void SetFloat(int slotIndex, float value);
        void SetVector(int slotIndex, Vector4 value);
        void SetMatrix(int slotIndex, Matrix4x4 value);
        void SetTexture2D(int slotIndex, const ObjectPtr<Texture2D>& texture);
        /// 用当前材质的所有物填充渲染管线
        void SetPass(int shaderIndex);
        bool SetPass(PassType shaderType);
    private:
        struct Parameters : Transferable
        {
            static int GetSize()
            {
                return sizeof(Parameters) - sizeof(std::uintptr_t);
            }

            Vector4 float0_3; //metallic,smoothness
            Vector4 float4_7;
            Vector4 vector0;
            Vector4 vector1;
            Vector4 vector2;
            Vector4 vector3;
            Vector4 vector4;
            Vector4 vector5;
            Matrix4x4 matrix0; //objectToWorld
            Matrix4x4 matrix1;
            Matrix4x4 matrix2;
            Matrix4x4 matrix3;

            char* GetPtr()
            {
                return reinterpret_cast<char*>(this) + sizeof(std::uintptr_t);
            }
            void Transfer(Transferer& transferer) override
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

        // 渲染顺序
        RenderQueue renderQueue = RenderQueue::Geometry;
        std::vector<ObjectPtr<Shader>> shaders;
        std::vector<PassType> shaderTypes;
        Parameters parameters;
        ObjectPtr<Texture2D> texture2D0; //albedo
        ObjectPtr<Texture2D> texture2D1;
        ObjectPtr<Texture2D> texture2D2;
        ObjectPtr<Texture2D> texture2D3;

        ObjectPtr<Buffer> parametersBuffer;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(Material)
}
