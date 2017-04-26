#define NUM_LIGHTS 3

cbuffer lightBuffer : register(b0)
{
	float4 lightPos[NUM_LIGHTS];
	float4 diffuseColor[NUM_LIGHTS];
	float  intensity[NUM_LIGHTS];
	float padding;
	float4 camPos;
};


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


float4 PS_main(PS_IN input)  : SV_Target
{ 
	//float4 s = float4(0.0f,0.0f,1.0f,1.0f);
	
	//Ljusstrålen från pixelns position till ljuset.
	//Normaliserat för att användas som en riktningsvektor
	float3 vRay = normalize((float3)(lightPos[0] - input.wPos));
	
	//Nu skapar vi en stråle från pixelns position till kameran.
	float3 v = normalize(camPos.xyz - input.wPos.xyz).xyz;
	
	//Vi gör reflect, detta används i specular shadingen.
	float3 r = reflect(-vRay, normalize(input.Normal));
	
	//vi beräknar hur mycket av pixeln som ska lysas upp, dvs jämföra vinkeln mellan ljuset och normalen. går det under 0 så clampar vi till 0.
	
	float fDot = max(0.0f, dot(normalize(vRay), normalize(input.Normal)));
	
	
	float3 ambient = { 0.1f, 0.1f, 0.1f }; //ambientLight
	
	 
	float3 ks = diffuseColor[0].xyz; // Specular light färg
	float shinyPower = 20.0f;
	float3 lightColor = mul(diffuseColor[0].xyz, intensity[0]);
	
	//Beräknar specular lighten (som är väldigt cool)
	float3 specularLight = { ks * pow(max(dot(r, v), 0.0), shinyPower) };
	
	//Multiplicera med lampans ljus (färg/intensitet)
	specularLight = specularLight * lightColor[0] * fDot;
	
	//sampla från texturen.
	float3 s = shaderTexture.Sample(SampleType, input.Texture).xyz;
	
	//Alpha, funkar inte nu, för jag har inte "aktiverat" att hantera alpha i cpun
	float alpha = 1.0f;
	
	
	//Texturen * skalärprodukten av ljus/normal
	float3 diffuse = s.xyz  * fDot;
	//Multiplicerat med ljusets färg/intensitet
	diffuse = diffuse * lightColor[0];
	ambient = (s.xyz * ambient); // vi multiplicerar ambienten med texturen också, så texturen syns korrekt
	
	float4 col = {(ambient + diffuse + specularLight)  , alpha };
	
	
	return float4(input.TBN._m00_m01_m02, 1.0);
	return col;
};