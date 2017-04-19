#pragma once
#include "pch.h"

class Camera
{
public:
	Camera();
	~Camera();

	int SetPosition(DirectX::XMFLOAT3 pos);
	int SetViewTarget(DirectX::XMFLOAT3 viewTarget);
	int SetFoV(float fov);
	int SetWindowHW(float height, float width);
	int SetFarPlane(float farPlane);
	int SetNearPlane(float nearPlane);

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetViewTarget();
	float GetFarPlane();
	float GetNearPlane();
	float GetFoV();


private:
	DirectX::XMFLOAT4X4 m_viewMatrix, m_projectionMatrix;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_viewTarget;
	DirectX::XMFLOAT3 m_up;

	float m_FoV, m_farPlane, m_nearPlane;


	int m_updateViewMatrix();
	int m_updateProjectionMatrix();

};

