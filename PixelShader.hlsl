struct VS_OUT
{
    float4 pos      : SV_POSITION;
    float3 worldPos : TEXCOORD0;
    float3 normal   : TEXCOORD1;
};

cbuffer ConstantBuffer : register(b1)
{
    float3 lightPos;
    float pad1;
    float3 lightColor;
    float pad2;
    float3 cameraPos;
    float pad3;
};

float4 main(VS_OUT input) : SV_Target
{
    float3 N = normalize(input.normal);
    float3 L = normalize(lightPos - input.worldPos);
    float3 V = normalize(cameraPos - input.worldPos);
    float3 R = reflect(-L, N);
    
    float3 baseColor = float3(1.0f, 0.5f, 0.5f);
    
    float3 ambient = 0.15f * baseColor;
    
    float diff = max(dot(N, L), 0.0);
    float3 diffuse = diff * lightColor * baseColor;
    
    float spec = pow(max(dot(R, V), 0.0), 32.0);
    float3 specular = spec * lightColor;
    
    return float4(ambient + diffuse + specular, 1.0);
}