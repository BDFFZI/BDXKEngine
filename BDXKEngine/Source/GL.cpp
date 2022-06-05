#include "GL.h"
#include"Assert.h"
#include"Debug.h"

CComPtr<ID3D11Device> GL::device;
CComPtr<ID3D11DeviceContext> GL::context;
CComPtr<IDXGISwapChain> GL::swapChain;


CComPtr<ID3D11RenderTargetView> GL::renderTargetView;
CComPtr<ID3D11DepthStencilView> GL::depthStencilView;


void GL::CreateDevice() {
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_0,
	};

	HRESULT result = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&device,
		NULL,
		&context
	);
	Assert::IsSucceeded(result, L"Direct3D设备创建失败");
}
void GL::CreateSwapChain(HWND hwnd)
{
	//获取底层DXGI的工厂
	CComPtr<IDXGIDevice> dxglDevice;
	CComPtr<IDXGIAdapter> dxglAdapter;
	CComPtr<IDXGIFactory> dxglFactory;

	Assert::IsSucceeded(device->QueryInterface(&dxglDevice), L"DXGI设备获取失败");
	Assert::IsSucceeded(dxglDevice->GetAdapter(&dxglAdapter), L"DXGI适配器获取失败");
	Assert::IsSucceeded(dxglAdapter->GetParent(IID_PPV_ARGS(&dxglFactory)), L"DXGI工厂获取失败");

	//交换链的属性配置
	DXGI_SWAP_CHAIN_DESC swapChainProperty = {};//局部变量一定要初始化
	swapChainProperty.Windowed = TRUE;
	swapChainProperty.OutputWindow = hwnd;
	swapChainProperty.BufferCount = 2;
	swapChainProperty.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainProperty.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainProperty.SampleDesc.Count = 1;
	swapChainProperty.SampleDesc.Quality = 0;
	swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	//创建交换链
	HRESULT result = dxglFactory->CreateSwapChain(device, &swapChainProperty, &swapChain);
	Assert::IsSucceeded(result, L"Direct3D交换链创建失败");
}
void GL::CreateRenderTexture()
{
	CComPtr<ID3D11Texture2D> colorTexture;
	CComPtr<ID3D11Texture2D> depthStencilTexture;

	//获取颜色缓冲区并创建渲染视图为颜色缓冲区
	swapChain->GetBuffer(0, IID_PPV_ARGS(&colorTexture));
	Assert::IsSucceeded(
		device->CreateRenderTargetView(colorTexture, nullptr, &renderTargetView),
		L"创建渲染视图失败"
	);
	//获取颜色缓冲区参数
	CD3D11_TEXTURE2D_DESC colorTextureDescription;
	colorTexture->GetDesc(&colorTextureDescription);
	//以该参数创建深度模板缓冲区
	CD3D11_TEXTURE2D_DESC depthStencilTextureDescription(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (colorTextureDescription.Width),
		static_cast<UINT> (colorTextureDescription.Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);
	Assert::IsSucceeded(
		device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture),
		L"创建深度测试纹理失败"
	);
	//并将其设为深度模板测试的视图
	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	Assert::IsSucceeded(
		device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView),
		L"创建深度测试视图失败"
	);

	D3D11_VIEWPORT viewport = {};
	viewport.Height = (float)colorTextureDescription.Height;
	viewport.Width = (float)colorTextureDescription.Width;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	context->RSSetViewports(1, &viewport);
}

void GL::CreateVertexShader(const char* path, D3D11_INPUT_ELEMENT_DESC inputLayoutDescriptor[], int inputLayoutDescriptorCount, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout)
{
	const int byteBufferSize = 32 * 1024;
	//读取顶点着色器二进制代码
	FILE* vertexShaderFile;
	fopen_s(&vertexShaderFile, path, "rb");
	Assert::IsTrue(vertexShaderFile != nullptr, L"读取顶点着色器文件失败");
	if (vertexShaderFile == 0)
		throw "为了消除编辑器警告";

	char* vertexShaderBytes = new char[byteBufferSize];
	size_t vertexShaderBytesSize = fread_s(vertexShaderBytes, byteBufferSize, 1, byteBufferSize, vertexShaderFile);
	fclose(vertexShaderFile);
	//创建顶点着色器
	Assert::IsSucceeded(device->CreateVertexShader(
		vertexShaderBytes,
		vertexShaderBytesSize,
		nullptr,
		vertexShader
	), L"创建顶点着色器失败");

	//创建语义信息
	Assert::IsSucceeded(device->CreateInputLayout(
		inputLayoutDescriptor,
		inputLayoutDescriptorCount,
		vertexShaderBytes,
		vertexShaderBytesSize,
		inputLayout
	), L"声明语义信息失败");

	delete[] vertexShaderBytes;
}
void GL::CreatePixelShader(const char* path, ID3D11PixelShader** pixelShader)
{
	const int byteBufferSize = 32 * 1024;
	//读取像素着色器二进制代码
	FILE* pixelShaderFile;
	fopen_s(&pixelShaderFile, "C:/Users/BDFFZI/Desktop/BDXKEngine/Shader/PixelShader.cso", "rb");
	Assert::IsTrue(pixelShaderFile != nullptr, L"读取像素着色器文件失败");
	if (pixelShaderFile == 0)
		throw "为了消除编辑器警告";

	char* pixelShaderBytes = new char[byteBufferSize];
	size_t pixelShaderBytesSize = fread_s(pixelShaderBytes, byteBufferSize, 1, byteBufferSize, pixelShaderFile);
	fclose(pixelShaderFile);
	//创建像素着色器
	Assert::IsSucceeded(device->CreatePixelShader(
		pixelShaderBytes,
		pixelShaderBytesSize,
		nullptr,
		pixelShader
	), L"创建像素着色器失败");

	delete[] pixelShaderBytes;
}

void GL::Clear(Color color)
{
	const float teal[] = { color.r, color.g,color.b, color.a };
	context->ClearRenderTargetView(
		renderTargetView,
		teal
	);
}
void GL::Render(
	ID3D11Buffer* vertexBuffer, ID3D11InputLayout* inputLayout, int vertexSize,
	ID3D11Buffer* indexBuffer, DXGI_FORMAT indexFormat, int indexsCount,
	ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader,
	D3D11_PRIMITIVE_TOPOLOGY drawMode)
{
	//绑定顶点数据
	UINT stride = vertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	//绑定顶点信息
	context->IASetInputLayout(inputLayout);

	//绑定索引数据
	context->IASetIndexBuffer(indexBuffer, indexFormat, 0);

	//绑定着色器
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	//设置绘制目标
	context->OMSetRenderTargets(1, &(renderTargetView.p), NULL);

	//设置绘制模式
	context->IASetPrimitiveTopology(drawMode);

	//开始绘制
	context->DrawIndexed(indexsCount, 0, 0);

	//显示结果
	swapChain->Present(0, 0);

	//D3D11_VIEWPORT viewport;
	//ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.Width = 800;
	//viewport.Height = 600;
	//context->RSSetViewports(1, &viewport);
}

