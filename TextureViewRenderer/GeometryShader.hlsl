cbuffer WorldViewProjectionConstantBuffer : register(b1)
{
	matrix World;
	matrix View;
	matrix Projection;

};

struct GSInput
{
	float4 pos :POSITION;
	float2 Texture : TEXCOORD0;
	float3 normal: NORMAL;
	float3 tangent : TANGENT;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 normal : NORMAL;
	float4 wPos : WORLDPOS;
	float3x3 TBN : TANGENTMATRIX;
};

float3x3 TBN(float3 normal, float4 tangent) 
{
	float handedNess = tangent.w;
	tangent.xyz -= dot(tangent.xyz, normal) * normal;
	float3 B = -cross(normal* handedNess, tangent.xyz ) ; //Bitangent

	return float3x3(tangent.xyz, B.xyz, normal.xyz); //return tangent space matrix
}


[maxvertexcount(3)]
void GS_main(
	triangle GSInput input[3] ,
	inout TriangleStream< GSOutput > output
	
)
{
	//GSOutput element;
	//float3 faceEdgeA = input[1].pos - input[0].pos;
	//float3 faceEdgeB = input[2].pos - input[0].pos;
	//float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));
	//faceNormal = mul(transpose(World), faceNormal);
	//faceNormal = normalize(faceNormal);
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		
		element.normal = float3(mul(float4(input[i].normal, 0.0), transpose(World)).xyz);
	
		element.pos = float4(mul(input[i].pos, transpose(World)).xyz, 1.0);
		element.wPos = element.pos;
		element.pos = mul(element.pos, transpose(View));
		element.pos = mul(element.pos, transpose(Projection));
		element.Texture = input[i].Texture;
		element.TBN = TBN(element.normal, mul(input[i].tangent, transpose(World)));

		output.Append(element);
	}

	output.RestartStrip();
}