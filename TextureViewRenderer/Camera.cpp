#include "Camera.h"

Camera::Camera()
{
	DirectX::XMStoreFloat4x4(&this->m_viewMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&this->m_projectionMatrix, DirectX::XMMatrixIdentity());

	this->m_position = DirectX::XMFLOAT3(0.0, 0.0, 0.0);
	this->m_viewTarget = DirectX::XMFLOAT3(0.0, 0.0, 1.0);
	this->m_up = DirectX::XMFLOAT3(0.0, 1.0, 0.0);

	this->m_FoV = 70.0f;
	this->m_nearPlane = 0.1f;
	this->m_farPlane = 1000.0f;
	this->m_wWidth = WINDOW_WIDTH;
	this->m_wHeight = WINDOW_HEIGHT;
	m_updateViewMatrix();
	m_updateProjectionMatrix();
}


Camera::~Camera()
{
}

int Camera::SetPosition(DirectX::XMFLOAT3 pos)
{
	this->m_position = pos;
	m_updateViewMatrix();
	return 0;
}

int Camera::SetViewTarget(DirectX::XMFLOAT3 viewTarget)
{
	this->m_viewTarget = viewTarget;
	m_updateViewMatrix();
	return 0;
}

int Camera::SetFoV(float fov)
{
	this->m_FoV = fov;
	return 0;
	m_updateProjectionMatrix();
}

int Camera::SetWindowHW(float height, float width)
{
	return 0;
}

int Camera::SetFarPlane(float farPlane)
{
	this->m_farPlane = farPlane;
	m_updateProjectionMatrix();
	return 0;
}

int Camera::SetNearPlane(float nearPlane)
{
	this->m_nearPlane = nearPlane;
	m_updateProjectionMatrix();
	return 0;
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return this->m_viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return this->m_projectionMatrix;
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return this->m_position;
}

DirectX::XMFLOAT3 Camera::GetViewTarget()
{
	return this->m_viewTarget;
}

float Camera::GetFarPlane()
{
	return this->m_farPlane;
}

float Camera::GetNearPlane()
{
	return this->m_nearPlane;
}

float Camera::GetFoV()
{
	return m_farPlane;
}

int Camera::m_updateViewMatrix()
{
	
	DirectX::XMStoreFloat4x4(
		&this->m_viewMatrix,
		DirectX::XMMatrixLookAtLH(
			DirectX::XMLoadFloat3(&m_position),
			DirectX::XMLoadFloat3(&m_viewTarget),
			DirectX::XMLoadFloat3(&m_up)
		)
	);


	return 0;
}

int Camera::m_updateProjectionMatrix()
{
	DirectX::XMStoreFloat4x4(
		&this->m_projectionMatrix,
		DirectX::XMMatrixPerspectiveFovRH(this->m_FoV,
			this->m_wHeight / this->m_wWidth,
			this->m_nearPlane,
			this->m_farPlane
		)
	);

	return 0;
}


