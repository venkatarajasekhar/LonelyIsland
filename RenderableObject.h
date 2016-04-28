#pragma once
#include <new>

#include "Geometry.h"

class RenderableObject {
protected:
	enum Topology {
		Triangles
	};
public:
	virtual SimpleVertex *getVertices() const = 0;
	virtual unsigned getNumVertices() const = 0;
	virtual unsigned *getIndices(Topology) const = 0;
	virtual unsigned getNumIndices(Topology) const = 0;
};

class Cube : public RenderableObject {
	SimpleVertex mVertices[8];
	unsigned mNumVertices;
	mutable unsigned *pTriangularIndices, mNumTriangularIndices;

	unsigned *getIndicesForTriangleTopology() const {
		// Indices are created lazily
		if (!pTriangularIndices) {
			mNumTriangularIndices = 6/*Shapes*/ * 2/*Triangles per shape*/ * 3/*Vertices per triangle*/;
			pTriangularIndices = (unsigned *) ::operator new (mNumTriangularIndices * sizeof(unsigned));
			unsigned PrettyIndices[] = {
				3, 1, 0,
				2, 1, 3,

				0, 5, 4,
				1, 5, 0,

				3, 4, 7,
				0, 4, 3,

				1, 6, 5,
				2, 6, 1,

				2, 7, 6,
				3, 7, 2,

				6, 4, 5,
				7, 4, 6,
			};
			std::copy(PrettyIndices, PrettyIndices + mNumTriangularIndices, pTriangularIndices);
		}
		return pTriangularIndices;
	}
public:
	Cube() : mNumVertices(8), pTriangularIndices(nullptr), mNumTriangularIndices(0) {
		mVertices[0] = { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) };
		mVertices[1] = { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),   DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) };
		mVertices[2] = { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),    DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) };
		mVertices[3] = { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),   DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
		mVertices[4] = { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) };
		mVertices[5] = { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
		mVertices[6] = { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
		mVertices[7] = { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	}

	virtual unsigned getNumVertices() const {
		return mNumVertices;
	}

	virtual SimpleVertex *getVertices() const override {
		return &mVertices[0];
	}

	virtual unsigned *getIndices(Topology T) const override {
		switch (T) {
		case Triangles:
			return getIndicesForTriangleTopology();
		default:
			return nullptr;
		}
	}

	virtual unsigned getNumIndices(Topology T) const override {
		switch (T) {
		case Triangles:
			return mNumTriangularIndices;
		default:
			return 0;
		}
	}
};