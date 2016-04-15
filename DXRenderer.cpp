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

	res = pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pd3dRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(res)) {
		//TODO: Process error here
		return res;
	}
	return S_OK;
}