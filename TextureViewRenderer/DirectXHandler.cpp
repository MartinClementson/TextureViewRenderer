#include "DirectXHandler.h"



DirectXHandler::DirectXHandler()
{
}


DirectXHandler::~DirectXHandler()
{
	m_VertexBuffer != nullptr ? m_VertexBuffer->Release(): NULL;
	m_TextureView  != nullptr ? m_TextureView->Release() : NULL;
	m_VertexLayout != nullptr ? m_VertexLayout->Release(): NULL;
	m_VertexShader != nullptr ? m_VertexShader->Release(): NULL;
	m_PixelShader  != nullptr ? m_PixelShader->Release() : NULL;
	//m_GeometryShader->Release();
	m_BackbufferRTV->Release();
	m_SwapChain->Release();
	m_depthBuffer->Release();
	m_depthStencilView->Release();
	m_Device->Release();
	m_DeviceContext->Release();
}

int DirectXHandler::Initialize(HWND wndHandle)
{

	assert(CreateContext(wndHandle) == 1);
	SetViewPort(WINDOW_WIDTH, WINDOW_HEIGHT);
	this->m_tweakbar = UI::TweakBar::GetInstance();
	this->m_tweakbar->Initialize(m_Device, WINDOW_WIDTH, WINDOW_HEIGHT);
	assert(CreateShaders() == 1);
	this->m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
	this->m_DeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->m_DeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->m_DeviceContext->GSSetShader(m_GeometryShader, nullptr, 0);
	this->m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);
	//m_DeviceContext->PSSetShaderResources(0, 1, &m_TextureView);
	return 0;
}

int DirectXHandler::Update(float dt)
{
	return 0;
}

int DirectXHandler::Render(float dt)
{
	m_DeviceContext->ClearRenderTargetView(m_BackbufferRTV, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);


	//m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
	//m_DeviceContext->HSSetShader(nullptr, nullptr, 0);
	//m_DeviceContext->DSSetShader(nullptr, nullptr, 0);
	//m_DeviceContext->GSSetShader(m_GeometryShader, nullptr, 0);
	//m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);
	//m_DeviceContext->PSSetShaderResources(0, 1, &m_TextureView);




	//UINT32 vertexSize = sizeof(TriangleVertex);
	//UINT32 offset = 0;
	//m_DeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	//m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//m_DeviceContext->IASetInputLayout(gVertexLayout);
	//
	//
	
	this->m_tweakbar->Render();
	m_SwapChain->Present(0, 0);
	return 0;
}

int DirectXHandler::CreateContext(HWND wndHandle)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount		  = 1;                                // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // use 32-bit color
	scd.BufferUsage		  = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // how swap chain is to be used
	scd.OutputWindow	  = wndHandle;                        // the window to be used
	scd.SampleDesc.Count  = 4;                                // how many multisamples
	scd.Windowed		  =	TRUE;                             // windowed/full-screen mode


															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_SwapChain,
		&m_Device,
		NULL,
		&m_DeviceContext);

	if (FAILED(hr))
		return 0;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width			    = WINDOW_WIDTH;
	desc.Height			    = WINDOW_HEIGHT;
	desc.MipLevels		    = 1;
	desc.ArraySize		    = 1;
	desc.Format			    = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count   = 4;
	desc.SampleDesc.Quality = 0;
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags	    = 0;
	desc.MiscFlags			= 0;


	hr = m_Device->CreateTexture2D(&desc, 0, &m_depthBuffer);
	if (FAILED(hr))
		return 0;
	hr = m_Device->CreateDepthStencilView(m_depthBuffer, 0, &m_depthStencilView);
	if (FAILED(hr))
		return 0;


		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackbufferRTV);
		pBackBuffer->Release();

		// set the render target as the back buffer
		m_DeviceContext->OMSetRenderTargets(1, &m_BackbufferRTV, m_depthStencilView);

	
	return 1;
}

int DirectXHandler::CreateShaders()
{
	HRESULT hr;
	//create vertex shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	hr = m_Device->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &m_VertexShader);
	if (FAILED(hr))
		return 0;
	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	hr = m_Device->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &m_VertexLayout);
	if (FAILED(hr))
		return 0;
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Fragment.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	hr =m_Device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &m_PixelShader);
	if (FAILED(hr))
		return 0;
	// we do not need anymore this COM object, so we release it.
	pPS->Release();

	//create geometry shader

	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShader.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_5_0",
		0,
		0,
		&pGS,
		nullptr);




	hr = m_Device->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &m_GeometryShader);
	if (FAILED(hr))
		return 0;
	pGS->Release();

	return 1;
}

void DirectXHandler::SetViewPort(float width, float height)
{
	if (m_DeviceContext)
	{
		D3D11_VIEWPORT vp;
		vp.Width    = width;
		vp.Height   = height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_DeviceContext->RSSetViewports(1, &vp);
	}
}
