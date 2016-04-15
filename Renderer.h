#pragma once

#include <d3d11.h>
//#include <d3dx11.h>
#include <windows.h>

class IRenderer {
public:
	virtual void Init(HINSTANCE hInstance, HWND hWnd) = 0;
	virtual void Render() = 0;
};

class DXRenderer : public IRenderer {
	ID3D11Device *pd3dDevice;
	D3D_DRIVER_TYPE d3dDriverType;
	D3D_FEATURE_LEVEL d3dFeatureLevel;
	ID3D11DeviceContext *pd3dDeviceContext;
	IDXGISwapChain *pSwapChain;
	ID3D11DeviceContext *pImmediateContext;
	ID3D11RenderTargetView *pd3dRenderTargetView;

	HWND hTargetWindow;
	unsigned WindowHeight, WindowWidth;

	HRESULT InitDXDeviceAndSwapChain();
	HRESULT InitRenderTarget();
public:
	virtual void Init(HINSTANCE hInstance, HWND hWnd) {
		RECT TargetWindowRect;
		GetClientRect(hWnd, &TargetWindowRect);
		WindowHeight =  TargetWindowRect.bottom - TargetWindowRect.top;
		WindowWidth = TargetWindowRect.right - TargetWindowRect.left;
		hTargetWindow = hWnd;

		// TODO: catch errors
		InitDXDeviceAndSwapChain();
		InitRenderTarget();
	}

	virtual void Render() {
		return;
	}

	~DXRenderer() {
		pd3dDevice->Release();
	}
};
