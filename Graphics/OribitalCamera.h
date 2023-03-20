#pragma once
#include "glm/glm.hpp"

class OribitalCamera
{
public:
	OribitalCamera();
	~OribitalCamera();

	void SetTarget(glm::mat4 transform, float distanceToTarget);

protected:
	float m_distanceToTarget;
	float m_oribtalSpeed;
};

