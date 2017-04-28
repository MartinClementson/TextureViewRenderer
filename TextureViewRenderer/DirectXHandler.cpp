#include "DirectXHandler.h"



DirectXHandler::DirectXHandler()
{
	DirectX::XMStoreFloat4x4(&this->m_wvpData.model,DirectX::XMMatrixIdentity());
}


DirectXHandler::~DirectXHandler()
{
	m_VertexBuffer		!= nullptr ? m_VertexBuffer		->Release(): NULL;
	m_VertexLayout		!= nullptr ? m_VertexLayout	    ->Release(): NULL;
	m_VertexShader		!= nullptr ? m_VertexShader		->Release(): NULL;
	m_PixelShader		!= nullptr ? m_PixelShader	    ->Release(): NULL;
	m_GeometryShader	!= nullptr ? m_GeometryShader	->Release(): NULL;
	m_BackbufferRTV		!= nullptr ? m_BackbufferRTV	->Release(): NULL;
	m_SwapChain			!= nullptr ? m_SwapChain		->Release(): NULL;
	m_depthBuffer		!= nullptr ? m_depthBuffer		->Release(): NULL;
	m_depthStencilView	!= nullptr ? m_depthStencilView	->Release(): NULL;
	m_Device			!= nullptr ? m_Device			->Release(): NULL;
	m_DeviceContext		!= nullptr ? m_DeviceContext	->Release(): NULL;
	m_SampleState		!= nullptr ? m_SampleState		->Release(): NULL;

	m_currentCustomTexture != nullptr ? m_currentCustomTexture->Release() : NULL;
	m_currentCustomSRV	   != nullptr ? m_currentCustomSRV	 ->Release() : NULL;

	for (size_t i = 0; i < NUM_MESH_TYPES; i++)
	{
		delete m_models[i];
	}
}

int DirectXHandler::Initialize(HWND wndHandle)
{
	CoInitialize((LPVOID)0); //initialize texture loader
	assert(CreateContext(wndHandle) == 1);
	SetViewPort(WINDOW_WIDTH, WINDOW_HEIGHT);
	this->m_tweakbar = UI::TweakBar::GetInstance();
	this->m_tweakbar->Initialize(m_Device, WINDOW_WIDTH, WINDOW_HEIGHT);
	assert(CreateShaders() == 1);
	assert(CreateConstantBuffer() == 1);
	this->m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
	this->m_DeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->m_DeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->m_DeviceContext->GSSetShader(m_GeometryShader, nullptr, 0);
	this->m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);
	MeshDataHandler::GetInstance()->m_initialize(m_Device);
	//m_DeviceContext->PSSetShaderResources(0, 1, &m_TextureView);

	m_lightData.camPos = DirectX::XMFLOAT4(0.0f, 0.0f, 5.0f, 1.0f);
	m_lightData.lightPos[0] = DirectX::XMFLOAT4(0.0f, 5.0f, 5.0f, 1.0f);
	m_lightData.intensity[0] = 1.0f;
	m_lightData.diffuseColor[0] = DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	this->UpdateLightConstBuffer(&m_lightData);
	
	Material materials[NUM_MESH_TYPES];
#pragma region Load textures
	std::string texturePathSource = "Blank.png";


	std::string mipTexturePaths[9]
	{
		"MipTextures/1.png",
		"MipTextures/2.png",
		"MipTextures/3.png",
		"MipTextures/4.png",
		"MipTextures/5.png",
		"MipTextures/6.png",
		"MipTextures/7.png",
		"MipTextures/8.png",
		"MipTextures/9.png",
	};
	ID3D11ShaderResourceView *mipTextureSRV[9] = {nullptr};
	ID3D11Resource*			  mipTextureRes[9] = {nullptr};
			
	size_t length = strlen(texturePathSource.c_str());
	wchar_t path[256];
	mbstowcs_s(&length, path, texturePathSource.c_str(), length);

	HRESULT hr = DirectX::CreateWICTextureFromFile(m_Device, m_DeviceContext, path, &materials[0].textureResource, &materials[0].m_TextureView);
	if (FAILED(hr))
	{
		printf("FAILED Loading texture");
	}


	for (size_t i = 0; i < 9; i++)
	{
		size_t length = strlen(mipTexturePaths[i].c_str());
		wchar_t path[256];
		mbstowcs_s(&length, path, mipTexturePaths[i].c_str(), length);

		HRESULT hr = DirectX::CreateWICTextureFromFile(m_Device, m_DeviceContext, path, &mipTextureRes[i], &mipTextureSRV[i]);
		if (FAILED(hr))
		{
			printf("FAILED Loading texture : ");
			printf(mipTexturePaths[i].c_str());
		}

	}
		m_DeviceContext->PSSetShaderResources(1, 9, mipTextureSRV);
#pragma endregion 
	m_DeviceContext->PSSetShaderResources(0, 1, &materials[0].m_TextureView);
	GenerateMipMaps(materials[0].textureResource, mipTextureRes, 9);

//for (size_t i = 0; i < 9; i++)
//{
//	mipTextureSRV[i]->Release();
//	mipTextureRes[i]->Release();
//}

	for (size_t i = 0; i < NUM_MESH_TYPES; i++)
	{
		m_models[i] = new Model(m_Device, MeshDataHandler::GetInstance()->GetMeshData(MeshType(i)),&materials[i]);
	}

	D3D11_SAMPLER_DESC samplerDesc;
	// use linear interpolation for minification, magnification, and mip-level sampling (quite expensive)
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;// D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
														 //for all filters: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476132(v=vs.85).aspx

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; //wrap, (repeat) for use of tiling texutures
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f; //mipmap offset level
	samplerDesc.MaxAnisotropy = 1; //Clamping value used if D3D11_FILTER_ANISOTROPIC or D3D11_FILTER_COMPARISON_ANISOTROPIC is specified in Filter. Valid values are between 1 and 16.
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0; //0 most detailed mipmap level, higher number == less detail
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 1.0f;




	hr = m_Device->CreateSamplerState(&samplerDesc, &m_SampleState);
	if (FAILED(hr))
	{
		printf("Failed to create samplerstate");
	}
	else
	{
		m_DeviceContext->PSSetSamplers(0, 1, &this->m_SampleState);
	}
	return 0;
}

int DirectXHandler::Update(float dt)
{
	UI::UIelements* elements = m_tweakbar->GetUiData();
	DirectX::XMFLOAT3 newPos = DirectX::XMFLOAT3(0.0f, 0.0f, elements->cameraDistance);
	m_cam.SetPosition(newPos);
	m_lightData.camPos.z = elements->cameraDistance;
	UpdateLightConstBuffer(&m_lightData);
	m_wvpData.projection = m_cam.GetProjectionMatrix();
	m_wvpData.view		 = m_cam.GetViewMatrix();

	if (elements->rotate)
	{
		DirectX::XMFLOAT4X4 rotMatrix;
		m_rotationValue += 0.001f;
		if (m_rotationValue >= 360)
			m_rotationValue = 0;
		DirectX::XMStoreFloat4x4(&rotMatrix, DirectX::XMMatrixRotationY(m_rotationValue));
		m_models[elements->currentMesh]->SetRotation(rotMatrix);
		m_models[elements->currentMesh]->Update();
	}
	//m_models[elements->currentMesh]->SetRotation()
	m_wvpData.model		 = m_models[elements->currentMesh]->GetTransformationMatrix();
	UpdateWVPConstBuffer(&m_wvpData);

	m_variableData.MIPBitPrecision[0]  = elements->mip0;
	m_variableData.MIPBitPrecision[1]  = elements->mip1;
	m_variableData.MIPBitPrecision[2]  = elements->mip2;
	m_variableData.MIPBitPrecision[3]  = elements->mip3;
	m_variableData.MIPBitPrecision[4]  = elements->mip4;
	m_variableData.MIPBitPrecision[5]  = elements->mip5;
	m_variableData.MIPBitPrecision[6]  = elements->mip6;
	m_variableData.MIPBitPrecision[7]  = elements->mip7;
	m_variableData.MIPBitPrecision[8]  = elements->mip8;
	m_variableData.MIPBitPrecision[9]  = elements->mip9;
	m_variableData.MIPBitPrecision[10] = elements->mip10;
	UpdateVariableConstBuffer(&m_variableData);

	ID3D11Buffer* vertBuff  = m_models[elements->currentMesh]->GetMeshData()->vertexBuffer;
	ID3D11Buffer* indexBuff = m_models[elements->currentMesh]->GetMeshData()->indexBuffer;
	
	UINT32 vertexSize = sizeof(VertexData);
	UINT32 offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &vertBuff, &vertexSize, &offset);
	m_DeviceContext->IASetIndexBuffer(indexBuff, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	return 0;
}

int DirectXHandler::Render(float dt)
{
	UI::UIelements* elements = m_tweakbar->GetUiData();
	m_DeviceContext->ClearRenderTargetView(m_BackbufferRTV, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);


	m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
	//m_DeviceContext->HSSetShader(nullptr, nullptr, 0);
	//m_DeviceContext->DSSetShader(nullptr, nullptr, 0);
	m_DeviceContext->GSSetShader(m_GeometryShader, nullptr, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);
	//m_DeviceContext->PSSetShaderResources(0, 1, &m_TextureView);

	m_DeviceContext->OMSetRenderTargets(1, &m_BackbufferRTV, m_depthStencilView);

	m_DeviceContext->RSSetState(this->m_rasterizerState); //Set the rasterstate


	//UINT32 vertexSize = sizeof(TriangleVertex);
	//UINT32 offset = 0;
	//m_DeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer, &vertexSize, &offset);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->IASetInputLayout(m_VertexLayout);
	//
	//
	m_DeviceContext->GSSetConstantBuffers(1, 1, &m_wvpConstantBuffer);
	m_DeviceContext->DrawIndexed(m_models[elements->currentMesh]->GetMeshData()->numIndices, 0, 0);
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
	scd.SampleDesc.Count  = 1;                                // how many multisamples
	scd.Windowed		  =	TRUE;                             // windowed/full-screen mode


															// create a device, device context and swap chain using the information in the scd struct
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
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
	desc.Width			    = (UINT)WINDOW_WIDTH;
	desc.Height			    = (UINT)WINDOW_HEIGHT;
	desc.MipLevels		    = 1;
	desc.ArraySize		    = 1;
	desc.Format			    = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count   = 1;
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


	

		// Create the rasterizer state
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

		rasterizerDesc.AntialiasedLineEnable = false;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;//D3D11_CULL_BACK; //Enable backface culling
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = false;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;

		hr = this->m_Device->CreateRasterizerState(&rasterizerDesc, &this->m_rasterizerState);
		if (FAILED(hr))
		{
			return 0;
		}

		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackbufferRTV);
		//pBackBuffer->Release();

		// set the render target as the back buffer
		m_DeviceContext->OMSetRenderTargets(1, &m_BackbufferRTV, m_depthStencilView);

		m_DeviceContext->RSSetState(this->m_rasterizerState); //Set the rasterstate

	
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
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

int DirectXHandler::CreateConstantBuffer()
{
	HRESULT hr;
	//Gör en description av buffern
	CD3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth		   = sizeof(ModelViewProjection);
	bufferDesc.BindFlags		   = D3D11_BIND_CONSTANT_BUFFER; //detta är en konstant buffer
	bufferDesc.Usage			   = D3D11_USAGE_DYNAMIC; //Read only from gpu, write from cpu
	bufferDesc.CPUAccessFlags	   = D3D11_CPU_ACCESS_WRITE;  // vi måste förtydliga det.
	bufferDesc.MiscFlags		   = 0;
	bufferDesc.StructureByteStride = 0;

	//Vi skapar buffern i device

	hr = m_Device->CreateBuffer(&bufferDesc, nullptr, &m_wvpConstantBuffer);

	if (SUCCEEDED(hr)) {

		m_DeviceContext->GSSetConstantBuffers(1, 1, &m_wvpConstantBuffer);
	}
	else
		return 0;


	//skapa Light constantBuffer


	//Gör en description av buffern
	CD3D11_BUFFER_DESC bufferDescL;
	
	
	ZeroMemory(&bufferDescL, sizeof(bufferDescL));
	
	bufferDescL.ByteWidth = sizeof(LightBuffer);
	bufferDescL.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //detta är en konstant buffer
	bufferDescL.Usage = D3D11_USAGE_DYNAMIC; //Read only from gpu, write from cpu
	bufferDescL.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  // vi måste förtydliga det.
	bufferDescL.MiscFlags = 0;
	bufferDescL.StructureByteStride = 0;
	
	//Vi skapar buffern i device
	
	hr = m_Device->CreateBuffer(&bufferDescL, nullptr, &m_lightConstBuffer);
	
	if (SUCCEEDED(hr)) {
	
		m_DeviceContext->PSSetConstantBuffers(0, 1, &m_lightConstBuffer);
	}




	CD3D11_BUFFER_DESC bufferDescV;


	ZeroMemory(&bufferDescV, sizeof(bufferDescV));

	bufferDescV.ByteWidth = sizeof(variablesBuffer);
	bufferDescV.BindFlags = D3D11_BIND_CONSTANT_BUFFER; //detta är en konstant buffer
	bufferDescV.Usage = D3D11_USAGE_DYNAMIC; //Read only from gpu, write from cpu
	bufferDescV.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;  // vi måste förtydliga det.
	bufferDescV.MiscFlags = 0;
	bufferDescV.StructureByteStride = 0;

	//Vi skapar buffern i device

	hr = m_Device->CreateBuffer(&bufferDescV, nullptr, &m_variableConstBuffer);

	if (SUCCEEDED(hr)) {

		m_DeviceContext->PSSetConstantBuffers(2, 1, &m_variableConstBuffer);
	}


	return 1;
}

int DirectXHandler::GenerateMipMaps(ID3D11Resource * source, ID3D11Resource ** mipTextures, int numMips)
{
	/*
		Here we will generate custom mip levels for the source texture.
		We do this because this project needs specialized, manually generated mip levels. This makes the standard mip generation ineffective.
	
	- Create a new texture resource with the same dimensions as the source 
	- generate the amount of mips that is specified by numMips
	- Copy the source to the new texture resource
	
	for i in numMips
		copy mipTextures[i] into newTexture.Mips[i] (Assert the resolution is the same)

	*/


	//Get the desciption of the original
	D3D11_TEXTURE2D_DESC sourceDesc;
	ID3D11Texture2D* sourceTex;
	source->QueryInterface(&sourceTex);
	sourceTex->GetDesc(&sourceDesc);
	
	//Create a new texture resource with the same dimensions as the source 
	//generate the amount of mips that is specified by numMips
	

	if (m_currentCustomTexture != nullptr || m_currentCustomSRV != nullptr)
	{
		m_currentCustomTexture->Release();
		m_currentCustomSRV->Release();
		m_currentCustomTexture = nullptr;
		m_currentCustomSRV	   = nullptr;
	}


#pragma region Create the new Texture
	D3D11_TEXTURE2D_DESC newTextDesc;

	newTextDesc.Width			     = sourceDesc.Width;
	newTextDesc.Height				 = sourceDesc.Height;
	newTextDesc.MipLevels		     = numMips +1;// index 0 is the original level
	newTextDesc.ArraySize		     = 1;
	newTextDesc.Format				 = sourceDesc.Format;
	newTextDesc.SampleDesc.Count     = 1;
	newTextDesc.SampleDesc.Quality   = 0;
	newTextDesc.Usage				 = D3D11_USAGE_DEFAULT;
	newTextDesc.BindFlags			 = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	newTextDesc.CPUAccessFlags		 = D3D11_CPU_ACCESS_WRITE;
	newTextDesc.MiscFlags			 = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	//Create the render target Texture

	HRESULT hResult = m_Device->CreateTexture2D(&newTextDesc, NULL, &m_currentCustomTexture);
	if (FAILED(hResult))
	{
		return 1;
	}
	//Set up the shader resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC DescRV;
	DescRV.Format = newTextDesc.Format;
	DescRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	DescRV.Texture2D.MipLevels = newTextDesc.MipLevels;
	DescRV.Texture2D.MostDetailedMip = 0;

	//Create the resourceView;

	hResult = m_Device->CreateShaderResourceView(m_currentCustomTexture, &DescRV, &m_currentCustomSRV);
	if (FAILED(hResult))
		return 1;

	m_DeviceContext->GenerateMips(m_currentCustomSRV);
	m_DeviceContext->PSSetShaderResources(0, 1, &m_currentCustomSRV);
#pragma endregion

	//Copy the source to the new texture resource

	/* 
		for i in numMips
			
			copy mipTextures[i] into newTexture.Mips[i] (Assert the resolution is the same)
	*/

	//m_DeviceContext->CopyResource(newTexture, source);
	m_DeviceContext->CopySubresourceRegion(m_currentCustomTexture, 0, 0, 0, 0, source, 0, NULL);

	for (size_t i = 0; i < numMips; i++) //for eaach
	{
		m_DeviceContext->CopySubresourceRegion(m_currentCustomTexture, i + 1, 0, 0, 0, mipTextures[i], 0, NULL);

	}





	return 0;
}

void DirectXHandler::UpdateWVPConstBuffer(ModelViewProjection * data)
{
	
	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	//mapping to the matrixbuffer
	this->m_DeviceContext->Map(this->m_wvpConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);
	
	ModelViewProjection* temporaryWorld = (ModelViewProjection*)mappedResourceWorld.pData;

	*temporaryWorld = *data;

	this->m_DeviceContext->Unmap(m_wvpConstantBuffer, 0);
}

void DirectXHandler::UpdateLightConstBuffer(LightBuffer * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResourceWorld;
	ZeroMemory(&mappedResourceWorld, sizeof(mappedResourceWorld));

	//mapping to the matrixbuffer
	this->m_DeviceContext->Map(this->m_lightConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceWorld);

	LightBuffer* temporaryWorld = (LightBuffer*)mappedResourceWorld.pData;

	*temporaryWorld = *data;
	this->m_DeviceContext->Unmap(m_lightConstBuffer, 0);
}


void DirectXHandler::UpdateVariableConstBuffer(variablesBuffer * data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResourceVar;
	ZeroMemory(&mappedResourceVar, sizeof(mappedResourceVar));

	//mapping to the matrixbuffer
	HRESULT hr = this->m_DeviceContext->Map(this->m_variableConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceVar);

	variablesBuffer* temporaryBuf = (variablesBuffer*)mappedResourceVar.pData;

	*temporaryBuf = *data;
	this->m_DeviceContext->Unmap(m_variableConstBuffer, 0);
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
