#include "Buffer.h"

namespace BDXKEngine
{
    ObjectPtr<Buffer> Buffer::Create(BufferTarget target, int size)
    {
        Buffer buffer = {};
        buffer.target = target;
        buffer.size = size;

        return Object::Instantiate<Buffer>(&buffer);
    }
    void Buffer::SetData(const char* data) const
    {
        context->UpdateSubresource(glBuffer, 0, nullptr, data, 0, 0);
        std::memcpy(this->data.get(), data, size);
    }

    void Buffer::GetData(char* data) const
    {
        std::memcpy(data, this->data.get(), size);
    }

    void Buffer::Transfer(Transferrer& transferrer)
    {
        Object::Transfer(transferrer);

        transferrer.TransferFieldOf<int>(nameof(target), target);
        transferrer.TransferField(nameof(size), size);
    }
    void Buffer::PreAwake()
    {
        Object::PreAwake();

        data = std::unique_ptr<char[]>(new char[size]);
        const CD3D11_BUFFER_DESC desc(size, static_cast<D3D11_BIND_FLAG>(target));
        const HRESULT result = device->CreateBuffer(&desc, nullptr, &glBuffer.p);
        assert(SUCCEEDED(result));
    }
}
