#pragma once
#include "DataStructs.h"
#include "pch.h"
class MeshDataHandler
{
public:
	enum MeshType
	{
		PLANE,
		CUBE,
		COMPLEX,

		NUM_MESH_TYPES
	};
	struct MeshData
	{
		MeshType type;
		VertexData * vertexData;
		unsigned short * indexData;
		unsigned short numVertices;
		unsigned short numIndices;
	};

	MeshData meshData[NUM_MESH_TYPES];

	const MeshDataHandler::MeshData * MeshDataHandler::GetMeshData(MeshType type);
	static MeshDataHandler * GetInstance();
	~MeshDataHandler();
private:
	template <typename T, unsigned S>
	inline unsigned arraysize(const T(&v)[S]) { return S; }
	MeshDataHandler();
};