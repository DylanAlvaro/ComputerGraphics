#pragma once
#include "glm/glm.hpp"
#include "SimpleCamera.h"

class StationaryCamera : public SimpleCamera
{
public:
	StationaryCamera();
	~StationaryCamera();

	void SetRotation(glm::vec3 rotation);

	void Update(float deltaTime) override {}
};

