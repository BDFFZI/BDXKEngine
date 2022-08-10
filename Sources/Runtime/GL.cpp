#include "GL.h"
#include <exception>
#include "Texture2D.h"
#include "TextureCube.h"
#include "Mesh.h"
#include "Buffer.h"
#include "Shader.h"

namespace BDXKEngine {
	ObjectPtr<Texture2D> GL::GetRenderTarget()
	{
		return renderTexture;
	}

	// ���õ�ǰ��Ⱦ���������ڵ���ɫ��
	void GL::SetShader(ObjectPtr<Shader> shader)
	{
		context->IASetInputLayout(shader->inputLayout);//�йض�����ɫ�������벼��
		context->VSSetShader(shader->vertexShader, nullptr, 0);
		context->PSSetShader(shader->pixelShader, nullptr, 0);

		//���ù�����Ⱦѡ��
		context->OMSetBlendState(shader->blendState, nullptr, 0xffffffff);
		context->OMSetDepthStencilState(shader->depthStencilState, 0);
	}
	// ���õ�ǰ��Ⱦ�����еĶ�����������
	void GL::SetMesh(ObjectPtr<Mesh> mesh)
	{
		//�󶨶�������
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh->vertexBuffer->buffer.p, &stride, &offset);

		//����������
		context->IASetIndexBuffer(mesh->triangleBuffer->buffer, DXGI_FORMAT_R32_UINT, 0);
	}
	// ���õ�ǰ��Ⱦ�����еĳ���������
	void GL::SetBuffer(unsigned int startSlot, ObjectPtr<Buffer> buffer)
	{
		ID3D11Buffer* d3dBuffer = nullptr;
		if (buffer != nullptr)
		{
			if (buffer->bindFlag != D3D11_BIND_CONSTANT_BUFFER)
				throw std::exception("��֧�ֳ��������������������");
			d3dBuffer = buffer->buffer;
		}

		context->VSSetConstantBuffers(startSlot, 1, &d3dBuffer);
		context->PSSetConstantBuffers(startSlot, 1, &d3dBuffer);
	}
	// ���õ�ǰ��Ⱦ�����е���ɫ����Դ
	void GL::SetTexture(unsigned int startSlot, ObjectPtr<Texture> texture)
	{
		ID3D11ShaderResourceView* resourceView = nullptr;
		ID3D11SamplerState* samplerState = nullptr;
		if (texture != nullptr)
		{
			resourceView = texture->GetResourceView().p;
			samplerState = texture->GetSamplerState().p;
		}

		context->PSSetShaderResources(startSlot, 1, &resourceView);
		context->PSSetSamplers(startSlot, 1, &samplerState);
	}
	// ������Ⱦ����Ŀ������
	void GL::SetRenderTarget(ObjectPtr<Texture2D> renderTexture) {
		GL::renderTexture = renderTexture;
		if (renderTexture != nullptr)
		{
			renderTargetView = renderTexture->renderTextureRTV;
			depthStencilView = renderTexture->depthTextureDSV;
			//�����ӿ����������������Сһ��
			D3D11_VIEWPORT viewport = {};
			viewport.Height = (float)renderTexture->GetHeight();
			viewport.Width = (float)renderTexture->GetWidth();
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			context->RSSetViewports(1, &viewport);
		}
		else//δ��ʱʹ��Ĭ��ֵ
		{
			renderTargetView = defaultRenderTargetView;
			depthStencilView = defaultDepthStencilView;
			//�����ӿ����������������Сһ��
			D3D11_VIEWPORT viewport = {};
			viewport.Height = (float)defaultRenderTargetDescription.Height;
			viewport.Width = (float)defaultRenderTargetDescription.Width;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			context->RSSetViewports(1, &viewport);
		}

		//������ȾĿ��
		context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
	}
	void GL::SetRenderTarget(ObjectPtr<TextureCube> textureCube, int index)
	{
		if (textureCube != nullptr)
		{
			renderTargetView = textureCube->renderTextureRTV[index];
			depthStencilView = textureCube->depthTextureDSV;
			//�����ӿ����������������Сһ��
			D3D11_VIEWPORT viewport = {};
			viewport.Height = (float)textureCube->GetHeight();
			viewport.Width = (float)textureCube->GetWidth();
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			context->RSSetViewports(1, &viewport);
		}
		else//δ��ʱʹ��Ĭ��ֵ
		{
			renderTargetView = defaultRenderTargetView;
			depthStencilView = defaultDepthStencilView;
			//�����ӿ����������������Сһ��
			D3D11_VIEWPORT viewport = {};
			viewport.Height = (float)defaultRenderTargetDescription.Height;
			viewport.Width = (float)defaultRenderTargetDescription.Width;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			context->RSSetViewports(1, &viewport);
		}

		//������ȾĿ��
		context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
	}

	void GL::Clear(bool clearDepth, bool clearColor, Color backgroundColor, float depth)
	{
		//���û�����ͼ
		if (clearColor == true)
		{
			const float teal[] = { backgroundColor.r, backgroundColor.g,backgroundColor.b, backgroundColor.a };
			context->ClearRenderTargetView(renderTargetView, teal);
		}

		//�������ģ�������ͼ
		if (clearDepth)
		{
			context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);
		}
	}
	void GL::Render(int indexsCount)
	{
		//��ʼ����
		context->DrawIndexed(indexsCount, 0, 0);
	}

	void GL::Initialize(Window* window)
	{
		GL::CreateDevice();
		GL::CreateSwapChain(window->GetHwnd());
		ResizeDefaultRenderTarget(window->GetScreenRect().GetSize());
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	CComPtr<ID3D11Device> GL::device = nullptr;
	CComPtr<ID3D11DeviceContext> GL::context = nullptr;
	CComPtr<IDXGISwapChain1> GL::swapChain = nullptr;
	D3D11_TEXTURE2D_DESC GL::defaultRenderTargetDescription = {};
	CComPtr<ID3D11RenderTargetView> GL::defaultRenderTargetView = nullptr;
	CComPtr<ID3D11DepthStencilView> GL::defaultDepthStencilView = nullptr;
	CComPtr<ID3D11RenderTargetView> GL::renderTargetView = nullptr;
	CComPtr<ID3D11DepthStencilView> GL::depthStencilView = nullptr;
	ObjectPtr<Texture2D> GL::renderTexture = nullptr;

	void GL::CreateDevice() {
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_11_1,
		};

		HRESULT result = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_SINGLETHREADED
			| D3D11_CREATE_DEVICE_BGRA_SUPPORT
			| D3D11_CREATE_DEVICE_DEBUG,
			levels,
			ARRAYSIZE(levels),
			D3D11_SDK_VERSION,
			&device,
			NULL,
			&context
		);
		assert(SUCCEEDED(result));// Direct3D�豸�����Ƿ�ɹ�
	}
	void GL::CreateSwapChain(HWND hwnd)
	{
		//��ȡ�ײ�DXGI�Ĺ���
		CComPtr<IDXGIDevice> dxglDevice;
		CComPtr<IDXGIAdapter> dxglAdapter;
		CComPtr<IDXGIFactory2> dxglFactory;

		assert(SUCCEEDED(device->QueryInterface(&dxglDevice)));//��ȡDXGI�豸
		assert(SUCCEEDED(dxglDevice->GetAdapter(&dxglAdapter)));//��ȡDXGI������
		assert(SUCCEEDED(dxglAdapter->GetParent(IID_PPV_ARGS(&dxglFactory)))); //��ȡDXGI����

		UINT m4xMsaaQuality;
		device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m4xMsaaQuality);
		assert(m4xMsaaQuality > 0);


		//����������������
		DXGI_SWAP_CHAIN_DESC1 swapChainProperty = {};//�ֲ�����һ��Ҫ��ʼ��
		swapChainProperty.BufferCount = 2;
		swapChainProperty.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainProperty.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainProperty.SampleDesc.Count = 1;
		swapChainProperty.SampleDesc.Quality = 0;
		swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
		fullScreenDesc.Windowed = true;

		//����������
		assert(SUCCEEDED(dxglFactory->CreateSwapChainForHwnd(
			device,
			hwnd,
			&swapChainProperty,
			&fullScreenDesc,
			nullptr,
			&swapChain)
		));

	}

	CComPtr<ID3D11Texture2D> GL::GetDefaultRenderTarget() {
		CComPtr<ID3D11Texture2D> renderTargetTexture;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetTexture));
		return renderTargetTexture;
	}

	void GL::ResizeDefaultRenderTarget(Vector2 size)
	{
		HRESULT result = 0;

		//��Ҫ���������С������ͼһֱռ�������������û�����ã�����Ҫ���ͷ���ͼ
		if (renderTargetView == defaultRenderTargetView)renderTargetView = nullptr;
		if (depthStencilView == defaultDepthStencilView)depthStencilView = nullptr;
		defaultRenderTargetView = nullptr;
		defaultDepthStencilView = nullptr;

		//������Ⱦ�����С
		swapChain->ResizeBuffers(2, (UINT)size.x, (UINT)size.y, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

		//���´�����ͼ

		//��ȡ��Ļ��ȾĿ�����������
		CComPtr<ID3D11Texture2D> targetTexture = nullptr;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&targetTexture.p));
		targetTexture->GetDesc(&defaultRenderTargetDescription);
		//�Ըò��������������ģ����Ե�����
		CComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;
		D3D11_TEXTURE2D_DESC depthStencilTextureDescription = {};
		depthStencilTextureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilTextureDescription.Width = static_cast<UINT> (defaultRenderTargetDescription.Width);
		depthStencilTextureDescription.Height = static_cast<UINT> (defaultRenderTargetDescription.Height);
		depthStencilTextureDescription.ArraySize = 1;
		depthStencilTextureDescription.SampleDesc = { 1,0 };
		result = device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture.p);
		assert(SUCCEEDED(result));
		//������ȾĿ��������ͼ
		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{ D3D11_RTV_DIMENSION_TEXTURE2D };
		result = device->CreateRenderTargetView(targetTexture, &renderTargetViewDesc, &defaultRenderTargetView.p);
		assert(SUCCEEDED(result));
		//�������ģ����Ե���ͼ
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{ D3D11_DSV_DIMENSION_TEXTURE2D };
		result = device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &defaultDepthStencilView.p);
		assert(SUCCEEDED(result));
	}
	void GL::Present()
	{
		swapChain->Present(0, 0);
	}
}

