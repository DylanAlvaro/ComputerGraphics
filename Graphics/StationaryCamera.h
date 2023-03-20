#pragma once
#include "glm/glm.hpp"
#include "SimpleCamera.h"

class StationaryCamera : SimpleCamera
{
	void SetRotation(glm::vec3 rotation);

	void Update(float deltaTime) override {}
};

