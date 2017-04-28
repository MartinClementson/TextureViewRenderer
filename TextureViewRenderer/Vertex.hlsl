

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Texture : TEXCOORD0;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
};

struct VS_OUT
{
	float4 Pos : POSITION;
	float2 Texture : TEXCOORD0;
	float3 Normal: NORMAL;
	float3 Tangent : TANGENT;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.Pos = float4(input.Pos, 1.0);
	output.Texture = input.Texture;
	output.Normal = input.Normal;
	output.Tangent = input.Tangent;
	
	
	return output;
}