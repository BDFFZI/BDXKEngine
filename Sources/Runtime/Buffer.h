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
		void SetData(char* data);
		void GetData(char* data);
	private:
		BufferTarget target;
		int size;
		std::unique_ptr<char> data;

		CComPtr<ID3D11Buffer> buffer;

		void Transfer(TransferBase& transfer)override;
		void Awake()override;
	};
}

