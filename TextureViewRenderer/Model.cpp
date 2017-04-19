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
}

MeshDataHandler::MeshData * Model::GetMeshData()
{
	return m_meshData;
}

Material * Model::GetMaterial()
{
	return m_material;
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
