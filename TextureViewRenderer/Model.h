#pragma once
#include "MeshDataHandler.h"

class Model
{
public:
	Model(ID3D11Device * gDevice, MeshDataHandler::MeshData * data, Material * material);
	MeshDataHandler::MeshData * GetMeshData();
	Material * GetMaterial();

	int Update(); //updates transform matrix with current set pos/rot/scale

	int SetPosition(DirectX::XMFLOAT3 position);
	int SetRotation(DirectX::XMFLOAT4X4 rotation);
	int SetScale(DirectX::XMFLOAT3 scale);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT4X4 GetRotation();

	~Model();
private:
	Model();

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT4X4 m_rotation;

	MeshDataHandler::MeshData * m_meshData;
	Material * m_material;
	DirectX::XMFLOAT4X4 m_TransformationMatrix;

	ID3D11Buffer * m_vertexBuffer;
	ID3D11Buffer * m_indexBuffer;

};

