#include "MeshDataHandler.h"
MeshDataHandler::MeshDataHandler()
{
}

int MeshDataHandler::m_initialize(ID3D11Device * gDevice)
{
	const unsigned int cubeIndices[] =
	{
		0,1,2,
		0,2,3,
		3,2,4,
		3,4,5,
		5,4,6,
		5,6,7,
		7,6,1,
		7,1,0,
		0,3,5,
		0,5,7,
		1,4,2,
		1,6,4
	};

	const unsigned int planeIndices[] =
	{
		0,1,2,
		0,2,3,
		3,2,4,
		3,4,5,
		5,4,6,
		5,6,7,
		7,6,1,
		7,1,0,
		0,3,5,
		0,5,7,
		1,4,2,
		1,6,4
	};

	const unsigned int complexIndices[] =
	{
		0,1,2,
		0,2,3,
		3,2,4,
		3,4,5,
		5,4,6,
		5,6,7,
		7,6,1,
		7,1,0,
		0,3,5,
		0,5,7,
		1,4,2,
		1,6,4
	};

	//cubeVerts[0].position = Float3(-0.5, 2.5,0.5);     //0
	//cubeVerts[1].position = Float3(-0.5, 0.0,0.5);     //1
	//cubeVerts[2].position = Float3(0.5, 0.0, 0.5);     //2
	//cubeVerts[3].position = Float3(0.5, 2.5, 0.5);     //3
	//cubeVerts[4].position = Float3(0.5, 0.0,-0.5);     //4
	//cubeVerts[5].position = Float3(0.5, 2.5,-0.5);     //5
	//cubeVerts[6].position = Float3(-0.5,0.0,-0.5);     //6
	//cubeVerts[7].position = Float3(-0.5,2.5,-0.5);     //7

	const VertexData cubeData[] =
	{
		{ DirectX::XMFLOAT3(-0.5, 2.5,0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5, 0.0,0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5, 0.0, 0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5, 2.5, 0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5, 0.0,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5, 2.5,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5,0.0,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5,2.5,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	const VertexData planeData[] =
	{
		{ DirectX::XMFLOAT3(-0.5, .5,0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5, -.5,0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5, -.5, 0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5, .5, 0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5, -.5,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5, .5,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5,-.5,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5,.5,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};


	const VertexData complexData[] =
	{
		{ DirectX::XMFLOAT3(-0.5, 2.5,0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5, 0.0,0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5, 0.0, 0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5, 2.5, 0.5),DirectX::XMFLOAT2(0.0f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT3(0.5, 0.0,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ DirectX::XMFLOAT3(0.5, 2.5,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(-0.5,0.0,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ DirectX::XMFLOAT3(-0.5,2.5,-0.5), DirectX::XMFLOAT2(0.0f,0.0f),DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	m_meshData[PLANE].vertexData = new VertexData[arraysize(planeData)];
	m_meshData[CUBE].vertexData = new VertexData[arraysize(cubeData)];
	m_meshData[COMPLEX].vertexData = new VertexData[arraysize(complexData)];

	m_meshData[PLANE].indexData = new unsigned int[arraysize(planeIndices)];
	m_meshData[CUBE].indexData = new unsigned int[arraysize(cubeIndices)];
	m_meshData[COMPLEX].indexData = new unsigned int[arraysize(complexIndices)];

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