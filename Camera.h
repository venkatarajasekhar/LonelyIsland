#pragma once
#include <DirectXMath.h>

struct Camera {
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mUp;
	DirectX::XMFLOAT3 mEye;
};