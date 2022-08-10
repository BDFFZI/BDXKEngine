#pragma once
#include <d3d11_1.h>
#include <dxgi.h>
#include <cassert>
#include <atlbase.h>
#include "Window.h"
#include "Color.h"
#include "Rect.h"
#include "ObjectPtr.h"

namespace BDXKEngine {
	class Texture;
	class Texture2D;
	class TextureCube;
	class Mesh;
	class Buffer;
	class Shader;
	class GL
	{
	public:
		static ObjectPtr<Texture2D> GetRenderTarget();

		/// ���õ�ǰ��Ⱦ���������ڵ���ɫ��
		static void SetShader(ObjectPtr<Shader> shader);
		/// ���õ�ǰ��Ⱦ�����еĶ�����������
		static void SetMesh(ObjectPtr<Mesh> mesh);
		/// ���õ�ǰ��Ⱦ�����еĳ���������������ƹ�����֮���
		static void SetBuffer(unsigned int startSlot, ObjectPtr<Buffer> buffer);
		/// ���õ�ǰ��Ⱦ�����е���ɫ����Դ,������֮���
		static void SetTexture(unsigned int startSlot, ObjectPtr<Texture> texture);
		/// ����Ҫ��Ⱦ����Ŀ������Ϊ��ʱ���û�Ĭ��Ŀ�꼴��Ļ,Present�ᵼ�½���󶨣�����ÿ�ζ����°�
		static void SetRenderTarget(ObjectPtr<Texture2D> renderTexture);
		static void SetRenderTarget(ObjectPtr<TextureCube> textureCube, int index);

		static void Clear(bool clearDepth, bool clearColor, Color backgroundColor = Color::clear, float depth = 1.0f);
		static void Render(int indexsCount);
	protected:
		static void Initialize(Window* window);

		static CComPtr<ID3D11Device> device;
		static CComPtr<ID3D11DeviceContext> context;
		static CComPtr<IDXGISwapChain1> swapChain;
		//�����Ⱦ�����ģ��ʱ�õ���
		static CComPtr<ID3D11RenderTargetView> renderTargetView;
		static CComPtr<ID3D11DepthStencilView> depthStencilView;
		static ObjectPtr<Texture2D> renderTexture;
		//Ĭ���ʲ�
		static D3D11_TEXTURE2D_DESC defaultRenderTargetDescription;
		static CComPtr<ID3D11RenderTargetView> defaultRenderTargetView;
		static CComPtr<ID3D11DepthStencilView> defaultDepthStencilView;

		static void CreateDevice();
		static void CreateSwapChain(HWND hwnd);

		static CComPtr<ID3D11Texture2D> GetDefaultRenderTarget();

		/// ��ʹ����GL2D���ܣ�����ܳ�����Ϊ��Ⱦ����GL2Dռ�ö������޷����������С������Ҫ���GL2D�Ǳߵ���Դ�ͷ�
		static void ResizeDefaultRenderTarget(Vector2 size);
		/// ��Ĭ����ȾĿ��Ļ�����ֵ���Ļ��
		static void Present();
	};
}



