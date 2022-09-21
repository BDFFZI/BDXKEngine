#include "Buffer.h"

namespace BDXKEngine {
	ObjectPtr<Buffer> Buffer::Create(BufferTarget target, int size)
	{
		Buffer buffer = {};
		buffer.target = target;
		buffer.size = size;

		return Object::Instantiate<Buffer>(&buffer);
	}
	void Buffer::SetData(char* data) {
		context->UpdateSubresource(glBuffer, 0, nullptr, data, 0, 0);
		std::memcpy(this->data.get(), data, size);
	}

	void Buffer::GetData(char* data)
	{
		std::memcpy(data, this->data.get(), size);
	}

	void Buffer::Export(Exporter& exporter)
	{
		Object::Export(exporter);

		exporter.TransferInt(static_cast<int>(target));
		exporter.TransferInt(size);
	}
	void Buffer::Import(Importer& importer)
	{
		Object::Import(importer);

		target = static_cast<BufferTarget>(importer.TransferInt());
		size = importer.TransferInt();
	}
	void Buffer::Awake()
	{
		Object::Awake();

		data = std::unique_ptr<char[]>(new char[size]);
		CD3D11_BUFFER_DESC desc(size, static_cast<D3D11_BIND_FLAG>(target));
		HRESULT result = device->CreateBuffer(&desc, nullptr, &glBuffer.p);
		assert(SUCCEEDED(result));
	}
}