#include "Buffer.h"

namespace BDXKEngine
{
    ObjectPtr<Buffer> Buffer::Create(BufferTarget target, int size)
    {
        ObjectPtr buffer = new Buffer{};
        buffer->target = target;
        buffer->buffer.resize(size);
        Instantiate(buffer);
        
        return buffer;
    }
    void Buffer::SetData(const void* data)
    {
        std::memcpy(buffer.data(), data, buffer.size());
        GL::GetDeviceContext()->UpdateSubresource(glBuffer, 0, nullptr, buffer.data(), 0, 0);
    }
    void Buffer::SetPass(unsigned int startSlot) const
    {
        if (target != BufferTarget::Constant)
            throw std::exception("不支持除常量缓冲区以外的类型");

        const auto context = GL::GetDeviceContext();
        context->VSSetConstantBuffers(startSlot, 1, &glBuffer.p);
        context->PSSetConstantBuffers(startSlot, 1, &glBuffer.p);
    }

    void Buffer::GetData(char* data) const
    {
        std::memcpy(data, buffer.data(), buffer.size());
    }
    CComPtr<ID3D11Buffer> Buffer::GetGLBuffer()
    {
        return glBuffer;
    }

    void Buffer::Awake()
    {
        Object::Awake();

        const CD3D11_BUFFER_DESC desc(static_cast<unsigned int>(buffer.size()), static_cast<D3D11_BIND_FLAG>(target));
        if (FAILED(GL::GetDevice()->CreateBuffer(&desc, nullptr, &glBuffer.p)))
            throw std::exception("创建缓冲区失败");
    }
}
