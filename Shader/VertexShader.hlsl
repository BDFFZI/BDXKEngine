//struct VOut
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//};

//VOut main(float4 position : POSITION, float4 color : COLOR)
//{
//    VOut output;

//    output.position = position;
//    output.color = color;

//    return output;
//}

float4 main(float3 position : POSITION) : SV_POSITION
{
    return float4(position, 1);
}