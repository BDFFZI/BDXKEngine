cbuffer MatrixBuffer : register(b0)
{
    row_major float4x4 LocalToWorld;
    row_major float4x4 WorldToCamera;
    row_major float4x4 CameraToView;
};

struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};