#pragma once
#include "glm/glm.hpp"
#include "SimpleCamera.h"


class OribitalCamera : public SimpleCamera
{
public:
	OribitalCamera();
	~OribitalCamera();

	void SetTarget(glm::mat4 transform, float distanceToTarget);

protected:
	float m_distanceToTarget;
	float m_oribtalSpeed;
};

