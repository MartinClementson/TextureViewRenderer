#pragma once
#include "pch.h"
#include "TweakBar.h"
#include "Model.h"
#include "Camera.h"
#include "WICTextureLoader.h"

class DirectXHandler
{
private:

	float clearColor[4] = { 1, 0, 0, 1 };
	IDXGISwapChain*		 m_SwapChain	 = nullptr;
	ID3D11Device*		 m_Device		 = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;
	
	// Buffers
	ID3D11RenderTargetView* m_BackbufferRTV		  = nullptr;
	ID3D11Buffer*			m_VertexBuffer		  = nullptr;
	ID3D11Buffer*			m_wvpConstantBuffer	  = nullptr;
	ID3D11Buffer*			m_lightConstBuffer	  = nullptr;
	ID3D11Buffer*			m_variableConstBuffer = nullptr;

	ModelViewProjection		m_wvpData;
	LightBuffer				m_lightData;
	variablesBuffer			m_variableData;
	
	ID3D11RasterizerState*	m_rasterizerState;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D*		m_depthBuffer	   = nullptr;

	ID3D11InputLayout*	  m_VertexLayout	 = nullptr;
	//Shaders
	ID3D11VertexShader*   m_VertexShader	 = nullptr;
	ID3D11GeometryShader* m_GeometryShader   = nullptr;
	ID3D11PixelShader*	  m_PixelShader		 = nullptr;
	//SamplerState
	ID3D11SamplerState* m_SampleState = nullptr;

	//Textures
	ID3D11Texture2D*		  m_currentCustomTexture		 = nullptr;
	ID3D11ShaderResourceView* m_currentCustomSRV		= nullptr;
	UI::TweakBar* m_tweakbar;

	
	//Models
	Model* m_models[NUM_MESH_TYPES];

	Camera m_cam;

	float m_rotationValue = 0;
public:
	DirectXHandler();
	~DirectXHandler();
	int Initialize(HWND wndHandle);
	int Update(float dt);
	int Render(float dt);

private:
	int CreateContext(HWND wndHandle);
	int CreateShaders();
	int CreateConstantBuffer();
	int GenerateMipMaps(ID3D11Resource* source, ID3D11Resource** mipTextures, int numMips);
	void UpdateWVPConstBuffer(ModelViewProjection* data);
	void UpdateLightConstBuffer(LightBuffer* data);
	void UpdateVariableConstBuffer(variablesBuffer * data);
	void SetViewPort(float width, float height);
	
};

