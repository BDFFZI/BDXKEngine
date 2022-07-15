#include "GL.h"

namespace BDXKEngine {
	CComPtr<ID3D11Device> GL::device;
	CComPtr<ID3D11DeviceContext> GL::context;
	CComPtr<IDXGISwapChain1> GL::swapChain;

	CComPtr<ID3D11RenderTargetView> GL::renderTargetView;
	CComPtr<ID3D11DepthStencilView> GL::depthStencilView;


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
		swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

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

	void GL::CreateVertexShader(const char* path, D3D11_INPUT_ELEMENT_DESC inputLayoutDescriptor[], int inputLayoutDescriptorCount,
		ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout)
	{
		const int byteBufferSize = 32 * 1024;
		//��ȡ������ɫ�������ƴ���
		FILE* vertexShaderFile;
		fopen_s(&vertexShaderFile, path, "rb");
		if (vertexShaderFile == 0)
			throw std::exception("������ɫ���ļ���ʧ��");

		char* vertexShaderBytes = new char[byteBufferSize];
		size_t vertexShaderBytesSize = fread_s(vertexShaderBytes, byteBufferSize, 1, byteBufferSize, vertexShaderFile);
		fclose(vertexShaderFile);
		//����������ɫ��
		assert(SUCCEEDED(device->CreateVertexShader(
			vertexShaderBytes,
			vertexShaderBytesSize,
			nullptr,
			vertexShader
		)));

		//����������Ϣ
		assert(SUCCEEDED(device->CreateInputLayout(
			inputLayoutDescriptor,
			inputLayoutDescriptorCount,
			vertexShaderBytes,
			vertexShaderBytesSize,
			inputLayout
		)));

		delete[] vertexShaderBytes;
	}
	void GL::CreatePixelShader(const char* path, ID3D11PixelShader** pixelShader)
	{
		const int byteBufferSize = 32 * 1024;
		//��ȡ������ɫ�������ƴ���
		FILE* pixelShaderFile;
		fopen_s(&pixelShaderFile, path, "rb");
		if (pixelShaderFile == 0)
			throw std::exception("������ɫ���ļ���ʧ��");

		char* pixelShaderBytes = new char[byteBufferSize];
		size_t pixelShaderBytesSize = fread_s(pixelShaderBytes, byteBufferSize, 1, byteBufferSize, pixelShaderFile);
		fclose(pixelShaderFile);

		//����������ɫ��
		assert(SUCCEEDED(device->CreatePixelShader(
			pixelShaderBytes,
			pixelShaderBytesSize,
			nullptr,
			pixelShader
		)));

		delete[] pixelShaderBytes;
	}

	void GL::CreateTexture2D(unsigned int width, unsigned int height, ID3D11Texture2D** texture, ID3D11ShaderResourceView** resourceView)
	{
		D3D11_TEXTURE2D_DESC textureDescription = CD3D11_TEXTURE2D_DESC(
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			width,
			height);
		textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		assert(SUCCEEDED(device->CreateTexture2D(&textureDescription, nullptr, texture)));

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceDescription{};
		resourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceDescription.Format = textureDescription.Format;
		resourceDescription.Texture2D = D3D11_TEX2D_SRV{};

		assert(SUCCEEDED(device->CreateShaderResourceView(*texture, nullptr, resourceView)));
	}
	void GL::CreateSamplerState(ID3D11SamplerState** samplerState)
	{
		D3D11_SAMPLER_DESC samplerDescription{};
		samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		device->CreateSamplerState(&samplerDescription, samplerState);
	}

	void GL::CreateBlendState(D3D11_BLEND sourceColor, D3D11_BLEND destinationColor, ID3D11BlendState** blendState)
	{
		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDescription{};
		renderTargetBlendDescription.BlendEnable = true;
		renderTargetBlendDescription.SrcBlend = sourceColor;
		renderTargetBlendDescription.DestBlend = destinationColor;
		renderTargetBlendDescription.BlendOp = D3D11_BLEND_OP_ADD;
		renderTargetBlendDescription.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDescription.DestBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDescription.BlendOpAlpha = D3D11_BLEND_OP_MAX;
		renderTargetBlendDescription.RenderTargetWriteMask = 15;

		D3D11_BLEND_DESC blendDescription{};
		blendDescription.RenderTarget[0] = renderTargetBlendDescription;

		HRESULT result = device->CreateBlendState(&blendDescription, blendState);
		assert(SUCCEEDED(result));
	}

	void GL::CreateDepthStencilState(float depthOffset, bool writeDepth, bool testDepth, ID3D11DepthStencilState** depthStencilState)
	{
		D3D11_DEPTH_STENCIL_DESC description{};
		description.DepthEnable = testDepth;
		description.DepthWriteMask = writeDepth ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		description.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		device->CreateDepthStencilState(&description, depthStencilState);
	}

	// ���õ�ǰ��Ⱦ���������ڵ���ɫ��
	void GL::SetShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexShaderInputLayout, ID3D11PixelShader* pixelShader)
	{
		context->IASetInputLayout(vertexShaderInputLayout);//�йض�����ɫ�������벼��
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);
	}
	// ���õ�ǰ��Ⱦ�����еĶ�����������
	void GL::SetBuffer(ID3D11Buffer* vertexBuffer, int vertexSize, ID3D11Buffer* indexBuffer, DXGI_FORMAT indexFormat)
	{
		//�󶨶�������
		UINT stride = vertexSize;
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		//����������
		context->IASetIndexBuffer(indexBuffer, indexFormat, 0);
	}
	// ���õ�ǰ��Ⱦ�����еĳ���������
	void GL::SetConstantBuffer(unsigned int startSlot, ID3D11Buffer** buffer)
	{
		context->VSSetConstantBuffers(startSlot, 1, buffer);
		context->PSSetConstantBuffers(startSlot, 1, buffer);
	}
	// ���õ�ǰ��Ⱦ�����е���ɫ����Դ
	void GL::SetShaderResource(unsigned int startSlot, ID3D11ShaderResourceView** resource)
	{
		context->VSSetShaderResources(startSlot, 1, resource);
		context->PSSetShaderResources(startSlot, 1, resource);
	}
	// ������ɫ����������ʱ�ķ�ʽ
	void GL::SetSamplerState(ID3D11SamplerState** samplerState)
	{
		context->VSSetSamplers(0, 1, samplerState);
		context->PSSetSamplers(0, 1, samplerState);
	}
	// ������Ⱦʱ����ɫ��Ϸ�ʽ
	void GL::SetBlendState(ID3D11BlendState* blendState)
	{
		//���û�Ϸ�ʽ
		context->OMSetBlendState(blendState, nullptr, 0xffffffff);
	}

	// ������Ⱦʱ�����ģ�建������Ӱ�췽ʽ
	void GL::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState)
	{
		context->OMSetDepthStencilState(depthStencilState, 0);
	}
	// ������Ⱦ����Ŀ������
	void GL::SetRenderTexture(ID3D11Texture2D* renderTargetTexture) {
		CComPtr<ID3D11Texture2D> defaultTexture;
		if (renderTargetTexture == nullptr)//δ��ʱʹ��Ĭ����Ⱦ����
		{
			swapChain->GetBuffer(0, IID_PPV_ARGS(&defaultTexture.p));
			renderTargetTexture = defaultTexture.p;
		}

		renderTargetView = nullptr;
		depthStencilView = nullptr;

		HRESULT result = 0;

		//��ȡ��Ⱦ�������
		CD3D11_TEXTURE2D_DESC colorTextureDescription;
		renderTargetTexture->GetDesc(&colorTextureDescription);
		//�Ըò��������������ģ����Ե��������
		CD3D11_TEXTURE2D_DESC depthStencilTextureDescription(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			static_cast<UINT> (colorTextureDescription.Width),
			static_cast<UINT> (colorTextureDescription.Height),
			1,
			0,
			D3D11_BIND_DEPTH_STENCIL
		);

		//������Ȳ�������
		CComPtr<ID3D11Texture2D> depthStencilTexture;
		assert(SUCCEEDED(device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture.p)));

		//������ȾĿ��������ͼ
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = colorTextureDescription.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		result = device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView.p);
		assert(SUCCEEDED(result));
		//�������ģ����Ե���ͼ
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		assert(SUCCEEDED(device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView.p)));

		//�����ӿ�������������ͼ��Сһ��
		D3D11_VIEWPORT viewport = {};
		viewport.Height = (float)colorTextureDescription.Height;
		viewport.Width = (float)colorTextureDescription.Width;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		context->RSSetViewports(1, &viewport);
	}

	void GL::Viewport(Rect rect)
	{
		//��ǰ�ͷţ���Ȼ����Ϊʹ���ж��޷����������С
		renderTargetView = nullptr;
		depthStencilView = nullptr;

		//������Ⱦ�����С
		swapChain->ResizeBuffers(2, (UINT)rect.width, (UINT)rect.height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

		SetRenderTexture(NULL);
	}
	void GL::Begin(D3D11_PRIMITIVE_TOPOLOGY drawMode)
	{
		//����ɫ��������ģ������õ���ͼ(Present�ᵼ�½���󶨣�����ÿ�ζ����°�)
		context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);

		//���û���ģʽ
		context->IASetPrimitiveTopology(drawMode);
	}
	void GL::End()
	{
		//��ʾ���
		swapChain->Present(0, 0);
	}

	void GL::Clear(bool clearDepth, bool clearColor, Color backgroundColor, float depth)
	{
		//���û�����ͼ
		if (clearColor == true)
		{
			const float teal[] = { backgroundColor.r, backgroundColor.g,backgroundColor.b, backgroundColor.a };
			context->ClearRenderTargetView(
				renderTargetView,
				teal
			);
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

	CComPtr<ID3D11Texture2D> GL::GetRenderTargetTexture() {
		CComPtr<ID3D11Texture2D> renderTargetTexture;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetTexture));
		return renderTargetTexture;
	}

	GL* GL::Initialize(HWND window)
	{
		GL::CreateDevice();
		GL::CreateSwapChain(window);
		SetRenderTexture(NULL);
		return new GL();
	}
}

