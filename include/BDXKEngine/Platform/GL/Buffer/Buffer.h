#pragma once
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "../GL.h"

namespace BDXKEngine
{
    enum class BufferTarget
    {
        Vertex = D3D11_BIND_VERTEX_BUFFER,
        Index = D3D11_BIND_INDEX_BUFFER,
        Constant = D3D11_BIND_CONSTANT_BUFFER,
    };

    class Buffer : public Object
    {
    public:
        static ObjectPtr<Buffer> Create(BufferTarget target, int size);

        void GetData(char* data) const;
        CComPtr<ID3D11Buffer> GetGLBuffer();

        void SetData(const void* data);
        void SetPass(unsigned int startSlot) const; // 设置当前渲染管线中的常量缓冲区，矩阵灯光数据之类的
    private:
        BufferTarget target = BufferTarget::Constant;
        std::vector<char> buffer;

        CComPtr<ID3D11Buffer> glBuffer;
        
        void Awake() override;
    };
}
