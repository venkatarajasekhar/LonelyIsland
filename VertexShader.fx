cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
};

VS_OUTPUT main(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	float3 vLightDirection = (-1, 0, 0.25);
	float4 vLightColor = (1, 1, 1, 1);
	output.Color = saturate(dot((float3)vLightDirection, output.Pos*0.75f) * vLightColor);
	return output;
}