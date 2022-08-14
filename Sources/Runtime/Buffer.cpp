#include "Buffer.h"

namespace BDXKEngine {
	void Buffer::SetData(char* data) {
		context->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
		std::memcpy(this->data.get(), data, size);
	}

	void Buffer::GetData(char* data)
	{
		std::memcpy(data, this->data.get(), size);
	}

	void Buffer::Transfer(TransferBase& transfer)
	{
		transfer.TransferInt(reinterpret_cast<int*>(&target));
		transfer.TransferInt(&size);
		transfer.TransferBytes(&data, size);
	}
	void Buffer::Awake()
	{
		CD3D11_BUFFER_DESC desc(size, (D3D11_BIND_FLAG)target);
		D3D11_SUBRESOURCE_DATA glData = { data.get() };
		HRESULT result = device->CreateBuffer(&desc, &glData, &buffer.p);
		assert(SUCCEEDED(result));
	}
}