#pragma once
#include "pch.h"



	enum MeshType
	{
		PLANE,
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

// Used to send per-vertex data to the vertex shader.
struct VertexData
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};

struct Material
{
	std::string texturePath;
};

