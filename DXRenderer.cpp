#include <d3dcompiler.h>

#include "Renderer.h"

HRESULT DXRenderer::InitDXDeviceAndSwapChain() {
	unsigned createDeviceFlags = 0;
#if defined(_DEBUG) || defined(DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // defined(_DEBUG) || defined(DEBUG)

	// Supported features.
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.BufferDesc.Width = WindowWidth;
	SwapChainDesc.BufferDesc.Height = WindowHeight;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = hTargetWindow;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.Windowed = TRUE;

	// Set of driver types we will iterate over to detect something accessible.
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	HRESULT res;
	for (UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE(driverTypes); ++driverTypeIndex) {
		d3dDriverType = driverTypes[driverTypeIndex];
		res = D3D11CreateDeviceAndSwapChain(NULL, d3dDriverType, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pd3dDevice, &d3dFeatureLevel, &pImmediateContext);
		if (SUCCEEDED(res))
			break;
	}

	if (FAILED(res)) {
		//TODO: Process error here
		return res;
	}

	return S_OK;
}

HRESULT DXRenderer::InitRenderTarget() {
	HRESULT res;
	ID3D11Texture2D* pBackBuffer = NULL;
	res = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer);
	if (FAILED(res))
		return res;

	res = pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(res)) {
		//TODO: Process error here
		return res;
	}

	res = pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(res))
		return res;

	// Creating surface for Z-buffer
	D3D11_TEXTURE2D_DESC DepthStencilDesc;
	ZeroMemory(&DepthStencilDesc, sizeof(DepthStencilDesc));
	DepthStencilDesc.Width = WindowWidth;
	DepthStencilDesc.Height = WindowHeight;
	DepthStencilDesc.MipLevels = 1;
	DepthStencilDesc.ArraySize = 1;
	DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilDesc.SampleDesc.Count = 1;
	DepthStencilDesc.SampleDesc.Quality = 0;
	DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilDesc.CPUAccessFlags = 0;
	DepthStencilDesc.MiscFlags = 0;
	res = pd3dDevice->CreateTexture2D(&DepthStencilDesc, NULL, &pDepthStencil);
	if (FAILED(res))
		return res;

	// Creating Z-buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc;
	ZeroMemory(&DepthStencilViewDesc, sizeof(DepthStencilViewDesc));
	DepthStencilViewDesc.Format = DepthStencilDesc.Format;
	DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DepthStencilViewDesc.Texture2D.MipSlice = 0;
	res = pd3dDevice->CreateDepthStencilView(pDepthStencil, &DepthStencilViewDesc, &pDepthStencilView);
	if (FAILED(res))
		return res;

	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	return S_OK;
}

HRESULT DXRenderer::InitViewport() {
	D3D11_VIEWPORT Viewport;
	Viewport.Width = (FLOAT) WindowWidth;
	Viewport.Height = (FLOAT) WindowHeight;
	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;
	Viewport.TopLeftX = 0;
	Viewport.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1, &Viewport);

	return S_OK;
}

HRESULT DXRenderer::CompileShader(LPCWSTR lpszFileName, ID3DBlob **ppBlobOut, LPCSTR szShaderModel, LPCSTR szEntry) {
	HRESULT res = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	res = D3DX11CompileFromFile(lpszFileName, NULL, NULL, szEntry,
		szShaderModel, dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);

	if (FAILED(res)) {
		EmitError(_T("Error occured while compiling shader!"));
		if (pErrorBlob) {
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		return res;
	}
	if (pErrorBlob)
		pErrorBlob->Release();
	return S_OK;
}

HRESULT DXRenderer::InitShaders() {
	HRESULT res;

	ID3DBlob *pShaderBlob = nullptr;
	res = CompileShader(_T("VertexShader.fx"), &pShaderBlob, "vs_4_0");
	if (FAILED(res)) {
		// The error is already emitted.
		return res;
	}

	// Vertex shader
	res = pd3dDevice->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(),
		nullptr, &pVertexShader);
	if (FAILED(res)) {
		EmitError(_T("Cannot create vertex shader!"));
		if (pShaderBlob)
			pShaderBlob->Release();
		return res;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	res = pd3dDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pShaderBlob->GetBufferPointer(),
		pShaderBlob->GetBufferSize(), &pVertexLayout);
	pShaderBlob->Release();
	if (FAILED(res))
		return res;

	pImmediateContext->IASetInputLayout(pVertexLayout);

	// Pixel shader
	pShaderBlob = nullptr;
	res = CompileShader(_T("PixelShader.fx"), &pShaderBlob, "ps_4_0");
	if (FAILED(res)) {
		// The error is already emitted.
		return res;
	}

	res = pd3dDevice->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, &pPixelShader);
	// Release pointer before we may fall out.
	if (pShaderBlob)
		pShaderBlob->Release();
	if (FAILED(res)) {
		EmitError(_T("Cannot create pixel shader!"));
		return res;
	}

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	res = pd3dDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);
	if (FAILED(res)) {
		EmitError(_T("ConstantBuffer creation failed!"));
		return res;
	}

	return S_OK;
}

void DXRenderer::Render(Scene *pScene) {
	float ClearColor[4] = { 0.1f, 0.9f, 0.1f, 1.0f };
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	ConstantBuffer ShaderInput;
	Camera *pCamera = pScene->getCameraPtr();
	ShaderInput.mWorld = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
	ShaderInput.mView = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(pCamera->mPosition, pCamera->mEye, pCamera->mUp));
	ShaderInput.mProjection = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, WindowWidth / (FLOAT)WindowHeight, 0.01f, 100.0f));
	pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &ShaderInput, 0, 0);

	pImmediateContext->VSSetShader(pVertexShader, NULL, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pImmediateContext->PSSetShader(pPixelShader, NULL, 0);

	if (FAILED(DrawGeometry(pScene))) {
		EmitError(_T("Drawing geometry failed!"));
		return;
	}

	pSwapChain->Present(0, 0);
}

HRESULT DXRenderer::DrawGeometry(Scene *pScene) {
	HRESULT res;
	D3D11_BUFFER_DESC TempBuffer;
	ZeroMemory(&TempBuffer, sizeof(TempBuffer));
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	RenderableObject *Cube = pScene->getObject();

	TempBuffer.Usage = D3D11_USAGE_DEFAULT;
	TempBuffer.ByteWidth = sizeof(SimpleVertex) * Cube->getNumVertices();
	TempBuffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	TempBuffer.CPUAccessFlags = 0;
	InitData.pSysMem = Cube->getVertices();
	res = pd3dDevice->CreateBuffer(&TempBuffer, &InitData, &pVertexBuffer);
	if (FAILED(res))
		return res;
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	TempBuffer.Usage = D3D11_USAGE_DEFAULT;
	TempBuffer.ByteWidth = sizeof(WORD) * Cube->getNumIndices(RenderableObject::Triangles);
	TempBuffer.BindFlags = D3D11_BIND_INDEX_BUFFER;
	TempBuffer.CPUAccessFlags = 0;
	InitData.pSysMem = Cube->getIndices(RenderableObject::Triangles);
	res = pd3dDevice->CreateBuffer(&TempBuffer, &InitData, &pIndexBuffer);
	if (FAILED(res))
		return res;
	pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	TempBuffer.Usage = D3D11_USAGE_DEFAULT;
	TempBuffer.ByteWidth = sizeof(ConstantBuffer);
	TempBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	TempBuffer.CPUAccessFlags = 0;
	res = pd3dDevice->CreateBuffer(&TempBuffer, NULL, &pConstantBuffer);
	if (FAILED(res))
		return res;

	pImmediateContext->DrawIndexed(Cube->getNumIndices(RenderableObject::Triangles), 0, 0);

	return S_OK;
}