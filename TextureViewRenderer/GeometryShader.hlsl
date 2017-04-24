cbuffer WorldViewProjectionConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;

};

struct GSInput
{
	float4 pos :SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 normal: NORMAL;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 normal : NORMAL;
	float4 wPos : WORLDPOS;
};


[maxvertexcount(3)]
void GS_main(
	triangle GSInput input[3] ,
	inout TriangleStream< GSOutput > output
	
)
{
	//GSOutput element;
	float3 faceEdgeA = input[1].pos - input[0].pos;
	float3 faceEdgeB = input[2].pos - input[0].pos;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
	
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		
		element.normal = mul(transpose(World),faceNormal );
		element.normal = normalize(element.normal);

	
		element.pos = mul(input[i].pos, transpose(World));
		element.wPos = element.pos;
		element.pos = mul(element.pos, transpose(View));
		element.pos = mul(element.pos, transpose(Projection));
		element.Texture = input[i].Texture;
		output.Append(element);
	}

}