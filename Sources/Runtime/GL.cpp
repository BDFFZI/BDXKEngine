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
		assert(SUCCEEDED(result));// Direct3D设备创建是否成功
	}
	void GL::CreateSwapChain(HWND hwnd)
	{
		//获取底层DXGI的工厂
		CComPtr<IDXGIDevice> dxglDevice;
		CComPtr<IDXGIAdapter> dxglAdapter;
		CComPtr<IDXGIFactory2> dxglFactory;

		assert(SUCCEEDED(device->QueryInterface(&dxglDevice)));//获取DXGI设备
		assert(SUCCEEDED(dxglDevice->GetAdapter(&dxglAdapter)));//获取DXGI适配器
		assert(SUCCEEDED(dxglAdapter->GetParent(IID_PPV_ARGS(&dxglFactory)))); //获取DXGI工厂

		UINT m4xMsaaQuality;
		device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m4xMsaaQuality);
		assert(m4xMsaaQuality > 0);


		//交换链的属性配置
		DXGI_SWAP_CHAIN_DESC1 swapChainProperty = {};//局部变量一定要初始化
		swapChainProperty.BufferCount = 2;
		swapChainProperty.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainProperty.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainProperty.SampleDesc.Count = 1;
		swapChainProperty.SampleDesc.Quality = 0;
		swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
		fullScreenDesc.Windowed = true;

		//创建交换链
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
		//读取顶点着色器二进制代码
		FILE* vertexShaderFile;
		fopen_s(&vertexShaderFile, path, "rb");
		if (vertexShaderFile == 0)
			throw std::exception("顶点着色器文件打开失败");

		char* vertexShaderBytes = new char[byteBufferSize];
		size_t vertexShaderBytesSize = fread_s(vertexShaderBytes, byteBufferSize, 1, byteBufferSize, vertexShaderFile);
		fclose(vertexShaderFile);
		//创建顶点着色器
		assert(SUCCEEDED(device->CreateVertexShader(
			vertexShaderBytes,
			vertexShaderBytesSize,
			nullptr,
			vertexShader
		)));

		//创建语义信息
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
		//读取像素着色器二进制代码
		FILE* pixelShaderFile;
		fopen_s(&pixelShaderFile, path, "rb");
		if (pixelShaderFile == 0)
			throw std::exception("像素着色器文件打开失败");

		char* pixelShaderBytes = new char[byteBufferSize];
		size_t pixelShaderBytesSize = fread_s(pixelShaderBytes, byteBufferSize, 1, byteBufferSize, pixelShaderFile);
		fclose(pixelShaderFile);

		//创建像素着色器
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

	// 设置当前渲染管线中用于的着色器
	void GL::SetShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexShaderInputLayout, ID3D11PixelShader* pixelShader)
	{
		context->IASetInputLayout(vertexShaderInputLayout);//有关顶点着色器的输入布局
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);
	}
	// 设置当前渲染管线中的顶点索引数据
	void GL::SetBuffer(ID3D11Buffer* vertexBuffer, int vertexSize, ID3D11Buffer* indexBuffer, DXGI_FORMAT indexFormat)
	{
		//绑定顶点数据
		UINT stride = vertexSize;
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		//绑定索引数据
		context->IASetIndexBuffer(indexBuffer, indexFormat, 0);
	}
	// 设置当前渲染管线中的常量缓冲区
	void GL::SetConstantBuffer(unsigned int startSlot, ID3D11Buffer** buffer)
	{
		context->VSSetConstantBuffers(startSlot, 1, buffer);
		context->PSSetConstantBuffers(startSlot, 1, buffer);
	}
	// 设置当前渲染管线中的着色器资源
	void GL::SetShaderResource(unsigned int startSlot, ID3D11ShaderResourceView** resource)
	{
		context->VSSetShaderResources(startSlot, 1, resource);
		context->PSSetShaderResources(startSlot, 1, resource);
	}
	// 设置着色器采样纹理时的方式
	void GL::SetSamplerState(ID3D11SamplerState** samplerState)
	{
		context->VSSetSamplers(0, 1, samplerState);
		context->PSSetSamplers(0, 1, samplerState);
	}
	// 设置渲染时的颜色混合方式
	void GL::SetBlendState(ID3D11BlendState* blendState)
	{
		//设置混合方式
		context->OMSetBlendState(blendState, nullptr, 0xffffffff);
	}

	// 设置渲染时对深度模板缓冲区的影响方式
	void GL::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState)
	{
		context->OMSetDepthStencilState(depthStencilState, 0);
	}
	// 设置渲染到的目标纹理
	void GL::SetRenderTexture(ID3D11Texture2D* renderTargetTexture) {
		CComPtr<ID3D11Texture2D> defaultTexture;
		if (renderTargetTexture == nullptr)//未空时使用默认渲染纹理
		{
			swapChain->GetBuffer(0, IID_PPV_ARGS(&defaultTexture.p));
			renderTargetTexture = defaultTexture.p;
		}

		renderTargetView = nullptr;
		depthStencilView = nullptr;

		HRESULT result = 0;

		//获取渲染纹理参数
		CD3D11_TEXTURE2D_DESC colorTextureDescription;
		renderTargetTexture->GetDesc(&colorTextureDescription);
		//以该参数创建用于深度模板测试的纹理参数
		CD3D11_TEXTURE2D_DESC depthStencilTextureDescription(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			static_cast<UINT> (colorTextureDescription.Width),
			static_cast<UINT> (colorTextureDescription.Height),
			1,
			0,
			D3D11_BIND_DEPTH_STENCIL
		);

		//创建深度测试纹理
		CComPtr<ID3D11Texture2D> depthStencilTexture;
		assert(SUCCEEDED(device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture.p)));

		//创建渲染目标纹理视图
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = colorTextureDescription.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		result = device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView.p);
		assert(SUCCEEDED(result));
		//创建深度模板测试的视图
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		assert(SUCCEEDED(device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView.p)));

		//调整视口数据至与新视图大小一致
		D3D11_VIEWPORT viewport = {};
		viewport.Height = (float)colorTextureDescription.Height;
		viewport.Width = (float)colorTextureDescription.Width;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		context->RSSetViewports(1, &viewport);
	}

	void GL::Viewport(Rect rect)
	{
		//提前释放，不然会因为使用中而无法重置纹理大小
		renderTargetView = nullptr;
		depthStencilView = nullptr;

		//重置渲染纹理大小
		swapChain->ResizeBuffers(2, (UINT)rect.width, (UINT)rect.height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

		SetRenderTexture(NULL);
	}
	void GL::Begin(D3D11_PRIMITIVE_TOPOLOGY drawMode)
	{
		//绑定颜色输出和深度模板测试用的视图(Present会导致解除绑定，所以每次都重新绑定)
		context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);

		//设置绘制模式
		context->IASetPrimitiveTopology(drawMode);
	}
	void GL::End()
	{
		//显示结果
		swapChain->Present(0, 0);
	}

	void GL::Clear(bool clearDepth, bool clearColor, Color backgroundColor, float depth)
	{
		//重置绘制视图
		if (clearColor == true)
		{
			const float teal[] = { backgroundColor.r, backgroundColor.g,backgroundColor.b, backgroundColor.a };
			context->ClearRenderTargetView(
				renderTargetView,
				teal
			);
		}

		//重置深度模板测试视图
		if (clearDepth)
		{
			context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);
		}
	}
	void GL::Render(int indexsCount)
	{
		//开始绘制
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

