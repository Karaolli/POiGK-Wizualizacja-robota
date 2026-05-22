struct VS_IN
{
    float3 pos    : POSITION;
    float3 normal : NORMAL;
};

struct VS_OUT
{
    float4 pos      : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal   : TEXCOORD1;
};

cbuffer CBuf
{
    matrix transform;
    matrix world;
};

VS_OUT main(VS_IN input)
{
    VS_OUT output;
    
    output.pos = mul(float4(input.pos, 1.0f), transform);
    output.worldPos = mul(float4(input.pos, 1.0f), world).xyz;
    output.normal = mul(input.normal, (float3x3) world);
    
    return output;
}