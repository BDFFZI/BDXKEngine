﻿#pragma once
#include <array>
#include "Platform/GL/GL.h"

namespace BDXKEngine {
	enum class BufferTarget
	{
		Vertex = D3D11_BIND_VERTEX_BUFFER,
		Index = D3D11_BIND_INDEX_BUFFER,
		Constant = D3D11_BIND_CONSTANT_BUFFER,
	};

	class Buffer : public Object, GL
	{
		friend GL;
	public:
		static ObjectPtr<Buffer> Create(BufferTarget target, int size);

		void SetData(const char* data) const;
		void GetData(char* data) const;
	private:
		BufferTarget target;
		int size;
		std::unique_ptr<char[]> data;

		CComPtr<ID3D11Buffer> glBuffer;
		
		void Transfer(Transferrer& transferrer) override;
		void Awake()override;
	};
}

