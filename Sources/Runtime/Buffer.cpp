#include "Buffer.h"

namespace BDXKEngine {
	Buffer::Buffer(BufferTarget target, int size) :Object(L"New Buffer")
	{
		CD3D11_BUFFER_DESC desc(size, bindFlag = (D3D11_BIND_FLAG)target);
		HRESULT result = device->CreateBuffer(&desc, nullptr, &buffer.p);
		assert(SUCCEEDED(result));
	}
	void Buffer::SetData(void* data) {
		context->UpdateSubresource(buffer, 0, nullptr, data, 0, 0);
	}
}