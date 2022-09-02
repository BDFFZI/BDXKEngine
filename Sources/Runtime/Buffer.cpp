#include "Buffer.h"

namespace BDXKEngine {
	ObjectPtr<Buffer> Buffer::Create(BufferTarget target, int size)
	{
		return { Object::InstantiateNoAwake<Buffer>(
			[=](Exporter& exporter) {
				exporter.TransferInt(static_cast<int>(target));
				exporter.TransferInt(size);
			}
		) };
	}
	void Buffer::SetData(char* data) {
		context->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
		std::memcpy(this->data.get(), data, size);
	}

	void Buffer::GetData(char* data)
	{
		std::memcpy(data, this->data.get(), size);
	}

	void Buffer::Export(Exporter& exporter)
	{
		exporter.TransferInt(static_cast<int>(target));
		exporter.TransferInt(size);
	}
	void Buffer::Import(Importer& importer)
	{
		target = static_cast<BufferTarget>(importer.TransferInt());
		size = importer.TransferInt();
	}
	void Buffer::Awake()
	{
		data = std::unique_ptr<char>(new char[size]);
		CD3D11_BUFFER_DESC desc(size, (D3D11_BIND_FLAG)target);
		HRESULT result = device->CreateBuffer(&desc, nullptr, &buffer.p);
		assert(SUCCEEDED(result));
	}
}