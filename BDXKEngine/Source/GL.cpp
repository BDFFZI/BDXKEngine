#include "GL.h"

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
	assert(SUCCEEDED(result));// Direct3D设备创建是否成功
}
void GL::CreateSwapChain(HWND hwnd)
{
	//获取底层DXGI的工厂
	CComPtr<IDXGIDevice> dxglDevice;
	CComPtr<IDXGIAdapter> dxglAdapter;
	CComPtr<IDXGIFactory> dxglFactory;

	assert(SUCCEEDED(device->QueryInterface(&dxglDevice)));//获取DXGI设备
	assert(SUCCEEDED(dxglDevice->GetAdapter(&dxglAdapter)));//获取DXGI适配器
	assert(SUCCEEDED(dxglAdapter->GetParent(IID_PPV_ARGS(&dxglFactory)))); //获取DXGI工厂

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
	assert(SUCCEEDED(dxglFactory->CreateSwapChain(device, &swapChainProperty, &swapChain)));
}
void GL::CreateRenderTexture()
{
	//获取交换链中渲染纹理
	CComPtr<ID3D11Texture2D> colorTexture;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&colorTexture));

	//创建颜色输出视图
	assert(SUCCEEDED(device->CreateRenderTargetView(colorTexture, nullptr, &renderTargetView)));

	//获取渲染纹理参数
	CD3D11_TEXTURE2D_DESC colorTextureDescription;
	colorTexture->GetDesc(&colorTextureDescription);

	//以该参数创建用于深度模板测试的纹理
	CD3D11_TEXTURE2D_DESC depthStencilTextureDescription(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (colorTextureDescription.Width),
		static_cast<UINT> (colorTextureDescription.Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);

	//创建深度测试纹理
	CComPtr<ID3D11Texture2D> depthStencilTexture;
	assert(SUCCEEDED(device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture)));

	//创建深度模板测试的视图
	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	assert(SUCCEEDED(device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView)));

	//设置视口数据
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
	if (vertexShaderFile == 0)
		throw "顶点着色器文件打开失败";

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
		throw "像素着色器文件打开失败";

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

void GL::Clear(Color color, float depth, unsigned char stencil)
{
	const float teal[] = { color.r, color.g,color.b, color.a };
	//重置绘制视图
	context->ClearRenderTargetView(
		renderTargetView,
		teal
	);
	//重置深度模板测试视图
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
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

	//设置绘制模式
	context->IASetPrimitiveTopology(drawMode);

	//绑定颜色输出和深度模板测试用的视图(Present会导致解除绑定，所有每次都重新绑定)
	context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);

	//开始绘制
	context->DrawIndexed(indexsCount, 0, 0);

	//显示结果
	swapChain->Present(0, 0);
}

