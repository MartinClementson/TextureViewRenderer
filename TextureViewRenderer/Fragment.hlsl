#define NUM_LIGHTS 3

cbuffer lightBuffer : register(b0)
{
	float4 lightPos[NUM_LIGHTS];
	float4 diffuseColor[NUM_LIGHTS];
	float  intensity[NUM_LIGHTS];
	float padding;
	float4 camPos;
};

cbuffer variables : register(b2)
{
	int MIPBitPrecision[12];
}

SamplerState SampleType :register(s0); //modifies how the pixels are written to the polygon face when shaded
Texture2D shaderTexture :register(t0);

struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 Texture : TEXCOORD0;
	float3 Normal : NORMAL;
	float4 wPos :  WORLDPOS;
	float3x3 TBN : TANGENTMATRIX;
};

float MipLevel(float2 uv, float texture_width, float texture_height, int mipCount)
{
	float2 dx = ddx(uv * texture_width);
	float2 dy = ddy(uv * texture_height);
	float d = max(dot(dx, dx), dot(dy, dy));

	// Clamp the value to the max mip level counts
	const float rangeClamp = pow(2.0, (mipCount - 1) * 2.0);
	d = clamp(d, 1.0, rangeClamp);

	float mipLevel = 0.5 * log2(d);
	mipLevel = floor(mipLevel); //uncomment to get integer value of the closest level

	return mipLevel;
}

float mip_map_level(in float2 texture_coordinate)
{
	// The OpenGL Graphics System: A Specification 4.2
	//  - chapter 3.9.11, equation 3.21


	float2  dx_vtc = ddx(texture_coordinate);
	float2  dy_vtc = ddy(texture_coordinate);
	float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));


	//return max(0.0, 0.5 * log2(delta_max_sqr) - 1.0); // == log2(sqrt(delta_max_sqr));
	return 0.5 * log2(delta_max_sqr); // == log2(sqrt(delta_max_sqr));
}

float3 quantize(float3 color, int precision)
{
	int tPrecision = ((8.0f * precision)/3) - 1;
	float3 tColor = color;

	tColor.x = floor(tPrecision * tColor.x) / tPrecision;
	tColor.y = floor(tPrecision * tColor.y) / tPrecision;
	tColor.z = floor(tPrecision * tColor.z) / tPrecision;

	return tColor;
}

float4 PS_main(PS_IN input)  : SV_Target
{ 
	int bitPrecision = 0;

	float3 normalSamp = normalize(mul(2.0f * shaderTexture.Sample(SampleType, input.Texture).rgb - 1.0f, input.TBN));


	float texture_height = 0.0f, texture_width = 0.0f, elements = 0.0f, mipLevels = 0.0f;
	shaderTexture.GetDimensions(0, texture_width, texture_height, mipLevels);
	
	//float mipLevel = MipLevel(input.Texture, texture_width, texture_height, mipLevels);
	float mipLevel = mip_map_level(input.Texture);
	float rangedMip;

	rangedMip = (((mipLevel - 1.0f)) / (mipLevels - 1.0f));
	//rangedMip *= -1.0f;

	float4 mipVisualizer = float4(rangedMip, rangedMip, rangedMip, 1.0f);


	bitPrecision = MIPBitPrecision[floor(mipLevel)];


	//float4 s = float4(0.0f,0.0f,1.0f,1.0f);
	
	//Ljusstrålen från pixelns position till ljuset.
	//Normaliserat för att användas som en riktningsvektor
	float3 vRay = normalize((float3)(lightPos[0] - input.wPos));
	
	//Nu skapar vi en stråle från pixelns position till kameran.
	float3 v = normalize(camPos.xyz - input.wPos.xyz).xyz;
	
	//Vi gör reflect, detta används i specular shadingen.
	float3 r = reflect(-vRay, normalize(input.Normal));
	
	//vi beräknar hur mycket av pixeln som ska lysas upp, dvs jämföra vinkeln mellan ljuset och normalen. går det under 0 så clampar vi till 0.
	
	float fDot = max(0.0f, dot(normalize(vRay), normalSamp));
	
	
	float3 ambient = { 0.1f, 0.1f, 0.1f }; //ambientLight
	
	 
	float3 ks = diffuseColor[0].xyz; // Specular light färg
	float shinyPower = 20.0f;
	float3 lightColor = mul(diffuseColor[0].xyz, intensity[0]);
	
	//Beräknar specular lighten (som är väldigt cool)
	float3 specularLight = { ks * pow(max(dot(r, v), 0.0), shinyPower) };
	
	//Multiplicera med lampans ljus (färg/intensitet)
	specularLight = specularLight * lightColor[0] * fDot;
	
	//sampla från texturen.
	float3 s = float3(0.5, 0.5, 0.5);// shaderTexture.Sample(SampleType, input.Texture).xyz;
	
	//Alpha, funkar inte nu, för jag har inte "aktiverat" att hantera alpha i cpun
	float alpha = 1.0f;
	
	
	//Texturen * skalärprodukten av ljus/normal
	float3 diffuse = s.xyz  * fDot;
	//Multiplicerat med ljusets färg/intensitet
	diffuse = diffuse * lightColor[0];
	ambient = (s.xyz * ambient); // vi multiplicerar ambienten med texturen också, så texturen syns korrekt
	
	float3 finalColor = ambient + diffuse + specularLight;

	float4 col = {(ambient + diffuse + specularLight), alpha };
	finalColor = quantize(finalColor, bitPrecision);
	
	//return float4(input.TBN._m00_m01_m02, 1.0); //printing tangent
	//return mipVisualizer;
	//return float4(finalColor, 1.0);
	return col;
};