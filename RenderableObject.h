#pragma once
#include <new>

#include "ErrorManager.h"
#include "Geometry.h"

class RenderableObject {
public:
	enum Topology {
		Triangles
	};

	virtual const SimpleVertex *getVertices() const = 0;
	virtual unsigned getNumVertices() const = 0;
	virtual WORD *getIndices(Topology) const = 0;
	virtual unsigned getNumIndices(Topology) const = 0;
};

class Cube : public RenderableObject {
	SimpleVertex mVertices[8];
	unsigned mNumVertices;
	mutable WORD *pTriangularIndices;
	mutable unsigned mNumTriangularIndices;

	WORD *getIndicesForTriangleTopology() const {
		// Indices are created lazily
		if (!pTriangularIndices) {
			pTriangularIndices = (WORD *) ::operator new (mNumTriangularIndices * sizeof(WORD));
			WORD PrettyIndices[] = {
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
	/* Default vertices are:
	mVertices[0] = { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),  DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) };
	mVertices[1] = { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),   DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) };
	mVertices[2] = { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),    DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) };
	mVertices[3] = { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),   DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	mVertices[4] = { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) };
	mVertices[5] = { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) };
	mVertices[6] = { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),   DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
	mVertices[7] = { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) };
	*/

	Cube(DirectX::XMFLOAT3 mCenter = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), float edge = 1) : mNumVertices(8),
		pTriangularIndices(nullptr), mNumTriangularIndices(6/*Shapes*/ * 2/*Triangles per shape*/ * 3/*Vertices per triangle*/) {
		const float d = edge / 2;
		for (unsigned i = 0; i < 8; i++) {
			// Too dirty
			float x = ((i & 1) << 1 == (i & 2)) ? mCenter.x - d : mCenter.x + d;
			float y = i >= 4 ? mCenter.y - d : mCenter.y + d;
			float z = (i & 2) ? mCenter.z + d : mCenter.z - d;
			mVertices[i] = { DirectX::XMFLOAT3(x, y, z),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) };
		}
	}

	virtual unsigned getNumVertices() const {
		return mNumVertices;
	}

	virtual const SimpleVertex *getVertices() const override {
		return &mVertices[0];
	}

	virtual WORD *getIndices(Topology T) const override {
		switch (T) {
		case Triangles:
			return getIndicesForTriangleTopology();
		default:
			EmitError(_T("Unknown topology type!"));
			return nullptr;
		}
	}

	virtual unsigned getNumIndices(Topology T) const override {
		switch (T) {
		case Triangles:
			return mNumTriangularIndices;
		default:
			EmitError(_T("Unknown topology type!"));
			return 0;
		}
	}
};