#pragma once
#include "pch.h"

	enum MeshType
	{
		SPHERE,
		CUBE,
		COMPLEX,

		NUM_MESH_TYPES
	};
// Constant buffer used to send MVP matrices to the vertex shader.
struct ModelViewProjection
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

#define NUM_LIGHTS 3
struct LightBuffer
{
	DirectX::XMFLOAT4 lightPos[NUM_LIGHTS];
	DirectX::XMFLOAT4 diffuseColor[NUM_LIGHTS];
	float intensity[NUM_LIGHTS];
	float PADDING;
	DirectX::XMFLOAT4 camPos;
	float asdsadsa[16];
};

struct variablesBuffer
{
	int MIPBitPrecision[12];
};
// Used to send per-vertex data to the vertex shader.
struct VertexData
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 tangent;
};

struct Material
{
	std::string texturePath;
	ID3D11ShaderResourceView *m_TextureView = nullptr;
	ID3D11Resource*			textureResource = nullptr;
	Material() {};
	~Material()
	{
		m_TextureView   != nullptr ? m_TextureView->Release()   : NULL;
		textureResource != nullptr ? textureResource->Release() : NULL;
	}
};

