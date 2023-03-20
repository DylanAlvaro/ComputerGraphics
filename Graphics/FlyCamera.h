#pragma once
#include "SimpleCamera.h"

class FlyCamera : public SimpleCamera
{
public:
	FlyCamera();
	~FlyCamera();
	virtual void Update(float deltaTime);
	void SetSpeed(float speed);

protected:

	float m_turnSpeed = glm::radians(180.f);

	float m_speed;
	glm::vec3 m_up;
};

