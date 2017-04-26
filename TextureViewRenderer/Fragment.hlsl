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
	
	//Ljusstr�len fr�n pixelns position till ljuset.
	//Normaliserat f�r att anv�ndas som en riktningsvektor
	float3 vRay = normalize((float3)(lightPos[0] - input.wPos));
	
	//Nu skapar vi en str�le fr�n pixelns position till kameran.
	float3 v = normalize(camPos.xyz - input.wPos.xyz).xyz;
	
	//Vi g�r reflect, detta anv�nds i specular shadingen.
	float3 r = reflect(-vRay, normalize(input.Normal));
	
	//vi ber�knar hur mycket av pixeln som ska lysas upp, dvs j�mf�ra vinkeln mellan ljuset och normalen. g�r det under 0 s� clampar vi till 0.
	
	float fDot = max(0.0f, dot(normalize(vRay), normalize(input.Normal)));
	
	
	float3 ambient = { 0.1f, 0.1f, 0.1f }; //ambientLight
	
	 
	float3 ks = diffuseColor[0].xyz; // Specular light f�rg
	float shinyPower = 20.0f;
	float3 lightColor = mul(diffuseColor[0].xyz, intensity[0]);
	
	//Ber�knar specular lighten (som �r v�ldigt cool)
	float3 specularLight = { ks * pow(max(dot(r, v), 0.0), shinyPower) };
	
	//Multiplicera med lampans ljus (f�rg/intensitet)
	specularLight = specularLight * lightColor[0] * fDot;
	
	//sampla fr�n texturen.
	float3 s = shaderTexture.Sample(SampleType, input.Texture).xyz;
	
	//Alpha, funkar inte nu, f�r jag har inte "aktiverat" att hantera alpha i cpun
	float alpha = 1.0f;
	
	
	//Texturen * skal�rprodukten av ljus/normal
	float3 diffuse = s.xyz  * fDot;
	//Multiplicerat med ljusets f�rg/intensitet
	diffuse = diffuse * lightColor[0];
	ambient = (s.xyz * ambient); // vi multiplicerar ambienten med texturen ocks�, s� texturen syns korrekt
	
	float4 col = {(ambient + diffuse + specularLight)  , alpha };
	
	
	return float4(input.TBN._m00_m01_m02, 1.0);
	return col;
};