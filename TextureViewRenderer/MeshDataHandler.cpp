#include "MeshDataHandler.h"
#include "ObjLoader.h"
MeshDataHandler::MeshDataHandler()
{
}

int MeshDataHandler::m_calcNormals(VertexData *& vData)
{
	return 0;
}

int MeshDataHandler::m_initialize(ID3D11Device * gDevice)
{
	ObjLoader objloader;
	
	VertexData * complexData;
	unsigned int * complexIndices;
	unsigned int cvCount, ciCount;
	objloader.loadObj("meshes/stanford-bunny.obj", complexData, complexIndices, cvCount, ciCount, 0.005, false);

	m_meshData[COMPLEX].numVertices = cvCount;
	m_meshData[COMPLEX].numIndices = ciCount;
	m_meshData[COMPLEX].indexData = complexIndices;
	m_meshData[COMPLEX].vertexData = complexData;

	VertexData * cubeData;
	unsigned int * cubeIndices;
	objloader.loadObj("meshes/cube.obj", cubeData, cubeIndices, cvCount, ciCount);
	m_meshData[CUBE].numVertices = cvCount;
	m_meshData[CUBE].numIndices = ciCount;
	m_meshData[CUBE].indexData = cubeIndices;
	m_meshData[CUBE].vertexData = cubeData;

	VertexData * planeData;
	unsigned int * planeIndices;
	objloader.loadObj("meshes/plane.obj", planeData, planeIndices, cvCount, ciCount);
	m_meshData[PLANE].numVertices = cvCount;
	m_meshData[PLANE].numIndices = ciCount;
	m_meshData[PLANE].indexData = planeIndices;
	m_meshData[PLANE].vertexData = planeData;
	

	for (int i = 0; i < NUM_MESH_TYPES; i++)
	{
		
	
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VertexData) * m_meshData[i].numVertices;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA vertexInitData;
		vertexInitData.pSysMem = m_meshData[i].vertexData;
		vertexInitData.SysMemPitch = 0;
		vertexInitData.SysMemSlicePitch = 0;

		// Create the vertex buffer.
		HRESULT hr = gDevice->CreateBuffer(&bufferDesc, &vertexInitData, &m_meshData[i].vertexBuffer);
		if (hr != S_OK)
			return 0;
		// Fill in a buffer description.
		bufferDesc = { 0 };
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(unsigned int) * m_meshData[i].numIndices;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		// Define the resource data.
		D3D11_SUBRESOURCE_DATA indexInitData;
		indexInitData.pSysMem = m_meshData[i].indexData;
		indexInitData.SysMemPitch = 0;
		indexInitData.SysMemSlicePitch = 0;

		// Create the buffer with the device.
		hr = gDevice->CreateBuffer(&bufferDesc, &indexInitData, &m_meshData[i].indexBuffer);
		if (hr != S_OK)
			return 0;
	}

	return 1;
}

 MeshDataHandler::MeshData * MeshDataHandler::GetMeshData(MeshType type)
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