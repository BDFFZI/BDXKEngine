#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"
#include "Matrix4x4.h"
#include "GL.h"

struct Vertex {
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Color color;
};

const D3D11_INPUT_ELEMENT_DESC VertexDescription[]{
   { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
   { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

struct WorldInfo {
	Matrix4x4 localToWorld;
	Matrix4x4 worldToCamera;
	Matrix4x4 cameraToView;
	Vector4 cameraPosition;
	Color environment;
};
static_assert((sizeof(WorldInfo) % 16) == 0, "������������С������16�ֽڵı���");


// LightInfo�д洢��ʱ���������Ϣ����ͬ�Ĺ��ռ���ʱ�Բ�����ʹ��������ͬ
enum class LightType {
	Directional,
	Point,
	Spot,
	Area
};

// ������Ⱦ�������羫�ȸ��ߵ����ع��գ����ʡ���ܵĶ������
enum class RenderMode {
	Auto,
	Important,
	NotImportant,
};


struct LightInfo {
	Vector4 lightPosition;
	Vector4 lightNormal;
	Color lightColor;
	LightType lightType;
	RenderMode lightRenderMode;
	int lightOrder;
	int space;
};
static_assert((sizeof(LightInfo) % 16) == 0, "������������С������16�ֽڵı���");