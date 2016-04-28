#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <DirectXMath.h>
#include <tchar.h>
#include <windows.h>

#include "ErrorManager.h"

class IRenderer {
public:
	virtual void Init(HINSTANCE hInstance, HWND hWnd) = 0;
	virtual void Render() = 0;
	virtual bool isInvalid() const = 0;
};


class DXRenderer : public IRenderer {
	ID3D11Device *pd3dDevice;
	D3D_DRIVER_TYPE d3dDriverType;
	D3D_FEATURE_LEVEL d3dFeatureLevel;
	ID3D11DeviceContext *pd3dDeviceContext;
	IDXGISwapChain *pSwapChain;
	ID3D11DeviceContext *pImmediateContext;
	ID3D11RenderTargetView *pRenderTargetView;
	ID3D11Texture2D *pDepthStencil;
	ID3D11DepthStencilView *pDepthStencilView;
	ID3D11VertexShader *pVertexShader;
	ID3D11PixelShader *pPixelShader;
	ID3D11InputLayout *pVertexLayout;
	ID3D11Buffer *pVertexBuffer;
	ID3D11Buffer *pIndexBuffer;
	ID3D11Buffer *pConstantBuffer;

	HWND hTargetWindow;
	unsigned WindowHeight, WindowWidth;

	HRESULT InitDXDeviceAndSwapChain();
	HRESULT InitRenderTarget();
	HRESULT InitViewport();

	bool Invalid;

	struct ConstantBuffer
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};

	HRESULT CompileShader(LPCWSTR lpszFileName, ID3DBlob **ppBlobOut, LPCSTR szShaderModel, LPCSTR szEntry = "main");
	HRESULT InitShaders();

public:
	DXRenderer() : pd3dDevice(nullptr), d3dDriverType(D3D_DRIVER_TYPE_UNKNOWN),
		d3dFeatureLevel(D3D_FEATURE_LEVEL_11_0), pd3dDeviceContext(nullptr),
		pSwapChain(nullptr), pImmediateContext(nullptr), pRenderTargetView(nullptr),
		pDepthStencil(nullptr), pDepthStencilView(nullptr), pVertexShader(nullptr),
		pPixelShader(nullptr), pVertexLayout(nullptr), pVertexBuffer(nullptr),
		pIndexBuffer(nullptr), pConstantBuffer(nullptr), hTargetWindow(nullptr),
		WindowHeight(0), WindowWidth(0), Invalid(true)
			{}

	virtual bool isInvalid() const {
		return Invalid;
	}

	virtual void Init(HINSTANCE hInstance, HWND hWnd) {
		RECT TargetWindowRect;
		GetClientRect(hWnd, &TargetWindowRect);
		WindowHeight =  TargetWindowRect.bottom - TargetWindowRect.top;
		WindowWidth = TargetWindowRect.right - TargetWindowRect.left;
		hTargetWindow = hWnd;

		HRESULT res;
		res = InitDXDeviceAndSwapChain();
		if (FAILED(res)) {
			EmitError(_T("Creating DX device and swap chain failed!"));
			return;
		}
		res = InitRenderTarget();
		if (FAILED(res)) {
			EmitError(_T("Creating DX render target failed!"));
			return;
		}
		res = InitShaders();
		if (FAILED(res)) {
			EmitError(_T("Creating DX render target failed!"));
			return;
		}
		InitViewport();
		Invalid = false;
	}

	virtual void Render() {
		return;
	}

	~DXRenderer() {
		if (pd3dDevice) pd3dDevice->Release();
		if (pd3dDeviceContext) pd3dDeviceContext->Release();
		if (pSwapChain) pSwapChain->Release();
		if (pImmediateContext) pImmediateContext->Release();
		if (pRenderTargetView) pRenderTargetView->Release();
		if (pDepthStencil) pDepthStencil->Release();
		if (pDepthStencilView) pDepthStencilView->Release();
		Invalid = true;
	}
};