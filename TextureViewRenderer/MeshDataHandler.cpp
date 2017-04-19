#include "MeshDataHandler.h"

MeshDataHandler::MeshDataHandler()
{
	const unsigned short cubeIndices[] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	const unsigned short planeIndices[] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	const unsigned short complexIndices[] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		2,6,7, // +y
		2,7,3,

		0,4,6, // -z
		0,6,2,

		1,3,7, // +z
		1,7,5,
	};

	const VertexData cubeData[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	const VertexData planeData[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	const VertexData complexData[] =
	{
		{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f, -0.5f,  0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5f,  0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5f,  0.5f,  0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	m_meshData[PLANE].vertexData = new VertexData[arraysize(planeData)];
	m_meshData[CUBE].vertexData = new VertexData[arraysize(cubeData)];
	m_meshData[COMPLEX].vertexData = new VertexData[arraysize(complexData)];

	m_meshData[PLANE].indexData = new unsigned short[arraysize(planeIndices)];
	m_meshData[CUBE].indexData = new unsigned short[arraysize(cubeIndices)];
	m_meshData[COMPLEX].indexData = new unsigned short[arraysize(complexIndices)];

	for (int i = 0; i < arraysize(planeData); i++)
	{
		m_meshData[PLANE].vertexData[i] = planeData[i];
	}
	for (int i = 0; i < arraysize(cubeData); i++)
	{
		m_meshData[CUBE].vertexData[i] = cubeData[i];
	}
	for (int i = 0; i < arraysize(complexData); i++)
	{
		m_meshData[COMPLEX].vertexData[i] = complexData[i];
	}

	for (int i = 0; i < arraysize(planeIndices); i++)
	{
		m_meshData[PLANE].indexData[i] = planeIndices[i];
	}

	for (int i = 0; i < arraysize(cubeIndices); i++)
	{
		m_meshData[CUBE].indexData[i] = cubeIndices[i];
	}

	for (int i = 0; i < arraysize(complexIndices); i++)
	{
		m_meshData[COMPLEX].indexData[i] = complexIndices[i];
	}

	m_meshData[PLANE].numVertices = arraysize(planeData);
	m_meshData[CUBE].numVertices = arraysize(cubeData);
	m_meshData[COMPLEX].numVertices = arraysize(complexData);

	m_meshData[PLANE].numIndices = arraysize(planeIndices);
	m_meshData[CUBE].numIndices = arraysize(cubeIndices);
	m_meshData[COMPLEX].numIndices = arraysize(complexIndices);
}

const MeshDataHandler::MeshData * MeshDataHandler::GetMeshData(MeshType type)
{
	return &m_meshData[type];
}

MeshDataHandler * MeshDataHandler::GetInstance()
{
	static MeshDataHandler instance;
	return &instance;
}

MeshDataHandler::~MeshDataHandler()
{
	for (MeshDataHandler::MeshData data : m_meshData)
	{
		delete[] data.indexData;
		delete[] data.vertexData;


	}
}