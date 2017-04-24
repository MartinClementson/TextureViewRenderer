

struct VS_IN
{
	float3 Pos : POSITION;
	float2 Texture : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 Normal: NORMAL;
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
	
	
	
	return output;
}