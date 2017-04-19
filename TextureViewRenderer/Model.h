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

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4X4 rotation;

	MeshDataHandler::MeshData * meshData;
	Material * material;
	DirectX::XMFLOAT4X4 TransformationMatrix;

	ID3D11Buffer * vertexBuffer;
	ID3D11Buffer * indexBuffer;

};

