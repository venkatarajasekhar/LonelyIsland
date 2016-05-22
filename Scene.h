#pragma once
#include <list>
#include <memory>

#include "Camera.h"
#include "Geometry.h"
#include "RenderableObject.h"

class Scene {
	std::shared_ptr<Camera> pCamera;
	// TODO: If we'll use the same object for different scenes,
	// add shared_ptr here and update destructor.
	std::list<RenderableObject *> Objects;

public:
	Scene() : pCamera(new Camera()) {
		Objects.push_back(new Cube());
	}

	Camera *getCameraPtr() {
		return pCamera.get();
	}

	RenderableObject *getObject() {
		return Objects.front();
	}

	~Scene() {
		for (auto pObject : Objects)
			delete pObject;
	}
};
