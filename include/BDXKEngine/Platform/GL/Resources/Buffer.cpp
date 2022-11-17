#include "Buffer.h"

namespace BDXKEngine
{
    ObjectPtr<Buffer> Buffer::Create(BufferTarget target, int size)
    {
        Buffer buffer = {};
        buffer.target = target;
        buffer.size = size;

        return Instantiate<Buffer>(&buffer);
    }
    void Buffer::SetData(const char* data) const
    {
        GL::GetDeviceContext()->UpdateSubresource(glBuffer, 0, nullptr, data, 0, 0);
        std::memcpy(this->data.get(), data, size);
    }

    void Buffer::GetData(char* data) const
    {
        std::memcpy(data, this->data.get(), size);
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
