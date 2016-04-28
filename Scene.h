#pragma once
#include <DirectXMath.h>
#include <list>

#include "Camera.h"
#include "Geometry.h"
#include "RenderableObject.h"

class Scene {
	Camera *pCamera;
	std::list<RenderableObject> Objects;

public:
	Scene() : pCamera(new Camera()) {
		Objects.back(new Cube());
	}
};
