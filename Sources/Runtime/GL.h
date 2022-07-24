#pragma once
#include <d3d11_1.h>
#include <dxgi.h>
#include <cassert>
#include <atlbase.h>
#include "Color.h"
#include "Rect.h"
#include "ObjectPtr.h"

#include "Blend.h"
#include "Cull.h"
#include "ZTest.h"

namespace BDXKEngine {
	enum class GLRenderStage {
		Vertex,
		Pixed,
	};
	
	class Texture;
	class Texture2D;
	class Mesh;
	class GL
	{
	public:
		static void CreateVertexShader(const wchar_t* path, D3D11_INPUT_ELEMENT_DESC vertexShaderInput[], int inputLayoutDescriptorCount, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout);
		static void CreatePixelShader(const wchar_t* path, ID3D11PixelShader** pixelShader);

		template<typename BufferData>
		static void CreateConstantBuffer(BufferData* source, int bufferSize, ID3D11Buffer** buffer)
		{
			CreateBuffer(source, bufferSize, D3D11_BIND_CONSTANT_BUFFER, buffer);
		}
		template<typename BufferData>
		static void CreateBuffer(BufferData* source, int bufferSize, D3D11_BIND_FLAG bindFlag, ID3D11Buffer** buffer)
		{
			CD3D11_BUFFER_DESC desc(bufferSize, bindFlag);
			D3D11_SUBRESOURCE_DATA data = { source ,0,0 };

			assert(SUCCEEDED(device->CreateBuffer(&desc, &data, buffer)));
		}

		static void CreateSamplerState(ID3D11SamplerState** samplerState);

		static void UpdateBlend(Blend* blend);
		static void UpdateZTest(ZTest* zTest);
		template<typename BufferData>
		static void UpdateBuffer(ID3D11Buffer* buffer, BufferData* source)
		{
			context->UpdateSubresource(buffer, 0, nullptr, source, 0, 0);
		}

		// ���õ�ǰ��Ⱦ���������ڵ���ɫ��
		static void SetShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexShaderInputLayout, ID3D11PixelShader* pixelShader);
		// ���õ�ǰ��Ⱦ�����еĶ�����������
		static void SetMesh(ObjectPtr<Mesh> mesh);
		// ���õ�ǰ��Ⱦ�����еĳ���������������ƹ�����֮���
		static void SetConstantBuffer(unsigned int startSlot, ID3D11Buffer** buffer);
		// ���õ�ǰ��Ⱦ�����е���ɫ����Դ,������֮���
		static void SetTexture(unsigned int startSlot, ObjectPtr<Texture> texture);
		// ������ɫ����������ʱ�ķ�ʽ
		static void SetSamplerState(ID3D11SamplerState** samplerState);
		// ����Ҫ��Ⱦ����Ŀ������Ϊ��ʱ���û�Ĭ��Ŀ�꣬����Ļ
		static void SetRenderTarget(ObjectPtr<Texture2D> renderTargetTexture);
		// ������Ⱦʱ����ɫ��Ϸ�ʽ
		static void SetBlend(Blend* blend);
		// ������Ⱦʱ�����ģ�建������Ӱ�췽ʽ
		static void SetZTest(ZTest* zTest);

		static ObjectPtr<Texture2D> GetRenderTarget();

		///������ȾĬ������ʱʹ�øú��������ܳ�����Ϊ��Ⱦ����GL2Dռ�ö������޷����������С
		static void ResetRenderTarget(Rect rect = {});
		static void Begin(D3D11_PRIMITIVE_TOPOLOGY drawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		static void End();
		static void Flush();
		static void Clear(bool clearDepth, bool clearColor, Color backgroundColor = Color::clear, float depth = 1.0f);
		static void Render(int indexsCount);


	protected:
		static GL* Initialize(HWND window);

		static CComPtr<ID3D11Device> device;
		static CComPtr<ID3D11DeviceContext> context;
		static CComPtr<IDXGISwapChain1> swapChain;
		//�����Ⱦ�����ģ��ʱ�õ���
		static CComPtr<ID3D11RenderTargetView> renderTargetView;
		static CComPtr<ID3D11DepthStencilView> depthStencilView;

		static void CreateDevice();
		static void CreateSwapChain(HWND hwnd);
		static void CompileShader(const wchar_t* path, const char* entrypoint, const char* object, ID3DBlob** blob);
	};
}



