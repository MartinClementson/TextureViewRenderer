#pragma once
#include "DataStructs.h"
#include "pch.h"
class MeshDataHandler
{
public:
	struct MeshData
	{
		MeshType type;
		VertexData * vertexData;
		unsigned short * indexData;
		unsigned short numVertices;
		unsigned short numIndices;

		ID3D11Buffer * vertexBuffer;
		ID3D11Buffer * indexBuffer;
	};


	 MeshDataHandler::MeshData * MeshDataHandler::GetMeshData(MeshType type);
	static MeshDataHandler * GetInstance();
	~MeshDataHandler();
private:
	MeshData m_meshData[NUM_MESH_TYPES];
	template <typename T, unsigned S>
	inline unsigned arraysize(const T(&v)[S]) { return S; }
	int MeshDataHandler::m_initialize(ID3D11Device * gDevice);
	MeshDataHandler();
};