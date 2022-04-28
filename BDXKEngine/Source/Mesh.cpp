#include "Mesh.h"
#include "Vector3.h"

Mesh::Mesh()
{
	Vector3 vertices[] =
	{
		{ 0, 0, 0 },
		{ 0,0.5f,0 },
		{ 0.5f,0, 0 },
	};
	UINT triangles[]{
		0,1,2,
	};

	GL::CreateVertexBuffer(vertices, sizeof(vertices), &vertexBuffer);
	GL::CreateIndexBuffer(triangles, sizeof(triangles), &indexBuffer);

	D3D11_INPUT_ELEMENT_DESC vertexShaderInput[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	GL::CreateVertexShader("C:/Users/BDFFZI/Desktop/BDXKEngine/Shader/VertexShader.cso", vertexShaderInput, ARRAYSIZE(vertexShaderInput), &vertexShader, &inputLayout);
	GL::CreatePixelShader("C:/Users/BDFFZI/Desktop/BDXKEngine/Shader/PixelShader.cso", &pixelShader);
}

void Mesh::Render()
{
	GL::Render(vertexBuffer, inputLayout, sizeof(Vector3), indexBuffer, DXGI_FORMAT_R32_UINT, 3, vertexShader, pixelShader, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
