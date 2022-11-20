#include "Buffer.h"

namespace BDXKEngine
{
    ObjectPtr<Buffer> Buffer::Create(BufferTarget target, int size)
    {
        const auto buffer = new Buffer{};
        buffer->target = target;
        buffer->size = size;

        return Instantiate<Buffer>(buffer);
    }
    void Buffer::SetData(const void* data) const
    {
        std::memcpy(this->data.get(), data, size);
        GL::GetDeviceContext()->UpdateSubresource(glBuffer, 0, nullptr, this->data.get(), 0, 0);
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
        std::memcpy(data, this->data.get(), size);
    }
    CComPtr<ID3D11Buffer> Buffer::GetGLBuffer()
    {
        return glBuffer;
    }

    void Buffer::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfoOf(target, int);
        TransferFieldInfo(size);
    }
    void Buffer::Awake()
    {
        Object::Awake();

        data = std::unique_ptr<char[]>(new char[size]);
        const CD3D11_BUFFER_DESC desc(size, static_cast<D3D11_BIND_FLAG>(target));
        if (FAILED(GL::GetDevice()->CreateBuffer(&desc, nullptr, &glBuffer.p)))
            throw std::exception("创建缓冲区失败");
    }
}
