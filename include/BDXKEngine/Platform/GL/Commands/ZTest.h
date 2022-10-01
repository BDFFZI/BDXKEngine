#pragma once
#include <d3d11.h>

namespace BDXKEngine {
	struct ZTest {
		enum class Operation
		{
			Less = D3D11_COMPARISON_LESS,
			LEqual = D3D11_COMPARISON_LESS_EQUAL,
			Equal = D3D11_COMPARISON_EQUAL,
			GEqual = D3D11_COMPARISON_GREATER_EQUAL,
			Greater = D3D11_COMPARISON_GREATER,
			NotEqual = D3D11_COMPARISON_NOT_EQUAL,
			Always = D3D11_COMPARISON_ALWAYS,
		};

		static ZTest Default;

		Operation operation = Operation::LEqual;
		float factor = 0;//缩放深度
		float units = 0;//偏移深度
		bool write = true;
		bool clip = true;
	};
}
