#include "Model.h"



Model::Model()
{
}

Model::Model(ID3D11Device * gDevice, MeshDataHandler::MeshData * meshData, Material * material)
{
	this->m_meshData = meshData;
	this->m_material = material;

	this->m_position = DirectX::XMFLOAT3(0.0, 0.0, 0.0);
	this->m_scale = DirectX::XMFLOAT3(1.0, 1.0, 1.0);

	DirectX::XMStoreFloat4x4(&this->m_rotation, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_TransformationMatrix, DirectX::XMMatrixIdentity());

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(VertexData) * meshData->numVertices;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = meshData->vertexData;
	vertexInitData.SysMemPitch = 0;
	vertexInitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	HRESULT hr = gDevice->CreateBuffer(&bufferDesc, &vertexInitData, &m_vertexBuffer);

	// Fill in a buffer description.
	bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned short) * meshData->numIndices;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA indexInitData;
	indexInitData.pSysMem = meshData->indexData;
	indexInitData.SysMemPitch = 0;
	indexInitData.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	hr = gDevice->CreateBuffer(&bufferDesc, &indexInitData, &m_indexBuffer);
}

MeshDataHandler::MeshData * Model::GetMeshData()
{
	return nullptr;
}

Material * Model::GetMaterial()
{
	return nullptr;
}

int Model::Update()
{
	DirectX::XMMATRIX transMat = DirectX::XMLoadFloat4x4(&this->m_rotation);

	transMat.r[0] = DirectX::XMVectorScale(transMat.r[0], this->m_scale.x);
	transMat.r[1] = DirectX::XMVectorScale(transMat.r[1], this->m_scale.y);
	transMat.r[2] = DirectX::XMVectorScale(transMat.r[2], this->m_scale.z);

	transMat.r[3] = DirectX::XMLoadFloat3(&this->m_position);

	DirectX::XMStoreFloat4x4(&this->m_TransformationMatrix, transMat);

	return 0;
}


int Model::SetPosition(DirectX::XMFLOAT3 position)
{
	this->m_position = position;

	return 0;
}

int Model::SetRotation(DirectX::XMFLOAT4X4 rotation)
{
	this->m_rotation = rotation;

	return 0;
}

int Model::SetScale(DirectX::XMFLOAT3 scale)
{
	this->m_scale = scale;

	return 0;
}

DirectX::XMFLOAT3 Model::GetPosition()
{
	return this->m_position;

}

DirectX::XMFLOAT3 Model::GetScale()
{
	return this->m_scale;

}

DirectX::XMFLOAT4X4 Model::GetRotation()
{
	return this->m_rotation;

}

Model::~Model()
{
}
