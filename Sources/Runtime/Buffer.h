#pragma once
#include "GL.h"

namespace BDXKEngine {
	enum class BufferTarget
	{
		Vertex = D3D11_BIND_VERTEX_BUFFER,
		Index = D3D11_BIND_INDEX_BUFFER,
		Constant = D3D11_BIND_CONSTANT_BUFFER,
	};

	class Buffer :public Object, GL
	{
		friend GL;
	public:
		Buffer(BufferTarget target, int size);

		void SetData(void* data);
	private:
		D3D11_BIND_FLAG bindFlag;
		CComPtr<ID3D11Buffer> buffer;
	};
}

