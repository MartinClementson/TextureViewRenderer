#include "DirectXHandler.h"



DirectXHandler::DirectXHandler()
{
}


DirectXHandler::~DirectXHandler()
{
}

int DirectXHandler::Initialize(HWND wndHandle)
{
	CreateContext(wndHandle);
	SetViewPort(WINDOW_WIDTH, WINDOW_HEIGHT);
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
	//gDeviceContext->Draw(6, 0);

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

	
	return hr;
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
