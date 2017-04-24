#pragma once
#include "pch.h"
#include "TweakBar.h"
#include "Model.h"
#include "Camera.h"


class DirectXHandler
{
private:

	float clearColor[4] = { 1, 0, 0, 1 };
	IDXGISwapChain*		 m_SwapChain	 = nullptr;
	ID3D11Device*		 m_Device		 = nullptr;
	ID3D11DeviceContext* m_DeviceContext = nullptr;
	
	// Buffers
	ID3D11RenderTargetView* m_BackbufferRTV		= nullptr;
	ID3D11Buffer*			m_VertexBuffer		= nullptr;
	ID3D11Buffer*			m_wvpConstantBuffer = nullptr;
	ID3D11Buffer*			m_lightConstBuffer  = nullptr;

	ModelViewProjection m_wvpData;

	
	ID3D11RasterizerState*	  m_rasterizerState;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11Texture2D*		m_depthBuffer	   = nullptr;

	ID3D11InputLayout*	  m_VertexLayout	 = nullptr;
	//Shaders
	ID3D11VertexShader*   m_VertexShader	 = nullptr;
	ID3D11GeometryShader* m_GeometryShader   = nullptr;
	ID3D11PixelShader*	  m_PixelShader		 = nullptr;


	//Textures
	ID3D11Texture2D			 *m_Texture		= nullptr;
	ID3D11ShaderResourceView *m_TextureView = nullptr;
	UI::TweakBar* m_tweakbar;


	//Models
	Model* m_models[NUM_MESH_TYPES];

	Camera m_cam;
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
	void UpdateConstBuffer(ModelViewProjection* data);
	void SetViewPort(float width, float height);
};

