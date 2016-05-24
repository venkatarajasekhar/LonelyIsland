#pragma once
#include <DirectXMath.h>

struct Camera {
	DirectX::XMVECTOR mPosition;
	DirectX::XMVECTOR mUp;
	DirectX::XMVECTOR mEye;

	float mSpeed;

	Camera() :
		mPosition(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
		mUp(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
		mEye(DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f)),
		mSpeed(0.2f)
	{}

	void moveForward() {
		mPosition = DirectX::XMVectorAdd(mPosition, DirectX::XMVectorScale(mEye, mSpeed));
	}

	void moveBackward() {
		mPosition = DirectX::XMVectorSubtract(mPosition, DirectX::XMVectorScale(mEye, mSpeed));
	}
};